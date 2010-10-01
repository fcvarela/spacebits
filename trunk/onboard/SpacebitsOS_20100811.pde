// include these headers, otherwise GCC will fail miserably 
#include <WProgram.h>
#include <Wire.h>

// External hardware 
#include <DS1307.h> 
#include <EEPROM.h>

#include "OneWire.h"
#include "DallasTemperature.h"

//#include "NMEA.h"
#include "SCP1000.h"
#include "TSL230.h"

#include "TinyGPS.h"
#include "Spacebits.h"

char csv_header_1[] = "Hour, Minute, Second, Latitude, Longitude, Altitude, Gyro X, Gyro Y, ";
char csv_header_2[] = "Accel X, Accel Y, Accel Z, Pressure, Internal Temperature, Voltage Source, ";
char csv_header_3[] = "Current, Humidity, Bearing, Light Intensity, External Temperature, Estimmated Altitude\n";

uint16_t Read_Compass(void);

// NMEA gps = NMEA(&Serial2, 4800); // Lassen IQ GPS
TinyGPS gps;
HardwareSerial *nss = &Serial2;

void gpsdump(TinyGPS &gps);
bool feedgps();

SCP1000 scp1000 = SCP1000(SCP1000_CS_PIN);

// Data wire is plugged into port 35 on the Arduino
#define ONE_WIRE_BUS 35

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature ds18b20(&oneWire);

// arrays to hold device addresses
DeviceAddress outsideThermometer;

#define RTC_INIT  0x00


// 
// Global vars used in main loop
//
Spacebits_t data;

void setup()
{ 
  data.start_mark = 'A';
  data.end_mark =   'Z';
  
  pinMode(PARACHUTE_CUTTER, OUTPUT);
  digitalWrite(PARACHUTE_CUTTER, LOW); 
  
  // TSL230 and DS18B20 powered from I/O pins
  pinMode(LIGHT_AND_TEMP_VCC_PIN, OUTPUT);
  digitalWrite(LIGHT_AND_TEMP_VCC_PIN, HIGH);
  
  pinMode(LIGHT_AND_TEMP_GND_PIN, OUTPUT);
  digitalWrite(LIGHT_AND_TEMP_GND_PIN, LOW);

  //
  // Serial ports
  //
  
  // Used for debug over USB
  Serial.begin(115200);
   
  // Used to store data into OpenLog
  Serial1.begin(57600);
  
  // Serial2 inited by NMEA (Lassen IQ)
  Serial2.begin(4800);
    
  // XBee-PRO 868MHz radio
  // warning: radio has a maximum RF Data Rate of 24kbps 
  Serial3.begin(19200);
     
  EEPROM.write(RTC_INIT, 0x00);
  
  // Init RTC only once address 0x00 in EEPROM has not been written
  if(EEPROM.read(RTC_INIT) == 0)
  {
    RTC.stop();
    RTC.set(DS1307_SEC,   0);     // set the seconds
    RTC.set(DS1307_MIN,   0);     // set the minutes
    RTC.set(DS1307_HR,    0);     // set the hours
    RTC.set(DS1307_DOW,   0);     // set the day of the week
    RTC.set(DS1307_DATE, 25);     // set the date
    RTC.set(DS1307_MTH,   4);     // set the month
    RTC.set(DS1307_YR,   10);     // set the year 
    RTC.start();  
    
    // Don't re-start RTC clock
    EEPROM.write(RTC_INIT, 0xFF);
  }
  
  Serial1.println("Booting SpacebitOS 1.0\n");
  Serial.print("Booting SpacebitOS 1.0\n");
  
  // wait for hardware to boot up properly
  delay(2000);
  
  // 
  // Init sensors
  // 
  
  // Pressure sensor 
  scp1000.init();  
  Serial1.println("\tSCP1000: OK");
  
  // Light sensor
  // tsl230_init();
  // Serial1.println("\tTSL230: OK");
  
  // Start up the library for Dallas temperature sensor
  ds18b20.begin();
  
  if (!ds18b20.getAddress(outsideThermometer, 0))
    Serial1.println("\tDS18B20: Unable to find address for Device 0"); 
  
  // Set the resolution to 9 bit
  ds18b20.setResolution(outsideThermometer, 9);
  Serial1.println("\tDS18B20: OK");

  data.gps.f_latitude = 0;
  data.gps.f_longitude = 0;
  data.gps.u_altitude = 0;
  data.gps.u_valid = 0;
  data.gps.u_satellites = 0;

  //
  // Add CSV header and start logging 
  //
  Serial1.println("\n\n\n");
   
  Serial1.print(csv_header_1);
  Serial1.print(csv_header_2);
  Serial1.println(csv_header_3);

  Serial.println("Done");
}

unsigned int i;
unsigned long lastSampleRead = millis();
unsigned long lastLog = millis();
unsigned long lastDataPacket = millis();

void loop()
{  
  if(millis() - lastSampleRead > 500)
  {
    feedgps();
    gpsdump(gps);
    
    GetDataSample();
    LogDebug();
    
    lastSampleRead = millis();
  }
      
  if(millis() - lastLog > 2000)
  {
    LogDataSample();
    lastLog = millis();
  }
  
  if(millis() - lastDataPacket > 15000)
  {    
    Serial3.write((uint8_t*) &data, sizeof(Spacebits_t));   
    memset(&data, 0, sizeof(Spacebits_t));
    data.start_mark = 'A';
    data.end_mark =   'Z';
    lastDataPacket = millis();
  }
  
  delay(500);
  
  /*
  // failsafe 
  if(RTC.get(DS1307_HR, true) == 1 && RTC.get(DS1307_MIN, false) > 30)
  {
    digitalWrite(PARACHUTE_CUTTER, HIGH);
    for(int i=0; i < 30; i++)
      delay(1000);
  }
  */
}


uint16_t Read_Compass()
{
  // Starts communication with cmps03
  Wire.beginTransmission(CMPS03_ADDRESS);     

  // Sends the register we wish to read
  Wire.send(2);                      
  Wire.endTransmission();

  // Requests high byte
  Wire.requestFrom((uint8_t)CMPS03_ADDRESS, (uint8_t)2); 

  // While there is a byte to receive  
  while(Wire.available() < 2);         

  // reads the byte as an integer
  uint8_t highByte = Wire.receive();       
  uint8_t lowByte = Wire.receive(); 
  return ((highByte << 8) + lowByte) / 10;
}

void GetDataSample()
{
  // External Temperature
  ds18b20.requestTemperatures();
  data.extern_temperature = (int16_t) ds18b20.getTempC(outsideThermometer) * 100;
  
   // RTC time since launch hour 
  data.rtc.tm_hour = RTC.get(DS1307_HR,  true);
  data.rtc.tm_min  = RTC.get(DS1307_MIN, false);
  data.rtc.tm_sec  = RTC.get(DS1307_SEC, false);
  
  // Get probe acceleration and rotation 
  // GX, GY, AX, AY, AZ
  data.imu.gx = analogRead(IMU_GX_PIN);
  data.imu.gy = analogRead(IMU_GY_PIN);
  data.imu.ax = analogRead(IMU_AX_PIN);
  data.imu.ay = analogRead(IMU_AY_PIN);
  data.imu.az = analogRead(IMU_AZ_PIN);
   
  // Get air pressure and altitude estimation;
  scp1000.readSensor();
  data.scp.pressure    = scp1000.BaroP * 100;
  data.scp.temperature = scp1000.TempC * 100;
 
  // Get analog sensors  
  data.voltage  = analogRead(VOLTAGE_SENSOR_PIN) * 2;
  data.current  = analogRead(CURRENT_SENSOR_PIN);
  data.humidity = analogRead(HUMIDITY_SENSOR_PIN);
 
  // Get compass position relative to north pole
  data.bearing = Read_Compass();
  
  // Light intensity scaled x100
  data.light_intensity = 0; // tsl230_calc_uwatt_cm2(tsl230_get_freq()) * 100;
}

void LogDataSample()
{  
 // RTC
  Serial1.print(data.rtc.tm_hour);
  Serial1.print(", ");
  Serial1.print(data.rtc.tm_min);
  Serial1.print(", ");
  Serial1.print(data.rtc.tm_sec);
  Serial1.print(", ");
  
  // GPS
  Serial1.print(data.gps.f_latitude);
  Serial1.print(", ");
  Serial1.print(data.gps.f_longitude);
  Serial1.print(", ");
  Serial1.print(data.gps.u_altitude);
  Serial1.print(", ");

  // IMU
  Serial1.print(data.imu.gx);
  Serial1.print(", ");
  Serial1.print(data.imu.gy);
  Serial1.print(", ");
  Serial1.print(data.imu.ax);
  Serial1.print(", ");
  Serial1.print(data.imu.ay);
  Serial1.print(", ");
  Serial1.print(data.imu.az);
  Serial1.print(", ");
  
  // SCP1000
  Serial1.print(data.scp.pressure);
  Serial1.print(", ");
  Serial1.print(data.scp.temperature);
  Serial1.print(", ");
 
  // Analog Sensors
  Serial1.print(data.voltage);
  Serial1.print(", ");
  Serial1.print(data.current);
  Serial1.print(", ");
  Serial1.print(data.humidity);
  Serial1.print(", ");
  
  // And finally
  Serial1.print(data.bearing);
  Serial1.print(", ");
  Serial1.print(data.light_intensity);
  Serial1.print(", ");
  Serial1.print(data.extern_temperature);
  Serial1.print(", ");
  
  scp1000.readSensor();
  Serial1.print(scp1000_get_altitude(scp1000.rawPressure >> 2));
  
  Serial1.println();
}

void LogDebug()
{  
 // RTC
  Serial.print(data.rtc.tm_hour);
  Serial.print(", ");
  Serial.print(data.rtc.tm_min);
  Serial.print(", ");
  Serial.print(data.rtc.tm_sec);
  Serial.print(", ");
  
  // GPS
  Serial.print(data.gps.f_latitude);
  Serial.print(", ");
  Serial.print(data.gps.f_longitude);
  Serial.print(", ");
  Serial.print(data.gps.u_altitude);
  Serial.print(", ");

  // IMU
  Serial.print(data.imu.gx);
  Serial.print(", ");
  Serial.print(data.imu.gy);
  Serial.print(", ");
  Serial.print(data.imu.ax);
  Serial.print(", ");
  Serial.print(data.imu.ay);
  Serial.print(", ");
  Serial.print(data.imu.az);
  Serial.print(", ");
  
  // SCP1000
  Serial.print(data.scp.pressure);
  Serial.print(", ");
  Serial.print(data.scp.temperature);
  Serial.print(", ");
 
  // Analog Sensors
  Serial.print(data.voltage);
  Serial.print(", ");
  Serial.print(data.current);
  Serial.print(", ");
  Serial.print(data.humidity);
  Serial.print(", ");
  
  // And finally
  Serial.print(data.bearing);
  Serial.print(", ");
  Serial.print(data.light_intensity);
  Serial.print(", ");
  Serial.print(data.extern_temperature);
  Serial.print(", ");
  
  scp1000.readSensor();
  Serial.print(scp1000_get_altitude(scp1000.rawPressure >> 2));
  
  Serial.println();
}

void gpsdump(TinyGPS &gps)
{
  float flat, flon;
  unsigned long age;

  feedgps();

  gps.f_get_position(&flat, &flon, &age);
    
  data.gps.f_latitude = flat;
  data.gps.f_longitude = flon;
  data.gps.u_altitude = gps.altitude();
}
  
bool feedgps()
{
  while (nss->available())
  {
    if (gps.encode(nss->read()))
      return true;
  }
  return false;
}

void printFloat(HardwareSerial *hsp, double number, int digits)
{
  // Handle negative numbers
  if (number < 0.0)
  {
     hsp->print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  hsp->print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    hsp->print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    hsp->print(toPrint);
    remainder -= toPrint; 
  } 
}


