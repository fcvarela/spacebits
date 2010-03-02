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

#define SERIAL_PORT "/dev/cu.usbserial-FTOYMOEJ"
#define SERIAL_SPD 115200

pthread_t command_fifo_thread;
pthread_mutex_t serial_port_lock = PTHREAD_MUTEX_INITIALIZER;

int serial_port;

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
	if ((serial_port = setup_port()) == -1) {
		perror("setup_port");
		return 1;
	}

	printf("Will spawn IO loops\n");
	packet_loop();

	// detach command thread
	pthread_create(&command_fifo_thread, NULL, (void *)&command_loop, NULL);
	
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

void packet_loop(void) {
	spacebits_telemetry_t packet;
	char c;
	
	while (1) {
		pthread_mutex_lock(&serial_port_lock);
		size_t len = read(serial_port, &c, 1);
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
		printf("Got a packet\n");
		// push packet to telemetry topic
	}
}

void command_loop(void) {
	char hf[] = {'A','Z'};
	printf("Command loop started...\n");
	while (1) {
		// read_from_queue...
		sleep(1);
		continue;
		// got cmd? write to queue
		pthread_mutex_lock(&serial_port_lock);
		write(serial_port, &hf[0], 1);
		write(serial_port, (char)COMMAND_RESET, 1);
		write(serial_port, &hf[1], 1);
		pthread_mutex_unlock(&serial_port_lock);
	}
}
