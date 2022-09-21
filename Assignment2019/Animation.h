#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include "Time.h"
class Animation
{
public:

	static void runAnimations();

	static void reset(float seconds);

	static int walkSteps;
	static void startWalking();
	static void stopWalking();
	static void rotateWalk(WPARAM key);
	static void walk();


	// Clamping rotations
	static float clampFloat(float value, float min, float max);

	static void clampHead();
	static void clampHip();
	static void clampBody();
	static void clampRightLeg();
	static void clampLeftLeg();
	static void clampRightArm();
	static void clampLeftArm();
	static void clampRightFingers();
	static void clampLeftFingers();
};

