#pragma once
#include <Windows.h>
#include <gl/GL.h>

class Controls
{
public:
	static bool isIndependentControls;

	// Variables for independent controls
	static int currentControlPart;
	static float rotateSpeed;

	// Variables for preset animation controls
	static int currentPlayingAnimation;

	static bool pressingWalkKeys[3];	// 'W', 'A', 'D'
	static bool isPressingWalk();

	// Methods
	static void initialize();
	static void reset();
	static void independentControls(WPARAM wParam);
	static void presetAnimationKeyDown(WPARAM wParam);
	static void presetAnimationKeyUp(WPARAM wParam);

};

