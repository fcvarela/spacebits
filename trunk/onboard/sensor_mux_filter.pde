#include <Wire.h>
#include <Kalman.h>
#include <IMU.h>
#include <NMEA.h>

#define IMU_SAMPLE_RATE 1.0/23.5 // 23.5 Hz

// KALMAN CONTROLLERS
Kalman kalman_roll(IMU_SAMPLE_RATE, 0.3, 0.003, 0.001);
Kalman kalman_pitch(IMU_SAMPLE_RATE, 0.3, 0.003, 0.001);
Kalman kalman_yaw(IMU_SAMPLE_RATE, 0.3, 0.003, 0.001);

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

	delay(1000);

	Serial.println("Starting IMU...");
	imuHandler.start();

	Serial.println("Calibration mode...");
	Serial.println("Set imu to flat position");

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

	// send packet
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
	if (Wire.available() == 2)
		compass_heading = Wire.receive() * 256 + Wire.receive();

	Wire.beginTransmission(0x60);
	Wire.send(0x02);
	Wire.endTransmission();
	// 8bit addr = 0xC0, 7bit addr = 0x60
	Wire.requestFrom(0x60, 2);
}
