#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h> 
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sys/select.h>
#include <signal.h>
#include <unistd.h>
#ifndef TESTMODE
#include <jni.h>
#endif

#include "../telemetry-daemon/telemetry.h"

int serial_port;
uint8_t balloon_id = 6;

void packet_loop(void);
uint64_t make_timestamp(void);
int setup_port(const char *serial_device);
void catch_quit(int signal);

void catch_quit(int signal) {
    close(serial_port);
    printf("Closing serial port\n");
    exit(0);
}

int main(int argc, char **argv) {
    // install quit handler
    signal(SIGINT, catch_quit);
    signal(SIGQUIT, catch_quit);

    // init serial port
    #ifndef TESTMODE
    if ((serial_port = setup_port("/dev/ttyMSM2")) == -1) {
        perror("setup_port");
        return 1;
    }
    #endif

    printf("Will spawn IO loops\n");
    packet_loop();

    return 0;
}

int setup_port(const char *serial_device) {
    int fd;
    struct termios newtio;

    fd = open(serial_device, O_RDWR | O_NOCTTY);
    if (fd == -1)
        return fd;

    tcgetattr(fd, &newtio);
    speed_t speed = B19200;
    cfsetispeed(&newtio, speed);
    cfsetospeed(&newtio, speed);

    newtio.c_cflag |= (CLOCAL | CREAD);
    newtio.c_cflag &= ~PARENB;
    newtio.c_cflag &= ~CSTOPB;
    newtio.c_cflag &= ~CSIZE;
    newtio.c_cflag |= CS8;
    newtio.c_iflag |= (IGNPAR | IGNBRK);
    newtio.c_oflag = 0;
    newtio.c_iflag &= ~(IXON | IXOFF | IXANY);
    newtio.c_lflag = 0;
    tcflush(fd, TCIOFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    return(fd);
}

size_t myread(int fd, void *ptr, size_t bytes) {
    size_t len = 0;
    size_t temp;

    do {
        temp = read(fd, ptr, bytes-len);
        if (temp > 0)
            len += temp;
    } while (len < bytes);

    return len;
}

void packet_loop(void) {
    sensor_data_t packet;
    char c, filename[128], msg[160];
    struct timeval tv;
    char t_hour[3], t_min[3], t_sec[3];
    uint64_t timestamp;
    uint8_t groundstation_id;

    while (1) {
        sleep(1);
        memset(&packet, '\0', sizeof(sensor_data_t));
        
        #ifdef TESTMODE
        gettimeofday(&tv, NULL);;
        strftime(t_hour, 3,"%H", localtime(&tv.tv_sec));
        strftime(t_min, 3,"%M", localtime(&tv.tv_sec));
        strftime(t_sec, 3,"%S", localtime(&tv.tv_sec));
        packet.rtc.tm_hour = atoi(t_hour);
        packet.rtc.tm_min = atoi(t_min);
        packet.rtc.tm_sec = atoi(t_sec);
    
        packet.imu.gx = packet.imu.gy = packet.imu.ax = packet.imu.ay = packet.imu.az = 512;
        
        packet.bmp.raw_pressure = 10;
        packet.bmp.raw_temperature = 512;
        
        packet.gps.f_latitude = 38.10;
        packet.gps.f_longitude = -6.7;
        packet.gps.u_altitude = 25000;
        packet.gps.u_satellites = 7;
        
        packet.internal_temp = 2500;
        packet.extern_temp = -5000;
        packet.humidity = 40;
        
        #else
        read(serial_port, &c, 1);
        if (c != 'A')
            continue;

        printf("GOT START\n");

        // RTC
        myread(serial_port, &packet.rtc.tm_hour, 2);
        myread(serial_port, &packet.rtc.tm_min, 2);
        myread(serial_port, &packet.rtc.tm_sec, 2);

        // IMU
        myread(serial_port, &packet.imu.gx, 2);
        myread(serial_port, &packet.imu.gy, 2);
        myread(serial_port, &packet.imu.ax, 2);
        myread(serial_port, &packet.imu.ay, 2);
        myread(serial_port, &packet.imu.az, 2);

        // SCP
        myread(serial_port, &packet.bmp.raw_pressure, 4);
        myread(serial_port, &packet.bmp.raw_temperature, 2);

        // GPS
        myread(serial_port, &packet.gps.f_latitude, 4);
        myread(serial_port, &packet.gps.f_longitude, 4);
        myread(serial_port, &packet.gps.u_altitude, 2);
        myread(serial_port, &packet.gps.u_valid, 1);
        myread(serial_port, &packet.gps.u_satellites, 1);

        myread(serial_port, &packet.humidity, 2);
        myread(serial_port, &packet.bearing, 2);
        myread(serial_port, &packet.light, 2);
        myread(serial_port, &packet.internal_temp, 2);
        myread(serial_port, &packet.extern_temp, 2);

        myread(serial_port, &packet.gsm_registered, 1);
        myread(serial_port, &packet.gsm_ready, 1);
        
        // 46 bytes

        myread(serial_port, &c, 1);
        if (c != 'Z') {
            printf("malformed packet\n");
            continue;
        }
        #endif

        // get a timestamp for our filename
        timestamp = make_timestamp();

        // make filename
        #ifndef TESTMODE
        sprintf(filename, "/sdcard/spacebits/new/%llu.txt", timestamp);
        #else
        sprintf(filename, "%llu.txt", timestamp);
        #endif

        // make groundstation id
        groundstation_id = 0;
#ifdef GROUNDSTATIONID
        groundstation_id = GROUNDSTATIONID;
#endif
        
        //R,ground_stations_id,ballon_id,lat,lon,alt,nstats,pressure,int_temp,ext_temp,humidity
        sprintf(msg, "%s R,%u,%u,%f,%f,%u,%u,%d,%hd,%hd,%u", "+351968252265", 
                groundstation_id, balloon_id,
                packet.gps.f_latitude, packet.gps.f_longitude, packet.gps.u_altitude, packet.gps.u_satellites,
                packet.bmp.raw_pressure, packet.internal_temp, packet.extern_temp, packet.humidity);

        // open file for writing
        FILE *sendsms;
        if ((sendsms = fopen(filename, "wt")) == NULL) {
            perror("could not open new sms file for writing");
            // act all christ here. we're _sure_ someOne will respawn us
            return;
        }

        // write message to file
        fprintf(sendsms, "%s\n", msg);

        // close, debug
        fclose(sendsms);
        printf("Wrote packet to file\n");
    }
}

uint64_t make_timestamp(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec;
}
