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
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <jni.h>

#include "telemetry.h"

int serial_port;
uint8_t balloon_id = 6;

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
    if ((serial_port = setup_port("/dev/ttyMSM2")) == -1) {
        perror("setup_port");
        return 1;
    }

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
    speed_t speed = B9600;
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
    char c;

    while (1) {
        sleep(1);
        memset(&packet, '\0', sizeof(sensor_data_t));
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

        myread(serial_port, &c, 1);
        if (c != 'Z') {
            printf("malformed packet\n");
            continue;
        }

        // save sms request to disk
        struct timeval tv;
        time_t curtime;
        gettimeofday(&tv, NULL);
        curtime=tv.tv_sec;
        char msg[160];
        char filename[128];
        sprintf(filename, "/sdcard/spacebits/new/%llu.txt", tv.tv_sec);
        sprintf(msg, "%u,%f,%f,%u,%u", balloon_id, packet.gps.f_latitude, packet.gps.f_longitude, packet.gps.u_altitude, packet.gps.u_satellites);
        
        FILE *sendsms;
        sendsms = fopen(filename, "wt");
        if (sendsms == NULL) {
            perror("could not open new sms file for writing");
            continue;
        }
        fprintf(sendsms, "%s\n", msg);
        fclose(sendsms);
        printf("Wrote packet to file\n");
    }
}
