#include "Util.h"

#include <math.h>
#include <sys/time.h>
#include <stddef.h>

uint64_t util_getTimeInNano() {
    struct timeval time;
	gettimeofday(&time, NULL);
	return 1000 * ((uint64_t)(time.tv_sec % 10000) * 1000000 + (uint64_t)time.tv_usec);
}

uint64_t util_getTimeInMs() {
    return util_getTimeInNano() / 1000000;
}