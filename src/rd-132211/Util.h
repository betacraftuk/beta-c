#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#define frand rand() / (float) RAND_MAX

uint64_t util_getTimeInNano();
uint64_t util_getTimeInMs();

#endif
