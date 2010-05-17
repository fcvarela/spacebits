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
#include "sapo_broker.h"

#define SERIAL_PORT "/dev/cu.usbserial-A6007x6A"
#define SERIAL_SPD 115200

pthread_t command_thread;
pthread_mutex_t serial_port_lock = PTHREAD_MUTEX_INITIALIZER;

int serial_port;
broker_server_t *broker_server;
sapo_broker_t *broker;

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
	pthread_create(&command_thread, NULL, (void *)&command_loop, NULL);
	
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
		// push packet to telemetry topic
		char msg[1024];
		char fmt[] = "<balloon><gps><lat>%f</lat><lon>%f</lon><altitude>%hd</altitude></gps><sensors><sensor idx=\"0\">%hd</sensor><sensor idx=\"1\">%hd</sensor><sensor idx=\"2\">%hd</sensor><sensor idx=\"3\">%hd</sensor><sensor idx=\"4\">%hd</sensor><sensor idx=\"5\">%hd</sensor><sensor idx=\"6\">%hd</sensor><sensor idx=\"7\">%hd</sensor><sensor idx=\"8\">%hd</sensor><sensor idx=\"9\">%hd</sensor><sensor idx=\"10\">%hd</sensor><sensor idx=\"11\">%hd</sensor><sensor idx=\"12\">%hd</sensor><sensor idx=\"13\">%hd</sensor><sensor idx=\"14\">%hd</sensor><sensor idx=\"15\">%hd</sensor></balloon>\n";
		sprintf(msg, fmt, packet.latitude, packet.longitude, packet.gps_altitude,
			packet.channel00, packet.channel01, packet.channel02, packet.channel03,
			packet.channel04, packet.channel05, packet.channel06, packet.channel07,
			packet.channel08, packet.channel09, packet.channel10, packet.channel11,
			packet.channel12, packet.channel13, packet.channel14, packet.channel15);
			
		unsigned int retval = broker_publish(broker, TELEMETRY_TOPIC, msg, strlen(msg));
		printf("Published packet: %u\n", retval);
	}
}

void command_loop(void) {
	char hf[] = {'A','Z'};
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
