#include <stdint.h>

#define PACKET_HEADER 'A'

typedef struct {
    uint8_t tm_hour, tm_min, tm_sec;
} rtc_data_t;

typedef struct {
    uint16_t gx, gy, ax, ay, az;
} imu_data_t;

typedef struct {
 uint32_t raw_pressure, raw_temperature;
} scp1000_data_t;

typedef struct {
 rtc_data_t rtc;
 imu_data_t imu;
 scp1000_data_t scp;
 uint16_t voltage, current, bearing, humidity, dust_density;
} sensor_data_t;

#define PACKET_FOOTER 'Z'

#define COMMAND_QUEUE "/spacebits/control"
#define TELEMETRY_TOPIC "/spacebits/telemetry"

// prototypes
void catch_quit(int signal);
int setup_port(void);
void packet_loop(void);
void command_loop(void);

