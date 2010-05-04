#include <stdio.h>
#include "libsapo-broker2.h"
#include "telemetry.h"

int main(int argc, char **argv) {
	char head_foot[2] = {'A','Z'};
	spacebits_telemetry_t telem;
	FILE *telem_test = fopen("telemetry_debug", "w");
	
	telem.latitude = 3800000;
	telem.longitude = -9000000;
	telem.gps_altitude = 30123;
	telem.channel00 = 512;
	telem.channel01 = 512;
	telem.channel02 = 512;
	telem.channel03 = 512;
	telem.channel04 = 512;
	telem.channel05 = 512;
	telem.channel06 = 512;
	telem.channel07 = 512;
	telem.channel08 = 512;
	telem.channel09 = 512;
	telem.channel10 = 512;
	telem.channel11 = 512;
	telem.channel12 = 512;
	telem.channel13 = 512;
	telem.channel14 = 512;
	telem.channel15 = 512;
	
	fwrite(&head_foot[0], 1, 1, telem_test);
	fwrite(&telem, sizeof(telem), 1, telem_test);
	fwrite(&head_foot[1], 1, 1, telem_test);
	fclose(telem_test);
	return 0;
}

