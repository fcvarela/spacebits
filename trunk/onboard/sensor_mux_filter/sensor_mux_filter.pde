#include <Wire.h>
#include <Kalman.h>
#include <IMU.h>
#include <NMEA.h>

#define IMU_SAMPLE_RATE 1.0/23.5 // 23.5 Hz
#define KALMAN_GYRO_TRUST 0.001 
#define KALMAN_ACCEL_TRUST 0.3
#define KALMAN_ERROR_SQUARED 0.003

// 8bit addr = 0xC0, 7bit addr = 0x60
#define I2C_COMPASS_ADDR 0x60

// KALMAN CONTROLLERS
Kalman kalman_roll(IMU_SAMPLE_RATE, KALMAN_ACCEL_TRUST, KALMAN_ERROR_SQUARED, KALMAN_GYRO_TRUST);
Kalman kalman_pitch(IMU_SAMPLE_RATE, KALMAN_ACCEL_TRUST, KALMAN_ERROR_SQUARED, KALMAN_GYRO_TRUST);
Kalman kalman_yaw(IMU_SAMPLE_RATE, KALMAN_ACCEL_TRUST, KALMAN_ERROR_SQUARED, KALMAN_GYRO_TRUST);

// IMU CONTROLLER
IMU imuHandler(&Serial1);
NMEA nmeaHandler(&Serial2);

// COMPASS AUX DATA
short compass_heading;

// final packet
typedef struct {
	char header;
	float imuAttitudes[3];
	float gpsTimeOfFix;
	float gpsLatLong[2];
	uint16_t gpsAltitude;
	uint8_t gpsValidFix;
	uint8_t gpsSatellites;
	char footer;
} packet_t;

packet_t telemetry_packet;

void setup() {
	Serial.begin(115200); // COMM
	Serial1.begin(57600); // IMU
	Serial2.begin(4800); // GPS
	Wire.begin(); // I2C

	telemetry_packet.header = 'A';
	telemetry_packet.footer = 'Z';
        
        // let the IMU power up
	delay(1000);

        // issue a start poll
	Serial.println("Starting IMU...");
	imuHandler.start();
        
	Serial.println("Calibration mode...");
	Serial.println("Set imu to flat position");
        
        // set nulls for flat position - not working
	while (!imuHandler.packetReady()) {}
	imuHandler.setNulls();
}

void loop() {
	processCompass();
	processGPS();
	processIMU();
}

void processIMU() {
	char line[256];
	if (!imuHandler.packetReady())
		return;
	
        // normalized g
	float g = sqrt(
		imuHandler.f_accel[0]*imuHandler.f_accel[0]+
		imuHandler.f_accel[1]*imuHandler.f_accel[1]+
		imuHandler.f_accel[2]*imuHandler.f_accel[2]);

	// chew packet
	kalman_roll.state_update(imuHandler.f_rate[0]);
	kalman_roll.kalman_update(-asin(imuHandler.f_accel[0]/g));

	kalman_pitch.state_update(imuHandler.f_rate[1]);
	kalman_pitch.kalman_update(asin(imuHandler.f_accel[1]/g));

	kalman_yaw.state_update(imuHandler.f_rate[2]);
	kalman_yaw.kalman_update(compass_heading);

	telemetry_packet.imuAttitudes[0] = kalman_roll._angle;
	telemetry_packet.imuAttitudes[1] = kalman_pitch._angle;
	telemetry_packet.imuAttitudes[2] = kalman_yaw._angle;

	// send packet and flash a led
	digitalWrite(13, HIGH);
	Serial.write((uint8_t *)&telemetry_packet, sizeof(packet_t));
	digitalWrite(13, LOW);
}

void processGPS() {
	if (!nmeaHandler.packetReady())
		return;
	
	// copy to local
	telemetry_packet.gpsTimeOfFix = nmeaHandler.f_timeOfFix;
	telemetry_packet.gpsLatLong[0] = nmeaHandler.f_latitude;
	telemetry_packet.gpsLatLong[1] = nmeaHandler.f_longitude;
	telemetry_packet.gpsValidFix = nmeaHandler.u_valid;
	telemetry_packet.gpsSatellites = nmeaHandler.u_satellites;
	telemetry_packet.gpsAltitude = nmeaHandler.u_altitude;
}

// READ COMPASS
void processCompass(void) {
        // how is this faster than a shift?
	if (Wire.available() == 2)
		compass_heading = Wire.receive() * 256 + Wire.receive();

	Wire.beginTransmission(I2C_COMPASS_ADDR);
	Wire.send(0x02); // request 2 bytes of data
	Wire.endTransmission();
	Wire.requestFrom(I2C_COMPASS_ADDR, 2);
}
