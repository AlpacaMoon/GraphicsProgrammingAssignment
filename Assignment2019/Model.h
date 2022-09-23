#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

class Model
{
public:

	static void Pathfinder();
	static void UpperBody();
	static void LowerBody();

	// Concrete Models
	// (Models that are made of other models, and are adjusted to the initial world position)
	static void RightLeg();
	static void LeftLeg();
	static void RightArm();
	static void LeftArm();

	// Generic Models
	// (Models that are reused for more than once / Models that are at the lowest level)

	static void Head();
	static void Torso();
	static void Waist();
	static void Buttock();
	static void ButtockJet();

	static void LegUpper();
	static void LegLower();
	static void LegPlate(float r);
	static void HipJoint();

	static void ArmUpper();
	static void ArmLower();
	static void Shoulder();
	static void RightHand();
	static void LeftHand();
	static void Finger(float r1, float r2, float r3);

	static void zipLineTube();
	static void zipLineBackTube();

	// Weapon Models
	static void r99();
	static void bullet();
	static void kukriKnife();
	static float bulletPos[3];
	static boolean isFired;

	// Variables to control animation / Model transformations
	static float bodyPos[3];
	static float headRot[3];
	static float RLegUpperRot[3];
	static float RLegHingeRot, RFeetRot;
	static float LLegUpperRot[3];
	static float LLegHingeRot, LFeetRot;
	static float RArmRot[3][3];
	static float LArmRot[3][3];
	static float RFingerRot[5][3];
	static float LFingerRot[5][3];
	static float hipRot[3];
	static float bodyRot[3];

	// Default values (Used in reset)
	static float openedFingerRot[5][3];
	static float closedFingerRot[5][3];
	static float defaultRArmRot[3][3];
	static float defaultLArmRot[3][3];
	static float defaultBodyPos[3];
	static float defaultHeadRot[3];
	static float defaultRLegUpperRot[3];
	static float defaultRLegHingeRot;
	static float defaultRFeetRot;
	static float defaultLLegUpperRot[3];
	static float defaultLLegHingeRot;
	static float defaultLFeetRot;
	static float defaultHipRot[3];
	static float defaultBodyRot[3];

	// Variables to control textures
	static GLuint tvTexture;
};

