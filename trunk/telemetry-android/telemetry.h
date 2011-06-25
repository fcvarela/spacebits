#include <stdint.h>

#define PACKET_HEADER 'A'

typedef struct {
    uint16_t tm_hour, tm_min, tm_sec;
} rtc_data_t;

typedef struct {
    int16_t gx, gy, ax, ay, az;
} imu_data_t;

typedef struct {
	int32_t raw_pressure;
	int16_t raw_temperature;
} bmp085_data_t;

typedef struct {
    float f_latitude;
    float f_longitude;
    uint16_t u_altitude;
    uint8_t u_valid;
    uint8_t u_satellites;
} gps_info_t;

typedef struct {
	rtc_data_t rtc;
	imu_data_t imu;
	bmp085_data_t bmp;
	gps_info_t gps;
	uint16_t humidity, bearing, light, internal_temp, extern_temp;
	uint8_t gsm_registered, gsm_ready;
} sensor_data_t;

#define PACKET_FOOTER 'Z'

// prototypes
void catch_quit(int signal);
void packet_loop(void);
int setup_port(const char *serial_device);
size_t myread(int fd, void *ptr, size_t bytes);
uint64_t make_timestamp(void);