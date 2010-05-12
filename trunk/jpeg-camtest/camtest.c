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
#include <stdint.h>

#define SERIAL_PORT "/dev/cu.usbserial-A6007wWi"
#define SERIAL_SPD 115200

// protos
void catch_quit(int signal);
int setup_port(void);
void packet_loop(void);

// global
int serial_port;

// signal handler (graceful port close)
void catch_quit(int signal) {
	close(serial_port);
	printf("Closing serial port\n");
	exit(0);
}

// duh
int main(int argc, char **argv) {
	// install quit handler
	signal(SIGINT, catch_quit);
	
	// init serial port
	if ((serial_port = setup_port()) == -1) {
		fprintf(stderr, "Could not open port %s\n", SERIAL_PORT);
		return 1;
	}
	
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

void packet_loop(void) {
	int32_t marker;
	uint16_t img_len=0, total_read=0;
	uint8_t *img_data;
	uint32_t img_counter = 0;
	char img_filename[1024];
	FILE *output;
	
	while (1) {
		size_t len = read(serial_port, &marker, sizeof(marker));
		if (marker != 0xFFFFFFFF) continue;
		len = read(serial_port, &img_len, sizeof(img_len));
		printf("Got a marker. Picture Size is :%u\n", img_len);
		img_data = (uint8_t *)malloc(img_len+58);

		// read cyle packet by packet
		while (total_read < img_len) {
			len = read(serial_port, img_data+total_read, img_len-total_read);
			total_read += len;
		}
		printf("Finished reading. Will save %u bytes\n", total_read);
		// dump to tmp file
		sprintf(img_filename, "image_%u.jpg", img_counter++);
		if ((output = fopen(img_filename, "w")) == NULL) {
			perror("phodass - fopen");
			continue;
		}
		len = fwrite(img_data+58, img_len, 1, output);
		fclose(output);
		free(img_data);
		printf("Done saving...\n");
		total_read = 0;
	}
}
