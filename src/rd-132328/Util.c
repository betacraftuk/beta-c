#include "Util.h"

#include <math.h>
#include <sys/time.h>
#include <stddef.h>

uint64_t startTime = 0;

uint64_t getRealTimeInNano() {
    struct timeval time;
	gettimeofday(&time, NULL);
    return 1000 * ((uint64_t)(time.tv_sec % 10000) * 1000000 + (uint64_t)time.tv_usec);
}

uint64_t util_getTimeInNano() {
    uint64_t t = getRealTimeInNano();
    if (startTime == 0) {
        startTime = t;
    }
	return t - startTime;
}

uint64_t util_getTimeInMs() {
    return getRealTimeInNano() / 1000000;
}