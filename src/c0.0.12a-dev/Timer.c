#include "Timer.h"

#include "Util.h"
#include <stdlib.h>

void createTimer(Timer* timer, float ticksPerSecond) {
    timer->ticksPerSecond = ticksPerSecond;
    timer->lastTime = util_getTimeInNano();
    timer->timeScale = 1.0f;
    timer->fps = 0.0f;
    timer->passedTime = 0.0f;
}

void timer_advance(Timer* timer) {
    uint64_t now = util_getTimeInNano();
    uint64_t passedNs = now - timer->lastTime;
    timer->lastTime = now;

    if (passedNs < 0L) {
        passedNs = 0L;
    }

    if (passedNs > 1000000000L) {
        passedNs = 1000000000L;
    }

    timer->fps = (float) (1000000000L / passedNs);
    timer->passedTime += (float)passedNs * timer->timeScale * timer->ticksPerSecond / 1.0E9F;
    timer->ticks = timer->passedTime;

    if (timer->ticks > 100) {
        timer->ticks = 100;
    }

    timer->passedTime -= (float)timer->ticks;
    timer->a = timer->passedTime;
}
