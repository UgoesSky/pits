#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <wiringPi.h>

#include "gps.h"
#include "geiger.h"
#include "misc.h"

#define GEIGER_PIN 1

volatile int counts = 0;

void myInterrupt(void)
{
	counts++;
}

void *GeigerLoop(void *some_void_ptr)
{
	FILE *fp;
	struct TGPS *GPS;
	int val1 = 0;
	int val2 = 0;
	int val3 = 0;

	GPS = (struct TGPS *)some_void_ptr;

	if ( wiringPiISR (GEIGER_PIN, INT_EDGE_FALLING, &myInterrupt) < 0 ) {
      		fprintf (stderr, "Unable to setup ISR: %s\n", strerror (errno));
  	}

	while (1)
	{
		sleep(15);
		GPS->CPM = val1 + val2 + val3 + counts;
		val1 = val2;
		val2 = val3;
		val3 = counts;
		counts = 0;
	}

	return 0;
}
