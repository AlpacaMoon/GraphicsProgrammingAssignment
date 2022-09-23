#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include "Time.h"
class Animation
{
public:

	static void runAnimations();
	static int playingCutscene;

	static void cancelAllAnimations();
	static void startReset();
	static bool isResetting;

	static void hardReset();
	static bool softReset(float speed);
	static bool softResetClamping(float* target, float min, float frustum, float max, float speed);
	
	// Walking Animation
	static int walkSteps, walkDir;
	static float walkSpeed, walkBodyShifting;
	static void startWalking();
	static void stopWalking();
	static void rotateWalk();
	static void walk();

	// Jumping Animation
	static int jumpSteps;
	static float jumpSpeed, movingBodyPartSpeed, jumpHeight;
	static bool isJumping();
	static void startJumping();
	static void jump();


	// Cutscene
	// 1) Zipline away


	// General Animations
	static float fingerSpeed;
	static void closeRightHand(float fingerSpeed);
	static void closeLeftHand(float fingerSpeed);
	static void openRightHand(float fingerSpeed);
	static void openLeftHand(float fingerSpeed);

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

	//weapon
	static  void shootBullet();

	// TV screen
	static int currentTVscreen;
	static void switchTVscreen();
};

