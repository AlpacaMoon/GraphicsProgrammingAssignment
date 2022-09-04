#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

class Model
{
public:

	// Concrete Models
	// (Models that are made of other models, and are adjusted to the initial world position)
	static void RightLeg();

	// Abstract/Generic Models
	// (Models that are reused for more than once)
	static void LegUpper();
	static void LegLower();
	static void LegPlate();

	static void ArmUpper();
	static void ArmLower();
	static void Hand();

	//weapon
	static void r99();
};

