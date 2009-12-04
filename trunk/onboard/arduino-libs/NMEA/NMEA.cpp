/*
 * (c) 2005-9 Filipe Varela
 *
 * NMEA is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * NMEA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with NMEA; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "WProgram.h"
#include "NMEA.h"

NMEA::NMEA(HardwareSerial *serialPort) {
	_serialPort = serialPort;
	memset(_NMEABuffer, 0, sizeof(_NMEABuffer));
}

bool NMEA::packetReady() {
	char c;
	while (_serialPort->available() > 0) {
		c = _serialPort->read();
		if (c != '\r' && c != '\n')
			_NMEABuffer[strlen(_NMEABuffer)] = c;
		else {
			parsePacket();
			memset(_NMEABuffer, 0, sizeof(_NMEABuffer));
			return true;
		}
	}
	return false;
}

void NMEA::parsePacket() {
	uint8_t i;
	char *endptr;
	double degrees, minutesfrac;
	
	// GPGGA?
	if (strncmp(_NMEABuffer, "$GPGGA", 6) == 0) {
		i = 7;
		// get UTC time [hhmmss.sss]
		f_timeOfFix = strtod(&_NMEABuffer[i], &endptr);
		while (_NMEABuffer[i++] != ',');
		
		// get latitude [ddmm.mmmmm]
		f_latitude = strtod(&_NMEABuffer[i], &endptr);
		minutesfrac = modf(f_latitude/100, &degrees);
		f_latitude = degrees + (minutesfrac*100)/60;
		while (_NMEABuffer[i++] != ',');
		if (_NMEABuffer[i] == 'S') f_latitude = -f_latitude;
		while (_NMEABuffer[i++] != ',');
		
		// get longitude [ddmm.mmmmm]
		f_longitude = strtod(&_NMEABuffer[i], &endptr);
		minutesfrac = modf(f_longitude/100, &degrees);
		f_longitude = degrees + (minutesfrac*100)/60;
		while (_NMEABuffer[i++] != ',');
		if (_NMEABuffer[i] == 'W') f_longitude = -f_longitude;
		while (_NMEABuffer[i++] != ',');
		
		// position fix status: 0 = Invalid, 1 = Valid SPS, 2 = Valid DGPS, 3 = Valid PPS
		if ((_NMEABuffer[i] != '0') && (_NMEABuffer[i] != ',')) u_valid = 1;
		else u_valid = 0;
		while (_NMEABuffer[i++] != ',');
		
		// get number of satellites used in GPS solution
		u_satellites = atoi(&_NMEABuffer[i]);
		while (_NMEABuffer[i++] != ',');
		
		// HDOP (horizontal dilution of precision)
		while (_NMEABuffer[i++] != ',');
		
		// get altitude (in meters)
		u_altitude = strtod(&_NMEABuffer[i], &endptr);
		
		while(_NMEABuffer[i++] != ',');              // next field: altitude units, always 'M'
		while(_NMEABuffer[i++] != ',');              // next field: geoid seperation
		while(_NMEABuffer[i++] != ',');              // next field: seperation units
		while(_NMEABuffer[i++] != ',');              // next field: DGPS age
		while(_NMEABuffer[i++] != ',');              // next field: DGPS station ID
		while(_NMEABuffer[i++] != '*');              // next field: checksum
	}
}
