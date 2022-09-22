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

	// Methods
	static void initialize();
	static void reset();
	static void independentControls(WPARAM wParam);
	static void presetAnimationControls(WPARAM wParam);
};

