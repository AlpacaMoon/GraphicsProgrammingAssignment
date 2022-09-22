#pragma once
class Animation
{
public:
	// Main animation switch
	static bool isPlayingAnim;
	static bool* currentPlayingAnim;

	// Specific animation statuses
	static bool isIdle;
	static bool isWalking;

	// General methods
	static void resetAnimation();
	static void switchAnimation(bool* target, bool value);
	static void runAnimations();
	static void addAnimationToQueue();
};

