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
#include <pthread.h>
#include <curl/curl.h>

#include "telemetry.h"

// prototypes
void catch_quit(int signal);
void packet_loop(void);
int setup_port(const char *serial_device, uint32_t port_baud);
size_t myread(int fd, void *ptr, size_t bytes);
uint64_t make_timestamp(void);
void packet_to_xml(sensor_data_t *packet, char *xml);
int broadcast_packet(const char *xml, size_t xml_len);
    
int serial_port;
uint8_t balloon_id = 0;

void catch_quit(int signal) {
	close(serial_port);
	printf("Closing serial port\n");
	exit(0);
}

int main(int argc, char **argv) {
	// install quit handler
	signal(SIGINT, catch_quit);
	signal(SIGQUIT, catch_quit);
	
	#ifndef TESTMODE
	if (argc < 4) {
        fprintf(stdout, "Usage: %s /dev/your_serial_device PORT_BAUD BALLOON_ID\n", argv[0]);
        return 1;
	}
	
	// init serial port
	if ((serial_port = setup_port(argv[1], atoi(argv[2]))) == -1) {
		perror("setup_port");
		return 1;
	}
    balloon_id = atoi(argv[3]);
    #else
    balloon_id = 6;
    #endif
	printf("Will spawn IO loops\n");
	packet_loop();

	return 0;
}

int setup_port(const char *serial_device, uint32_t port_baud) {
    int fd;
    struct termios newtio;

    fd = open(serial_device, O_RDWR | O_NOCTTY);
    if (fd == -1)
        return fd;

    tcgetattr(fd, &newtio);
    cfsetspeed(&newtio, port_baud);

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
		memset(&packet, '\0', sizeof(sensor_data_t));
		#ifndef TESTMODE
        read(serial_port, &c, 1);
        if (c != 'A')
            continue;
        
        fprintf(stderr, "GOT START\n");
        
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
        if (c != 'Z')
            continue;
        #endif
        
        // push packet to telemetry topic
        char *xml = (char *)calloc(10000, sizeof(char));
        packet_to_xml(&packet, xml);
        int retval = broadcast_packet(xml, strlen(xml));
        free(xml);
        fprintf(stderr, "Published packet: %u\n", retval);
        sleep(1);
    }
}

int broadcast_packet(const char *xml, size_t xml_len) {
    CURL *curl;
    CURLcode res;
    
    curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, "http://spacebits.eu/api/put");
    //curl_easy_setopt (curl, CURLOPT_HTTPHEADER, array("Content-Type: text/xml"));
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, xml);
    res = curl_easy_perform(curl);
    if (res != 0) {
        perror("curl_easy_perform");
        curl_easy_cleanup(curl);
        return 1;
    }
    curl_easy_cleanup(curl);
    return 0;
}

void packet_to_xml(sensor_data_t *packet, char *xml) {
    // dummy payload
    #ifdef TESTMODE
    struct timeval tv;
    time_t curtime;
    gettimeofday(&tv, NULL);
    curtime=tv.tv_sec;
    char t_hour[3], t_min[3], t_sec[3];
    strftime(t_hour, 3,"%H", localtime(&curtime));
    strftime(t_min, 3,"%M", localtime(&curtime));
    strftime(t_sec, 3,"%S", localtime(&curtime));
    packet->rtc.tm_hour = atoi(t_hour);
    packet->rtc.tm_min = atoi(t_min);
    packet->rtc.tm_sec = atoi(t_sec);
    packet->rtc.tm_hour = atoi(t_hour);
    packet->rtc.tm_min = atoi(t_min);
    packet->rtc.tm_sec = atoi(t_sec);

    packet->imu.gx = packet->imu.gy = packet->imu.ax = packet->imu.ay = packet->imu.az = 512;
    
    packet->bmp.raw_pressure = 10;
    packet->bmp.raw_temperature = 2500;
    
    packet->gps.f_latitude = 38.10;
    packet->gps.f_longitude = -6.7;
    packet->gps.u_altitude = 25000;
    packet->gps.u_satellites = 7;
    
    packet->internal_temp = 2500;
    packet->extern_temp = -5000;
    packet->humidity = 40;
	#endif
    
    char fmt[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<balloon>\n<id>%u</id>\n<token>29v856792b29##/++9</token>\n<atmosphere>\n<pressure>%hd</pressure>\n<temp>%hd</temp>\n<temp_int>%hd</temp_int>\n<temp_ext>%hd</temp_ext>\n<light>%u</light>\n<humidity>%u</humidity>\n</atmosphere>\n<rtc>%u:%u:%u</rtc>\n<geo>\n<lat>%.5f</lat>\n<lon>%.5f</lon>\n<alt>%u</alt>\n<bear>%u</bear>\n</geo>\n<imu>\n<gx>%hd</gx>\n<gy>%hd</gy>\n<ax>%hd</ax>\n<ay>%hd</ay>\n<az>%hd</az>\n</imu>\n<gsm>\n<registered>%u</registered>\n<ready>%u</ready>\n</gsm>\n</balloon>";
    sprintf(xml, fmt,
        balloon_id,
        packet->bmp.raw_pressure, packet->bmp.raw_temperature, packet->internal_temp,
        packet->extern_temp, packet->light, packet->humidity,
        packet->rtc.tm_hour, packet->rtc.tm_min, packet->rtc.tm_sec,
        packet->gps.f_latitude, packet->gps.f_longitude, packet->gps.u_altitude, packet->bearing,
        packet->imu.gx, packet->imu.gy, packet->imu.ax, packet->imu.ay, packet->imu.az,
        packet->gsm_registered, packet->gsm_ready);
    fprintf(stderr, "Time is: %d:%d:%d\n", packet->rtc.tm_hour, packet->rtc.tm_min, packet->rtc.tm_sec);
    fprintf(stderr, "XML msg:\n---\n%s\n---\n", xml);
}
