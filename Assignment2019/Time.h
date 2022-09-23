#pragma once
#include <ctime>

class Time
{
public:
	// Static variables shared across the entire program
	static clock_t currentTicks;
	static clock_t elapsedTicks;
	static float elapsedSeconds;

	// Static methods
	static float toSeconds(clock_t ticks);
	static clock_t toTicks(float seconds);


	// Non-static variables for each Time instance, used in animation
	Time(float seconds);
	Time(clock_t ticks);
	void setTime(float seconds);
	void setTime(clock_t ticks);
	clock_t startTime;
	clock_t endTime;
	bool hasPassedEndTime();
	float timePassedInSeconds();
	float timePassedInRatio();
};