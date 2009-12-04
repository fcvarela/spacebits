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

#ifndef NMEA_h
#define NMEA_h

#include "WProgram.h"
#include <HardwareSerial.h>

class NMEA {
	public:
		NMEA(HardwareSerial *serialPort);
		bool packetReady();
		double f_timeOfFix;
		double f_latitude;
		double f_longitude;
		uint16_t u_altitude;
		uint8_t u_valid;
		uint8_t u_satellites;
	private:
		HardwareSerial *_serialPort;
		char _NMEABuffer[90];
		void parsePacket();
};

#endif
