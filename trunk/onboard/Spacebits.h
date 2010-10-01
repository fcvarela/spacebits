// Spacebits 2010, Filipe Valpereiro
// http://softwarelivre.sapo.pt/projects/spacebits

/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef Spacebits_h
#define Spacebits_h 

#include "WProgram.h"

//
// IMU 5DOF
//
#define IMU_GX_PIN    0
#define IMU_GY_PIN    1
#define IMU_AX_PIN    4
#define IMU_AY_PIN    5
#define IMU_AZ_PIN    6

//
// Analog Sensors
//
#define VOLTAGE_SENSOR_PIN   8
#define CURRENT_SENSOR_PIN   9
#define HUMIDITY_SENSOR_PIN  10

//
// Barometric pressure and altitude sensor
//
#define SCP1000_CS_PIN    53

//
// uDrive logger
//
#define LOG_FILENAME    "sp.txt"

//
// Compass
//
#define CMPS03_ADDRESS    0x60

//
// Hardware initialization and support 
//
#define DEBUG_LED_PIN    13

#define LIGHT_AND_TEMP_VCC_PIN  33
#define LIGHT_AND_TEMP_GND_PIN  37

#define PARACHUTE_CUTTER        5

typedef struct
{
  uint16_t tm_hour;
  uint16_t tm_min;
  uint16_t tm_sec;
} rtc_data_t;

typedef struct 
{
  int16_t gx;
  int16_t gy;
  int16_t ax;
  int16_t ay;
  int16_t az;
} imu_data_t;

typedef struct
{
  int16_t pressure;
  int16_t temperature;
} scp1000_data_t;

typedef struct 
{
    double f_latitude;
    double f_longitude;
    uint16_t u_altitude;
    uint8_t u_valid;
    uint8_t u_satellites;
} gps_info_t;

typedef struct 
{
  char              start_mark;
  rtc_data_t        rtc;
  imu_data_t        imu;
  scp1000_data_t    scp;
  gps_info_t        gps;
  
  uint16_t          voltage;
  uint16_t          current;
  uint16_t          humidity;
  uint16_t          bearing;  
  uint16_t          light_intensity;
  int16_t	    extern_temperature;
  char              end_mark;
} Spacebits_t;

#endif // Spacebits_h

