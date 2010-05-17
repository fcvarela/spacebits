#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h> 
#include <sys/stat.h> 
#include <sys/types.h>
#include <sys/time.h>
#include <sys/select.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>

#include "telemetry.h"
#include "libsapo-broker2.h"

#define SERIAL_PORT "/dev/cu.usbserial-A6007x6A"
#define SERIAL_SPD 115200

#define DEBUG

#ifndef DEBUG
pthread_t command_thread;
pthread_mutex_t serial_port_lock = PTHREAD_MUTEX_INITIALIZER;
int serial_port;
#endif

broker_server_t *broker_server;
sapo_broker_t *broker;

void catch_quit(int signal) {
	#ifndef DEBUG
	close(serial_port);
	#endif
	
	printf("Closing serial port\n");
	exit(0);
}

int main(int argc, char **argv) {
	// install quit handler
	signal(SIGINT, catch_quit);
	signal(SIGQUIT, catch_quit);
	
	#ifndef DEBUG
	// init serial port
	if ((serial_port = setup_port()) == -1) {
		perror("setup_port");
		return 1;
	}
	#endif
	
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

	// detach command thread
	#ifndef DEBUG
	pthread_create(&command_thread, NULL, (void *)&command_loop, NULL);
	#endif
	
	return 0;
}

#ifndef DEBUG
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
#endif

void packet_loop(void) {
	sensor_data_t packet;
	#ifndef DEBUG
	char c;
	#endif
	
	while (1) {
		#ifndef DEBUG
		    pthread_mutex_lock(&serial_port_lock);
		    read(serial_port, &c, 1);
		    if (c != 'A') {
			    pthread_mutex_unlock(&serial_port_lock);
			    continue;
		    }
		    
		    read(serial_port, &packet, sizeof(packet));
    		read(serial_port, &c, 1);
    		if (c != 'Z') {
    			pthread_mutex_unlock(&serial_port_lock);
    			continue;
    		}

    		pthread_mutex_unlock(&serial_port_lock);
		#else
		    packet.voltage = 1024;
            packet.current = 1024;
            packet.bearing = 1024;
            packet.humidity = 1024;
            packet.dust_density = 1024;
            packet.rtc.tm_hour = 12;
            packet.rtc.tm_min = 30;
            packet.rtc.tm_sec = 59;
            packet.imu.gx = 1024;
            packet.imu.gy = 1024;
            packet.imu.ax = 1024;
            packet.imu.ay = 1024;
            packet.imu.az = 1024;
            packet.scp.raw_pressure = 2048;
            packet.scp.raw_temperature = 2048;
            sleep(1); //simulate a little locking delay (locking read on serial port)
        #endif

		// push packet to telemetry topic
		unsigned int retval = broker_publish(broker, TELEMETRY_TOPIC, (char *)&packet, sizeof(sensor_data_t));
		fprintf(stderr, "Published packet: %u\n", retval);
	}
}

void command_loop(void) {
	printf("Command loop started...\n");
	while (1) {
		// read from command queue...
		sleep(1);
		continue;
		// got cmd? write to queue
		/*
		pthread_mutex_lock(&serial_port_lock);
		write(serial_port, &hf[0], 1);
		write(serial_port, (char)COMMAND_RESET, 1);
		write(serial_port, &hf[1], 1);
		pthread_mutex_unlock(&serial_port_lock);
		*/
	}
}
