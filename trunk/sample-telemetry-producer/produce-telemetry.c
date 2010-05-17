#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint8_t tm_hour, tm_min, tm_sec;
} rtc_data_t;

typedef struct {
    uint16_t gx, gy, ax, ay, az;
} imu_data_t;

typedef struct {
 uint32_t raw_pressure, raw_temperature;
} scp1000_data_t;

typedef struct 
{
 rtc_data_t        rtc;
 imu_data_t        imu;
 scp1000_data_t    scp;

 uint16_t          voltage;
 uint16_t          current;
 uint16_t          bearing;
 uint16_t          humidity;
 uint16_t          dust_density;
} sensor_data_t;

int dump_data(sensor_data_t *data);

// call me with program > dump.txt. im dumping to stdout :)
int main(int argc, char **argv) {
    sensor_data_t data;
    char head_foot[2] = {'A','Z'};
    
    fprintf(stderr, "Starting...\n");
    fprintf(stderr, "Data struct has %ld bytes\n", sizeof(sensor_data_t));
    data.voltage = 1024;
    data.current = 1024;
    data.bearing = 1024;
    data.humidity = 1024;
    data.dust_density = 1024;
    data.rtc.tm_hour = 12;
    data.rtc.tm_min = 30;
    data.rtc.tm_sec = 59;
    data.imu.gx = 1024;
    data.imu.gy = 1024;
    data.imu.ax = 1024;
    data.imu.ay = 1024;
    data.imu.az = 1024;
    data.scp.raw_pressure = 2048;
    data.scp.raw_temperature = 2048;
    
    fwrite(&head_foot[0], 1, 1, stdout);
    fwrite(&data, sizeof(sensor_data_t), 1, stdout);
    fwrite(&head_foot[1], 1, 1, stdout);
    
    fprintf(stderr, "Dumping sample packet...\n---\n");
    dump_data(&data);
    fprintf(stderr, "---\nDump complete...\n");
    
    return 0;
}

int dump_data(sensor_data_t *data) {
    fprintf(stderr, "Main container:\n");
    
    fprintf(stderr, "Voltage: %u Current: %u Bearing: %u Humidity: %u DustDensity: %u\n\n",
        data->voltage, data->current, data->bearing, data->humidity, data->dust_density);
    
    fprintf(stderr, "RTC:\n");
    fprintf(stderr, "Hour: %u Min: %u Sec: %u\n\n",
        data->rtc.tm_hour, data->rtc.tm_min, data->rtc.tm_sec);
        
    fprintf(stderr, "IMU:\n");
    fprintf(stderr, "GX: %u GY: %u AX: %u AY: %u AZ: %u\n",
        data->imu.gx, data->imu.gy, data->imu.ax, data->imu.gy, data->imu.az);
    
    fprintf(stderr, "SCP:\n");
    fprintf(stderr, "Raw temp: %u Raw press: %u\n",
        data->scp.raw_temperature, data->scp.raw_pressure);
}
