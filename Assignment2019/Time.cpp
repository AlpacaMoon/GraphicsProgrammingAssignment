#include "Time.h"

clock_t Time::currentTicks;
clock_t Time::elapsedTicks;
float Time::elapsedSeconds;

// Converts ticks (clock_t) to seconds
float Time::toSeconds(clock_t ticks) {
	return (float)ticks / CLOCKS_PER_SEC;
}

// Convert seconds to ticks (clock_t)
clock_t Time::toTicks(float seconds) {
	return seconds * CLOCKS_PER_SEC;
}

// Constructors
Time::Time(float seconds) {
	Time(toTicks(seconds));
}

Time::Time(clock_t ticks) {
	setTime(ticks);
}

void Time::setTime(clock_t ticks) {
	startTime = clock();
	endTime = startTime + ticks;
}