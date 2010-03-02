#include <stdint.h>

// axis names
enum axisnames {AXIS_X, AXIS_Y, AXIS_Z};

// commands
enum commands {
	COMMAND_RESET,
	COMMAND_TAKE_PICTURE,
	COMMAND_RECORD_START,
	COMMAND_RECORD_STOP
};

// the telemetry packet: head/foot sparkfun-like
// header+packet+footer = 256 bits (32 bytes)
#define PACKET_HEADER 'A'
typedef struct {
	int32_t latitude; // times 10000;
	int32_t longitude; // times 10000;
	uint16_t gps_altitude; // m [0-65535]
	unsigned int channel00: 10;
	unsigned int channel01: 10;
	unsigned int channel02: 10;
	unsigned int channel03: 10;
	unsigned int channel04: 10;
	unsigned int channel05: 10;
	unsigned int channel06: 10;
	unsigned int channel07: 10;
	unsigned int channel08: 10;
	unsigned int channel09: 10;
	unsigned int channel10: 10;
	unsigned int channel11: 10;
	unsigned int channel12: 10;
	unsigned int channel13: 10;
	unsigned int channel14: 10;
	unsigned int channel15: 10; //16 chans 10 bit: 20 bytes
} spacebits_telemetry_t;
#define PACKET_FOOTER 'Z'

#define COMMAND_QUEUE "_commands_"
#define TELEMETRY_TOPIC "_telemetry_"

// prototypes
void catch_quit(int signal);
int setup_port(void);
void packet_loop(void);
void command_loop(void);

