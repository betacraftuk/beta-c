#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

typedef struct Timer {
    float ticksPerSecond;
	uint64_t lastTime;
	int ticks;
	float a;
    float timeScale;
    float fps;
    float passedTime;
} Timer;

void createTimer(Timer* timer, float ticksPerSecond);
void timer_advance(Timer* timer);

#endif
