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

// final packet
typedef struct {
	float imuAttitudes[3];
	float gpsTimeOfFix;
	float gpsLatLong[2];
	unsigned short gpsAltitude;
	unsigned char gpsValidFix;
	unsigned char gpsSatellites;
} packet_t;

packet_t packet;
int port;
struct timeval start,end;
char serialportname[] = "/dev/cu.usbserial-A7006REC";

void catch_quit(int signal) {
	close(port);
	printf("Closing serial port\n");
	exit(0);
}

int setupPort(char *port);
void parsePacket();
float timeval_subtract(struct timeval *x, struct timeval *y);

int main(int argc, char *argv[]) {
	char c;
	size_t len;

	// setup the serial port
	signal(SIGINT, catch_quit);
	port = setupPort(serialportname);
	if (port == -1) {
		perror("setupPort");
		return 1;
	}

	gettimeofday(&start,0);

	while (1) {
		len = read(port, &c, 1);
		if (c == 'A') {
			read(port, &packet, sizeof(packet_t));
			read(port, &c, 1);
			if (c == 'Z') {
				gettimeofday(&end, 0);
				parsePacket();
				gettimeofday(&start, 0);
			}
		}
	}

	close(port);
	return 0;
}

int setupPort(char *port) {
	int fd; 
	struct termios newtio; 

	fd = open(port, O_RDWR | O_NOCTTY);
	if (fd == -1)
		return(-1);

	tcgetattr(fd, &newtio);
	cfsetspeed(&newtio, 115200);

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

void parsePacket() {
	printf("Freq: %f\n", 1.0/timeval_subtract(&end, &start));
	printf("Roll: %f Pitch: %f Yaw: %f\n",
		packet.imuAttitudes[0]*57.2957795,packet.imuAttitudes[1]*57.2957795,packet.imuAttitudes[2]*57.2957795);
	printf("Lat: %f Lon: %f Alt: %d\n",
		packet.gpsLatLong[0], packet.gpsLatLong[1], packet.gpsAltitude);
	printf("TOF: %f Valid: %d, Satellites: %d\n",
		packet.gpsTimeOfFix, packet.gpsValidFix, packet.gpsSatellites);
}

float timeval_subtract(struct timeval *x, struct timeval *y) {
	double a,b,result;

	a = x->tv_sec + (x->tv_usec/1000000.0);
	b = y->tv_sec + (y->tv_usec/1000000.0);

	result = a-b;

	return((float)result);
}
