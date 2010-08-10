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

#include "telemetry.h"
#include "/Volumes/SVNRepo/broker/trunk/clients/c-component/libsapo-broker2/src/sapo-broker2.h"

#define SERIAL_PORT "/dev/cu.usbserial-A800ekp4"
#define SERIAL_SPD 19200

broker_server_t *broker_server;
sapo_broker_t *broker;
int serial_port;

void packet_to_xml(sensor_data_t *packet, char *xml);
size_t myread(int fd, void *ptr, size_t bytes);

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
	/*if ((serial_port = setup_port()) == -1) {
		perror("setup_port");
		return 1;
	}*/
	
	// init broker connection
	char hostname[] = "127.0.0.1";
	broker_server = malloc(sizeof(broker_server_t));
	broker_server->hostname = strdup(hostname);
	broker_server->port = 3323;
	broker_server->transport = 0;
	broker_server->protocol = 1;
	broker = broker_init(*broker_server);

	printf("Will spawn IO loops\n");
	packet_loop();

	return 0;
}

int setup_port(void) {
    int fd;
    struct termios newtio;

    fd = open(SERIAL_PORT, O_RDWR | O_NOCTTY);
    if (fd == -1)
        return fd;

    tcgetattr(fd, &newtio);
    cfsetspeed(&newtio, SERIAL_SPD);

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
        /*read(serial_port, &c, 1);
        if (c != 'A')
            continue;
            
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
        myread(serial_port, &packet.scp.raw_pressure, 2);
        myread(serial_port, &packet.scp.raw_temperature, 2);
        
        // GPS
        myread(serial_port, &packet.gps.f_latitude, 4);
        myread(serial_port, &packet.gps.f_longitude, 4);
        myread(serial_port, &packet.gps.u_altitude, 2);
        myread(serial_port, &packet.gps.u_valid, 1);
        myread(serial_port, &packet.gps.u_satellites, 1);
        
        myread(serial_port, &packet.voltage, 2);
        myread(serial_port, &packet.current, 2);
        myread(serial_port, &packet.humidity, 2);
        myread(serial_port, &packet.bearing, 2);
        myread(serial_port, &packet.light, 2);
        myread(serial_port, &packet.extern_temp, 2);
 
        myread(serial_port, &c, 1);
        if (c != 'Z')
            continue;
        */
		packet.gps.f_latitude = 37.761933;
		packet.gps.f_longitude = -8.091904;
		sleep(1);
        // push packet to telemetry topic
        char *xml = (char *)malloc(10000);
        packet_to_xml(&packet, xml);
        unsigned int retval = broker_publish(broker, TELEMETRY_TOPIC, xml, strlen(xml));
        free(xml);
        fprintf(stderr, "Published packet: %u\n", retval);
    }
}

void packet_to_xml(sensor_data_t *packet, char *xml) {
    char fmt[] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><balloon><id>2</id><token>29v856792b29##/++9</token><power><current>%u</current><voltage>%u</voltage></power><atmosphere><pressure>%hd</pressure><temp>%hd</temp><temp_ext>%hd</temp_ext><light>%u</light><humidity>%u</humidity></atmosphere><rtc>%u:%u:%u</rtc><geo><lat>%.5f</lat><lon>%.5f</lon><alt>%u</alt><bear>%u</bear></geo><imu><gx>%hd</gx><gy>%hd</gy><ax>%hd</ax><ay>%hd</ay><az>%hd</az></imu></balloon>";
    sprintf(xml, fmt,
        packet->current, packet->voltage,
        packet->scp.raw_pressure, packet->scp.raw_temperature,
        packet->extern_temp, packet->light, packet->humidity,
        packet->rtc.tm_hour, packet->rtc.tm_min, packet->rtc.tm_sec,
        packet->gps.f_latitude, packet->gps.f_longitude, packet->gps.u_altitude, packet->bearing,
        packet->imu.gx, packet->imu.gy, packet->imu.ax, packet->imu.ay, packet->imu.az);
    fprintf(stderr, "Time is: %d:%d:%d\n", packet->rtc.tm_hour, packet->rtc.tm_min, packet->rtc.tm_sec);
    fprintf(stderr, "XML msg:\n---\n%s\n---\n", xml);
}
