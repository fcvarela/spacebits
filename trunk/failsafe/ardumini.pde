#include <NewSoftSerial.h>
#include <NMEANewSoftSerial.h>

// nmea handler
NewSoftSerial nmea(3, 2);
NMEANewSoftSerial nmeaHandler(&nmea);

void setup() {
    // init GSM module
    Serial.begin(115200);
    nmea.begin(4800); // GPS
        
    delay(1000 * 3);
    Serial.flush();
    Serial.write("ATZ");
    Serial.write(13);
    waitForOK();

    Serial.write("AT");
    Serial.write(13);
    waitForOK();
    
    Serial.write("AT+CMGF=1");
    Serial.write(13);
    waitForOK();
    
    delay(1000 * 10);
}

void loop() {
    if (!nmeaHandler.packetReady())
        return;
    
    // check signal strength
    sendSMS();
}

void sendSMS() {
    Serial.write("AT");
    Serial.write(13);
    waitForOK();
    Serial.write("AT+CMGS=");
    Serial.write(34);
    Serial.write("964250307");
    Serial.write(34);
    Serial.write(13);
    while (Serial.available() < 5);
    while (Serial.available())
        Serial.read();

    Serial.write("http://maps.google.com/maps?q=");
    Serial.print(nmeaHandler.f_latitude, 4);
    Serial.write(",");
    Serial.print(nmeaHandler.f_longitude, 4);
    Serial.write(13);
    Serial.write("versao 1");
    Serial.write(13);
    Serial.write(26);
    
    waitForOK();
    delay(1000 * 3);

}

void waitForOK() {
    while (Serial.available() < 6);
    while (Serial.available())
        Serial.read();
}
