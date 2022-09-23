#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Model.h"
#include "Utility.h"
#include "CoordinateSet.h"
#include "Color.h"
#include "Lightning.h"
#include "Texture.h"
#include "TextureMap.h"
#include "Animation.h"

// Three main axes
float xAxis[3] = { 1, 0, 0 };
float yAxis[3] = { 0, 1, 0 };
float zAxis[3] = { 0, 0, 1 };

// General values for model
float torsoWidth = 0.5f;
float waistWidth = 0.2f;
float legWidth = 0.125f;
float armWidth = 0.15f;
float handThickness = 0.075f;
float buttWidth = 0.1f;

// Weapon Variables
/* Current weapon
*	0 = Empty handed
*	1 = Gun (R99)
*	2 = Knife (Kukri Knife)
*/
int Model::currentWeapon = 1;
GLfloat Model::gunFirePointMatrix[16];
GLfloat Model::temporaryMatrix[16];
boolean Model::isFired = false;
float Model::bulletSpeed = 50.0f;
float Model::fireCooldown = 1 / 100.0f;
float Model::bulletSpread = 5.0f;

/* Bullet positions
*	- Stores the necessary values needed to calculate the positions of all bullets on screen
*	[0] = Z-value, distance away from the gun (value of 1 means that this position has no bullet)
*	[1] = Spread angle on X-axis
*	[2] = Spread angle on Y-axis
*/
float Model::bulletPositions[MAX_BULLETS_ON_SCREEN][3];
float Model::bulletDespawnDistance = 30.0f;

// Texture Variables
GLuint Model::tvTexture;

// GLUquadricObj variables
GLUquadricObj* weaponObj;
GLUquadricObj* headObj;
GLUquadricObj* torsoObj, * waistObj, * buttockJetObj;
GLUquadricObj* torsoBar;
GLUquadricObj* legLowerSpring, * legPlateObj;
GLUquadricObj* leftLegObj, * rightLegObj;
GLUquadricObj* hipBallObj;
GLUquadricObj* armUpperObj, * armLowerObj, * shoulderObj, * fingerObj;;
GLUquadricObj* zipLineTubeObj, * zipLineBackTubeObj;

// Animation variables / Model transformation variables
float Model::bodyPos[3] = { 0, 0, 0 };
float Model::headRot[3] = { 0, 0, 0 };
float Model::RLegUpperRot[3] = { 0, 0, 0 };
float Model::RLegHingeRot = 0;
float Model::RFeetRot = 0;
float Model::LLegUpperRot[3] = { 0, 0, 0 };
float Model::LLegHingeRot = 0;
float Model::LFeetRot = 0;
float Model::hipRot[3] = { 0, 0, 0 };
float Model::bodyRot[3] = { 0, 0, 0 };

float Model::defaultBodyPos[3] = { 0, 0, 0 };
float Model::defaultHeadRot[3] = { 0, 0, 0 };
float Model::defaultRLegUpperRot[3] = { 0, 0, 0 };
float Model::defaultRLegHingeRot = 0;
float Model::defaultRFeetRot = 0;
float Model::defaultLLegUpperRot[3] = { 0, 0, 0 };
float Model::defaultLLegHingeRot = 0;
float Model::defaultLFeetRot = 0;
float Model::defaultHipRot[3] = { 0, 0, 0 };
float Model::defaultBodyRot[3] = { 0, 0, 0 };

float Model::RArmRot[3][3] = {
	{20, -10, 15},
	{0, 0, 90},
	{0, 0, 0}
};
float Model::LArmRot[3][3] = {
	{-20, 10, 15},
	{0, 0, 90},
	{0, 0, 0}
};
float Model::defaultRArmRot[3][3] = {
	{20, -10, 15},
	{0, 0, 90},
	{0, 0, 0}
};
float Model::defaultLArmRot[3][3] = {
	{-20, 10, 15},
	{0, 0, 90},
	{0, 0, 0}
};
float Model::RFingerRot[5][3] = {
	{30, 37, 45},
	{15, 70, 70},
	{40, 70, 70},
	{80, 80, 80},
	{80, 80, 80}
};
float Model::LFingerRot[5][3] = {
	{15, 15, 15},
	{10, 10, 10},
	{10, 10, 10},
	{10, 10, 10},
	{10, 10, 10}
};
float Model::openedFingerRot[5][3] = {
	{15, 15, 15},
	{10, 10, 10},
	{10, 10, 10},
	{10, 10, 10},
	{10, 10, 10}
};
float Model::closedFingerRot[5][3] = {
	{30, 37, 45},
	{80, 80, 80},
	{80, 80, 80},
	{80, 80, 80},
	{80, 80, 80}
};
float Model::holdingGunFingerRot[5][3] = {
	{30, 37, 45},
	{15, 70, 70},
	{40, 70, 70},
	{80, 80, 80},
	{80, 80, 80}
};
void Model::Pathfinder() {
	glPushMatrix();
	{
		glTranslatef(bodyPos[0], bodyPos[1], bodyPos[2]);

		glRotatef(bodyRot[1], 0, 1, 0);
		glRotatef(bodyRot[0], 1, 0, 0);
		glRotatef(bodyRot[2], 0, 0, 1);

		glRotatef(90, 0, 1, 0);

		// Displacement to rotation pivots
		float displace = 0.35;
		float displace2 = 0.15;
		glPushMatrix();
		{
			glTranslatef(0, -displace2, 0);
			glRotatef(hipRot[0], 1, 0, 0);
			glRotatef(hipRot[1], 0, 1, 0);
			glTranslatef(0, displace2, 0);
			glTranslatef(0, -displace, 0);
			glRotatef(hipRot[2], 0, 0, 1);
			glTranslatef(0, displace, 0);
			glTranslatef(-0.01, 0.275, 0);
			glScalef(0.75, 0.9, 0.9);
			UpperBody();
		}
		glPopMatrix();

		glTranslatef(0, -0.175, 0);
		LowerBody();
	}
	glPopMatrix();
}

void Model::UpperBody() {
	glPushMatrix();
	{
		glPushMatrix();
		{
			glTranslatef(0, 0.3, 0);
			glScalef(0.7, 0.7, 0.7);
			Head();
		}
		glPopMatrix();

		glPushMatrix();
		{
			glScalef(0.5, 0.5, 0.5);
			Torso();
		}
		glPopMatrix();

		float Zdisplacement = 0.49f;
		glPushMatrix();
		{
			glTranslatef(0, 0, -Zdisplacement / 2.0f);
			glScalef(0.9, 0.9, 0.9);
			RightArm();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0, 0, Zdisplacement / 2.0f);
			glScalef(0.9, 0.9, 0.9);
			LeftArm();
		}
		glPopMatrix();

	}
	glPopMatrix();
}

void Model::LowerBody() {
	glPushMatrix();
	{
		float legScale = 1.1;
		glPushMatrix();
		{
			glRotatef(hipRot[0], 1, 0, 0);
			glRotatef(hipRot[1], 0, 1, 0);
			glTranslatef(0, -0.185, 0);
			glRotatef(hipRot[2], 0, 0, 1);
			glTranslatef(0, 0.185, 0);
			Waist();
		}
		glPopMatrix();

		float gap = 0.115;
		glTranslatef(0, -0.19, -gap);

		glPushMatrix();
		{
			glScalef(legScale, legScale, legScale);
			Model::RightLeg();
		}
		glPopMatrix();
		glTranslatef(0, 0, gap * 2);

		glScalef(legScale, legScale, legScale);
		Model::LeftLeg();
	}
	glPopMatrix();
}

// Concrete Models (Highest-level model)

void Model::RightLeg() {
	glPushMatrix();
	{
		glScalef(0.5, 0.5, 0.5);

		glRotatef(RLegUpperRot[0], 1, 0, 0);
		glRotatef(RLegUpperRot[1], 0, 1, 0);
		glRotatef(RLegUpperRot[2], 0, 0, 1);

		glTranslatef(0, 0, -0.14);
		// Upper leg
		glPushMatrix();
		{

			glTranslatef(0, 0, legWidth / 2.0f);
			HipJoint();
		}
		glPopMatrix();
		LegUpper();

		// Lower Leg
		glTranslatef(0.215, -0.785, 0);
		glRotatef(RLegHingeRot, 0, 0, 1);
		glPushMatrix();
		{
			glScalef(1.1, 1.1, 1.1);
			LegLower();
		}
		glPopMatrix();

		// Feet
		glTranslatef(-0.2, -0.85, 0);
		glPushMatrix();
		{
			glScalef(1.1, 1.1, 1.1);
			LegPlate(RFeetRot);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Model::LeftLeg() {
	glPushMatrix();
	{
		glScalef(0.5, 0.5, 0.5);

		glRotatef(LLegUpperRot[0], 1, 0, 0);
		glRotatef(LLegUpperRot[1], 0, 1, 0);
		glRotatef(LLegUpperRot[2], 0, 0, 1);

		glTranslatef(0, 0, 0.14);

		// Upper leg
		glPushMatrix();
		{
			glRotatef(180, 1, 0, 0);
			glTranslatef(0, 0, legWidth / 2.0f);
			HipJoint();
		}
		glPopMatrix();
		LegUpper();

		// Lower Leg
		glTranslatef(0.215, -0.785, 0);
		glRotatef(LLegHingeRot, 0, 0, 1);
		glPushMatrix();
		{
			glScalef(1.1, 1.1, 1.1);
			LegLower();
		}
		glPopMatrix();

		// Feet
		glTranslatef(-0.2, -0.85, 0);
		glPushMatrix();
		{
			glScalef(1.1, 1.1, 1.1);
			LegPlate(LFeetRot);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Model::RightArm() {
	glPushMatrix();
	{
		glScalef(0.5, 0.5, 0.5);

		// Upper arm
		glRotatef(RArmRot[0][0], 1, 0, 0);
		glRotatef(RArmRot[0][1], 0, 1, 0);
		glRotatef(RArmRot[0][2], 0, 0, 1);
		glTranslatef(-0.25, -1.08, -armWidth / 2.0f);
		Shoulder();
		ArmUpper();

		// Zipline tube on upper arm
		glPushMatrix();
		{
			glTranslatef(0.05, 0.5, -0.12);
			glRotatef(-90, 1, 0, 0);
			glScalef(1.2, 1.2, 1.2);
			zipLineTube();
		}
		glPopMatrix();

		// Lower Arm
		glTranslatef(0.25, -0.12, 0);
		glRotatef(RArmRot[1][0], 1, 0, 0);
		glRotatef(RArmRot[1][1], 0, 1, 0);
		glRotatef(RArmRot[1][2], 0, 0, 1);
		glRotatef(-90, 0, 0, 1);
		ArmLower();

		// Hand
		glTranslatef(0.93, 0, 0.0375);
		glRotatef(RArmRot[2][0], 1, 0, 0);
		glRotatef(RArmRot[2][1], 0, 1, 0);
		glRotatef(RArmRot[2][2], 0, 0, 1);
		RightHand();

		// Weapon
		glPushMatrix();
		{
			switch (currentWeapon) {
			case 0:
				// Empty handed
				break;
			case 1:
				// Gun (R99)
				glTranslatef(0.605, 0.25, 0.05);
				glRotatef(3, 0, 0, 1);
				glRotatef(90, 0, -1, 0);

				glPushMatrix();
				{
					glTranslatef(0, 0, -0.6);
					// Save the fire point as a transformation matrix
					glGetFloatv(GL_MODELVIEW_MATRIX, gunFirePointMatrix);
				}
				glPopMatrix();

				r99();
				break;
			case 2:
				// Knife (Kukri Knife)
				break;
			}
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Model::LeftArm() {
	glPushMatrix();
	{
		glScalef(0.5, 0.5, 0.5);

		// Upper arm
		glRotatef(LArmRot[0][0], 1, 0, 0);
		glRotatef(LArmRot[0][1], 0, 1, 0);
		glRotatef(LArmRot[0][2], 0, 0, 1);
		glTranslatef(-0.25, -1.08, -armWidth / 2.0f);
		Shoulder();
		ArmUpper();

		// Lower Arm
		glTranslatef(0.25, -0.12, 0);
		glRotatef(LArmRot[1][0], 1, 0, 0);
		glRotatef(LArmRot[1][1], 0, 1, 0);
		glRotatef(LArmRot[1][2], 0, 0, 1);
		glRotatef(-90, 0, 0, 1);
		ArmLower();

		// Hand
		glTranslatef(0.93, 0, 0.0375f + armWidth / 2.0f);
		glRotatef(LArmRot[2][0], 1, 0, 0);
		glRotatef(LArmRot[2][1], 0, 1, 0);
		glRotatef(LArmRot[2][2], 0, 0, 1);
		glRotatef(180, 1, 0, 0);
		glRotatef(180, 0, 0, 1);
		LeftHand();
	}
	glPopMatrix();
}


// Generic Models (Basic unit models)

void Model::Head() {
	if (headObj == NULL) {
		headObj = gluNewQuadric();
	}

	CoordinateSet tempSet(1), tempSet2(1);

	glPushMatrix();
	{
		float headWidth = 0.25;
		float headWidthHalf = headWidth / 2.0f;
		float headHeightHalf = 0.12f;
		float eyeBallRadius = 0.08f;
		float eyeBallCircumInner = 0.07f;
		float eyeBallCircum = headWidthHalf - 0.02f;
		float eyeBallCircumExtrude = 0.04f;
		float eyeBallEdges = 16;

		glPushMatrix();
		{
			Color::lightBlue();
			Lightning::blueMaterial();

			// Apply head rotation
			glRotatef(headRot[0], 1, 0, 0);
			glRotatef(headRot[1], 0, 1, 0);
			glRotatef(headRot[2], 0, 0, 1);

			// Skull
			glPushMatrix();
			{
				glTranslatef(0, 0, -headWidthHalf);
				tempSet.addCoordinate(0.3, headHeightHalf, 0);
				tempSet.addCoordinate(0, headHeightHalf + 0.08f, 0);
				tempSet.addCoordinate(-0.3, headHeightHalf, 0);
				tempSet2.destroy();
				tempSet2 = Utility::bezierCurveCoords(tempSet, 6);
				tempSet2.addCoordinate(-0.3, -0.12, 0);
				tempSet2.addCoordinate(0.3, -0.12, 0);

				float center[3] = { 0, 0, 0 };

				Utility::extrudePolygon(tempSet2, center, zAxis, headWidth, TextureMap::allBlue());
			}
			glPopMatrix();

			// Black disk around eye
			glPushMatrix();
			{
				Color::lightGrey();
				Lightning::greyMaterial();

				glTranslatef(0.301, 0, 0);

				// Plate at the front
				glPushMatrix();
				{
					glTranslatef(eyeBallCircumExtrude, 0, 0);
					glRotatef(90, 0, 1, 0);
					gluDisk(headObj, eyeBallCircumInner, eyeBallCircum, eyeBallEdges, 1);
				}
				glPopMatrix();

				// Outer cylinder
				glPushMatrix();
				{
					glRotatef(90, 0, 1, 0);
					float center[3] = { 0, 0, 0 };
					tempSet.destroy();
					tempSet = Utility::circleCoords(center, eyeBallCircum, eyeBallEdges);
					tempSet2.destroy();
					tempSet2 = tempSet.copy();
					tempSet2.translate(0, 0, eyeBallCircumExtrude);
					float volumeCenter[3] = { center[0], center[1], center[2] + eyeBallCircumExtrude / 2.0f };
					Utility::connectTwoFaces(tempSet, tempSet2, volumeCenter);
				}
				glPopMatrix();

				// Inner cylinder
				glPushMatrix();
				{
					glRotatef(90, 0, 1, 0);
					float center[3] = { 0, 0, 0 };
					tempSet.destroy();
					tempSet = Utility::circleCoords(center, eyeBallCircumInner, eyeBallEdges);
					tempSet2.destroy();
					tempSet2 = tempSet.copy();
					tempSet2.translate(0, 0, eyeBallCircumExtrude);
					float volumeCenter[3] = { center[0], center[1], center[2] + eyeBallCircumExtrude / 2.0f };
					Utility::connectTwoFaces(tempSet, tempSet2, volumeCenter);
				}
				glPopMatrix();
			}
			glPopMatrix();

			// Eyeball
			glPushMatrix();
			{
				Color::gold();
				Lightning::goldMaterial();

				Texture::on();
				Texture::use(Texture::_eye);
				gluQuadricTexture(headObj, GL_TRUE);
				glTranslatef(0.28, 0, 0);
				glRotatef(90, 0, 0, 1);
				gluSphere(headObj, eyeBallRadius, 16, 16);
				gluQuadricTexture(headObj, GL_FALSE);
				Texture::off();
			}
			glPopMatrix();

			// Yellow eyebrows Upper
			glPushMatrix();
			{
				float barRad = 0.015f;
				float barOffset = 0;

				Color::yellow();
				Lightning::yellowMaterial();

				tempSet.clear(10);
				tempSet.addCoordinate(0.2, 0.0251, 0);
				tempSet.addCoordinate(0.2, 0.025, -headWidthHalf + barOffset);
				tempSet.addCoordinate(0.325, 0.025, -headWidthHalf + barOffset);
				tempSet.addCoordinate(0.325, 0.03, -headWidthHalf + barOffset);
				Utility::drawStraightTubes(tempSet, 6, 3, 8, barRad, 0.1, Texture::_yellow);

				tempSet.clear(10);
				tempSet.addCoordinate(0.2, 0.0251, 0);
				tempSet.addCoordinate(0.2, 0.025, headWidthHalf - barOffset);
				tempSet.addCoordinate(0.325, 0.025, headWidthHalf - barOffset);
				tempSet.addCoordinate(0.325, 0.03, headWidthHalf - barOffset);
				Utility::drawStraightTubes(tempSet, 6, 3, 8, barRad, 0.1, Texture::_yellow);

				glPushMatrix();
				{
					tempSet.clear(5);
					tempSet.addCoordinate(0.325, 0.03, -headWidthHalf + barOffset);
					tempSet.addCoordinate(0.325, headWidthHalf * 1.35f - barOffset, -headWidthHalf + barOffset);
					tempSet.addCoordinate(0.325, headWidthHalf * 1.35f - barOffset, headWidthHalf - barOffset);
					tempSet.addCoordinate(0.325, 0.03, headWidthHalf - barOffset);
					Utility::drawBezierTube(tempSet, 12, 8, barRad, Texture::_yellow);
				}
				glPopMatrix();
			}
			glPopMatrix();

			// Yellow eyebrows Lower
			glPushMatrix();
			{
				float barRad = 0.015f;
				float barOffset = 0;

				Color::yellow();
				Lightning::yellowMaterial();

				tempSet.clear(10);
				tempSet.addCoordinate(0.2, -0.0251, 0);
				tempSet.addCoordinate(0.2, -0.025, -headWidthHalf + barOffset);
				tempSet.addCoordinate(0.325, -0.025, -headWidthHalf + barOffset);
				tempSet.addCoordinate(0.325, -0.03, -headWidthHalf + barOffset);
				Utility::drawStraightTubes(tempSet, 6, 3, 8, barRad, 0.1, Texture::_yellow);

				tempSet.clear(10);
				tempSet.addCoordinate(0.2, -0.0251, 0);
				tempSet.addCoordinate(0.2, -0.025, headWidthHalf - barOffset);
				tempSet.addCoordinate(0.325, -0.025, headWidthHalf - barOffset);
				tempSet.addCoordinate(0.325, -0.03, headWidthHalf - barOffset);
				Utility::drawStraightTubes(tempSet, 6, 3, 8, barRad, 0.1, Texture::_yellow);

				glPushMatrix();
				{
					tempSet.clear(5);
					tempSet.addCoordinate(0.325, -0.03, -headWidthHalf + barOffset);
					tempSet.addCoordinate(0.325, -headWidthHalf * 1.35f + barOffset, -headWidthHalf + barOffset);
					tempSet.addCoordinate(0.325, -headWidthHalf * 1.35f + barOffset, headWidthHalf - barOffset);
					tempSet.addCoordinate(0.325, -0.03, headWidthHalf - barOffset);
					Utility::drawBezierTube(tempSet, 12, 8, barRad, Texture::_yellow);
				}
				glPopMatrix();
			}
			glPopMatrix();

			// Ears
			glPushMatrix();
			{
				Color::lightBlue2();
				Lightning::darkerBlueMaterial();

				float earRadius = 0.12f;
				float earExtrude = 0.02f;
				int earEdges = 16;
				float center[3] = { 0, 0, 0 };

				// Right ear drum
				glTranslatef(0, 0.02, -headWidthHalf - earExtrude);
				CoordinateSet ear = Utility::circleCoords(center, earRadius, earEdges);
				Utility::extrudePolygon(ear, center, zAxis, earExtrude, TextureMap::allBlue(), true, false);

				glTranslatef(0, 0, headWidth + earExtrude);
				Utility::extrudePolygon(ear, center, zAxis, earExtrude, TextureMap::allBlue(), false, true);
			}
			glPopMatrix();
		}
		glPopMatrix();

		// Neck
		glPushMatrix();
		{
			Color::lightGrey();
			float neckRadiusUpper = headWidthHalf - 0.02f;
			float neckRadiusLower = headWidthHalf;
			float neckLength = 0.15;
			int neckEdges = 16;
			float center[3] = { 0, 0, -headHeightHalf };

			// Compute upper neck (upper neck rotates as the head rotates)
			CoordinateSet upperNeck = Utility::circleCoords(center, neckRadiusUpper, neckEdges);
			upperNeck.rotate(-90, 0, 0);
			upperNeck.rotate(headRot[0], headRot[1], headRot[2]);

			// Compute lower neck (Stays still no matter the head rotation)
			float center2[3] = { center[0], center[1], center[2] - neckLength };
			CoordinateSet lowerNeck = Utility::circleCoords(center2, neckRadiusLower, neckEdges);
			lowerNeck.rotate(-90, 0, 0);

			float volumeCenter[3] = { 0, -headHeightHalf - neckLength / 2.0f, 0 };

			Utility::connectTwoFaces(upperNeck, lowerNeck, center2);

			upperNeck.destroy();
			lowerNeck.destroy();
		}
		glPopMatrix();
	}
	glPopMatrix();

	tempSet.destroy();
	tempSet2.destroy();
}


void Model::Torso() {
	if (torsoObj == NULL) {
		torsoObj = gluNewQuadric();
	}

	if (torsoBar == NULL) {
		torsoBar = gluNewQuadric();
	}

	glPushMatrix();
	{
		CoordinateSet tempSet(10);
		CoordinateSet tempSet2(1);

		// Torso
		glPushMatrix();
		{
			float z1 = -torsoWidth / 2.0f;
			CoordinateSet torso(10);
			torso.addCoordinate(0.75, 0.1, z1);
			torso.addCoordinate(0.5, 0.25, z1);
			torso.addCoordinate(-0.4, 0.25, z1);
			torso.addCoordinate(-0.75, -0.05, z1);
			torso.addCoordinate(-0.65, -0.25, z1);
			torso.addCoordinate(0.75, -0.25, z1);

			Color::lightBlue();
			Lightning::blueMaterial();

			float torsoCenter[3] = { 0, 0, z1 };

			Utility::extrudePolygon(torso, torsoCenter, zAxis, torsoWidth, TextureMap::allBlue());

			Color::red();
			Lightning::windRedMaterial();

			// Deco Right
			glPushMatrix();
			{
				int decoFaces = 6;
				float decoRadius = 0.01;

				glTranslatef(-0.5, 0, -torsoWidth / 2.0f - decoRadius);
				glRotatef(37.5, 0, 0, 1);
				glTranslatef(0, 0.05, 0);

				glPushMatrix();
				{
					glTranslatef(-0.1, 0.01, 0);
					glRotatef(90, 1, 0, 0);
					gluCylinder(torsoObj, decoRadius, decoRadius, 0.12, decoFaces, 1);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(0.1, 0.01, 0);
					glRotatef(90, 1, 0, 0);
					gluCylinder(torsoObj, decoRadius, decoRadius, 0.12, decoFaces, 1);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(-0.11, -0.1, 0);
					glRotatef(90, 0, 1, 0);
					gluCylinder(torsoObj, decoRadius, decoRadius, 0.22, decoFaces, 1);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(-0.1, 0, 0);
					glRotatef(90, 0, 1, 0);
					gluCylinder(torsoObj, decoRadius, decoRadius, 0.045, decoFaces, 1);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(0.1 - 0.045, 0, 0);
					glRotatef(90, 0, 1, 0);
					gluCylinder(torsoObj, decoRadius, decoRadius, 0.045, decoFaces, 1);
				}
				glPopMatrix();
			}
			glPopMatrix();

			// Deco Left
			glPushMatrix();
			{
				int decoFaces = 6;
				float decoRadius = 0.01;

				glTranslatef(-0.5, 0, torsoWidth / 2.0f + decoRadius);
				glRotatef(37.5, 0, 0, 1);
				glTranslatef(0, 0.05, 0);

				glPushMatrix();
				{
					glTranslatef(-0.1, 0.01, 0);
					glRotatef(90, 1, 0, 0);
					gluCylinder(torsoObj, decoRadius, decoRadius, 0.12, decoFaces, 1);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(0.1, 0.01, 0);
					glRotatef(90, 1, 0, 0);
					gluCylinder(torsoObj, decoRadius, decoRadius, 0.12, decoFaces, 1);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(-0.11, -0.1, 0);
					glRotatef(90, 0, 1, 0);
					gluCylinder(torsoObj, decoRadius, decoRadius, 0.22, decoFaces, 1);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(-0.1, 0, 0);
					glRotatef(90, 0, 1, 0);
					gluCylinder(torsoObj, decoRadius, decoRadius, 0.045, decoFaces, 1);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(0.1 - 0.045, 0, 0);
					glRotatef(90, 0, 1, 0);
					gluCylinder(torsoObj, decoRadius, decoRadius, 0.045, decoFaces, 1);
				}
				glPopMatrix();
			}
			glPopMatrix();

			torso.destroy();
		}
		glPopMatrix();

		// Back
		glPushMatrix();
		{
			Color::lightBlue2();
			Lightning::darkerBlueMaterial();

			float halfTorsoWidth = torsoWidth / 2.0f;
			float levels[3] = { -0.25, -0.4, -0.6 };
			float radiuses[3] = { 1, 0.875, 0.6 };
			CoordinateSet back1(10);
			back1.addCoordinate(-0.55, levels[0], -halfTorsoWidth);
			back1.addCoordinate(-0.1, levels[0], -halfTorsoWidth);
			back1.addCoordinate(-0.1, levels[0], halfTorsoWidth);
			back1.addCoordinate(-0.55, levels[0], halfTorsoWidth);

			CoordinateSet back2(10);
			back2.addCoordinate(-0.52, levels[1], -halfTorsoWidth * radiuses[1]);
			back2.addCoordinate(-0.1, levels[1], -halfTorsoWidth * radiuses[1]);
			back2.addCoordinate(-0.1, levels[1], halfTorsoWidth * radiuses[1]);
			back2.addCoordinate(-0.52, levels[1], halfTorsoWidth * radiuses[1]);

			CoordinateSet back3(10);
			back3.addCoordinate(-0.35, levels[2], -halfTorsoWidth * radiuses[2]);
			back3.addCoordinate(-0.1, levels[2], -halfTorsoWidth * radiuses[2]);
			back3.addCoordinate(-0.1, levels[2], halfTorsoWidth * radiuses[2]);
			back3.addCoordinate(-0.35, levels[2], halfTorsoWidth * radiuses[2]);

			float volumeCenter[3] = { -0.25, levels[2], 0 };
			Utility::connectTwoFaces(back1, back2, Texture::_blue, volumeCenter);
			Utility::connectTwoFaces(back2, back3, Texture::_blue, volumeCenter);

			back1.destroy();
			back2.destroy();
			back3.destroy();
		}
		glPopMatrix();

		// Shoulder connection
		glPushMatrix();
		{
			Color::lightBlue2();
			Lightning::darkerBlueMaterial();

			float radius[2] = { 0.2f, 0.12f };
			float widths[2] = { 0.075, 0.15 };
			Texture::on();
			Texture::use(Texture::_blue);
			gluQuadricTexture(torsoObj, GL_TRUE);
			glPushMatrix();
			{
				glTranslatef(0, 0, -torsoWidth / 2.0f - widths[0]);
				gluCylinder(torsoObj, radius[0], radius[0], widths[0], 16, 1);
				glTranslatef(0, 0, -widths[1]);
				gluCylinder(torsoObj, radius[1], radius[0], widths[1], 16, 1);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslatef(0, 0, torsoWidth / 2.0f);
				gluCylinder(torsoObj, radius[0], radius[0], widths[0], 16, 1);
				glTranslatef(0, 0, widths[0]);
				gluCylinder(torsoObj, radius[0], radius[1], widths[1], 16, 1);
			}
			glPopMatrix();
			gluQuadricTexture(torsoObj, GL_FALSE);
			Texture::off();
		}
		glPopMatrix();

		// Television body in front of chest
		float tvBodyWidth = torsoWidth + 0.25f;
		glPushMatrix();
		{
			Color::lightBlue2();
			Lightning::darkerBlueMaterial();

			CoordinateSet chest(10);
			chest.addCoordinate(0.85, 0.1, 0);
			chest.addCoordinate(0.7, 0.1, 0);
			chest.addCoordinate(0.68, 0.08, 0);
			chest.addCoordinate(0.6, 0.08, 0);
			chest.addCoordinate(0.4, -0.05, 0);
			chest.addCoordinate(0.4, -0.15, 0);
			chest.addCoordinate(0.37, -0.2, 0);
			chest.addCoordinate(0.37, -0.6, 0);
			chest.addCoordinate(0.85, -0.6, 0);
			chest.addCoordinate(0.85, -0.55, 0);
			chest.addCoordinate(0.815, -0.53, 0);
			chest.addCoordinate(0.815, 0.03, 0);
			chest.addCoordinate(0.85, 0.05, 0);

			glTranslatef(0, 0, -tvBodyWidth / 2.0f);
			float chestCenter[3] = { 0.55, -0.25, 0 };
			Utility::extrudePolygon(chest, chestCenter, zAxis, tvBodyWidth, TextureMap::allBlue());

			chest.destroy();
		}
		glPopMatrix();

		// Television Screen
		glPushMatrix();
		{
			Color::cyan();
			glTranslatef(0, -0.01, 0);
			// 0.68027 : 0.45 = The tv texture ratio
			float tvWidthHalf = (0.68027f) / 2.0f;
			float tvHeight = 0.45 * 1.1;
			float x = 0.8151f;
			float volumeCenter[3] = { 0, 0, 0 };

			Texture::on();
			Texture::use(Texture::_tvTextures[Animation::currentTVscreen]);
			glBegin(GL_QUADS);
			glTexCoord2f(1, 1);
			glVertex3f(x, 0, -tvWidthHalf);
			glTexCoord2f(1, 0);
			glVertex3f(x, -tvHeight, -tvWidthHalf);
			glTexCoord2f(0, 0);
			glVertex3f(x, -tvHeight, tvWidthHalf);
			glTexCoord2f(0, 1);
			glVertex3f(x, 0, tvWidthHalf);
			glEnd();
			Texture::off();
		}
		glPopMatrix();

		// Protection below tv
		glPushMatrix();
		{
			Color::lightBlue2();
			Lightning::darkerBlueMaterial();

			float pWidth = torsoWidth * 0.4f;
			float gap = torsoWidth * 0.1f;
			CoordinateSet protection(10);
			protection.addCoordinate(0, 0, 0);
			protection.addCoordinate(0, -0.5, 0);
			protection.addCoordinate(0.11, -0.5, 0);
			protection.addCoordinate(0.24, -0.37, 0);
			protection.addCoordinate(0.25, 0, 0);

			float protCenter[3] = { 0.12, -0.25, 0 };

			// Connection to the protections
			CoordinateSet bar(10);
			bar.addCoordinate(-0.05, 0, 0);
			bar.addCoordinate(-0.05, 0.15, 0);
			bar.addCoordinate(-0.15, 0.15, 0);
			bar.addCoordinate(-0.15, -0.1, 0);
			bar.addCoordinate(0, -0.1, 0);
			bar.addCoordinate(0, 0, 0);
			bar.translate(0, -0.05, 0);
			float barCenter[3] = { -0.1, -0.05, 0 };
			float barWidth = pWidth / 2.0f;

			glTranslatef(0.6, -0.62, 0);
			glPushMatrix();
			{
				glTranslatef(0, 0, -pWidth / 2.0f);
				glRotatef(-10, 0, 0, 1);
				Color::lightBlue2();
				Lightning::darkerBlueMaterial();
				Utility::extrudePolygon(protection, protCenter, zAxis, pWidth, TextureMap::allBlue());

				Color::lightGrey();
				Lightning::greyMaterial();
				Utility::extrudePolygon(bar, barCenter, zAxis, barWidth);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslatef(0, 0, -pWidth / 2.0f - gap - pWidth);
				glRotatef(-10, 0, 0, 1);
				Color::lightBlue2();
				Lightning::darkerBlueMaterial();
				Utility::extrudePolygon(protection, protCenter, zAxis, pWidth, TextureMap::allBlue());

				Color::lightGrey();
				Lightning::greyMaterial();
				Utility::extrudePolygon(bar, barCenter, zAxis, barWidth);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslatef(0, 0, gap + pWidth / 2.0f);
				glRotatef(-10, 0, 0, 1);
				Color::lightBlue2();
				Lightning::darkerBlueMaterial();
				Utility::extrudePolygon(protection, protCenter, zAxis, pWidth, TextureMap::allBlue());

				Color::lightGrey();
				Lightning::greyMaterial();
				Utility::extrudePolygon(bar, barCenter, zAxis, barWidth);
			}
			glPopMatrix();

			bar.destroy();
			protection.destroy();
		}
		glPopMatrix();

		// Yellow bars protecting TV
		glPushMatrix();
		{
			Color::yellow();
			Lightning::yellowMaterial();

			float barRad = 0.02f;
			float barLen1 = torsoWidth + 0.3f;
			float barLenHalf = barLen1 / 2.0f;
			float barOutGap = 0.5f;

			tempSet.clear(10);
			tempSet.addCoordinate(0.65, -0.4, -barLenHalf + barOutGap);
			tempSet.addCoordinate(0.65, -0.4, -barLenHalf);
			tempSet.addCoordinate(0.75, -0.6, -barLenHalf);
			tempSet.addCoordinate(0.9, -0.6, -barLenHalf);
			tempSet.addCoordinate(0.9, -0.6, 0);
			tempSet.addCoordinate(0.9, -0.6, barLenHalf);
			tempSet.addCoordinate(0.75, -0.6, barLenHalf);
			tempSet.addCoordinate(0.65, -0.4, barLenHalf);
			tempSet.addCoordinate(0.65, -0.4, barLenHalf - barOutGap);
			Utility::drawStraightTubes(tempSet, 8, 4, 8, barRad, 0.2f, Texture::_yellow);
		}
		glPopMatrix();

		// Bar on shoulder
		glPushMatrix();
		{
			Color::lightBlue2();
			Lightning::darkerBlueMaterial();

			float barRad = 0.02f;
			float barGap = 0.05f;
			glTranslatef(0, 0, -torsoWidth / 2.0f - barRad - barGap);
			tempSet.clear(10);
			tempSet.addCoordinate(-0.2, 0.15, 0);
			tempSet.addCoordinate(-0.15, 0.37, 0);
			tempSet.addCoordinate(0.7, 0.37, 0);
			tempSet.addCoordinate(0.75, 0.05, 0);
			Utility::drawBezierTube(tempSet, 12, 8, barRad, Texture::_blue);

			glPushMatrix();
			{
				glTranslatef(0, 0, torsoWidth + (barGap + barRad) * 2);
				Utility::drawBezierTube(tempSet, 12, 8, barRad, Texture::_blue);
			}
			glPopMatrix();

			glTranslatef(0, 0, barGap);
			tempSet.clear(10);
			tempSet.addCoordinate(-0.25, 0.1, barGap);
			tempSet.addCoordinate(-0.25, 0.1, -barGap);
			tempSet.addCoordinate(-0.2, 0.15, -barGap);
			Utility::drawBezierTube(tempSet, 4, 8, barRad, Texture::_blue);

			glTranslatef(0, 0, torsoWidth + barRad * 2);
			tempSet.clear(10);
			tempSet.addCoordinate(-0.25, 0.1, -barGap);
			tempSet.addCoordinate(-0.25, 0.1, barGap);
			tempSet.addCoordinate(-0.2, 0.15, barGap);
			Utility::drawBezierTube(tempSet, 4, 8, barRad, Texture::_blue);
		}
		glPopMatrix();

		// Yellow Bar on the bar of the shoulder (RIGHT)
		glPushMatrix();
		{
			Color::yellow();
			Lightning::yellowMaterial();

			float barRad = 0.02f;
			float barGap = 0.05f;
			float barGap2 = 0.06f;

			tempSet.clear(10);

			glTranslatef(0, 0, -torsoWidth / 2.0f - barRad - barGap);

			tempSet.addCoordinate(0.6, 0.225, 0);
			tempSet.addCoordinate(0.6, 0.225, -barGap2);
			tempSet.addCoordinate(0.55, 0.3, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(0.55, 0.3, -barGap2);
			tempSet.addCoordinate(0.47, 0.38, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(0.47, 0.38, -barGap2);
			tempSet.addCoordinate(0.45, 0.4, -barGap2);
			tempSet.addCoordinate(0.39, 0.395, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(0.39, 0.395, -barGap2);
			tempSet.addCoordinate(-0.09, 0.36, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(-0.09, 0.36, -barGap2);
			tempSet.addCoordinate(-0.15, 0.35, -barGap2);
			tempSet.addCoordinate(-0.16, 0.335, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(-0.16, 0.335, -barGap2);
			tempSet.addCoordinate(-0.24, 0.215, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(-0.24, 0.215, -barGap2);
			tempSet.addCoordinate(-0.25, 0.2, -barGap2);
			tempSet.addCoordinate(-0.25, 0.2, -barGap2 * 0.1f);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(-0.25, 0.2, -barGap2 * 0.1f);
			tempSet.addCoordinate(-0.25, 0.199, barGap * 2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
		}
		glPopMatrix();

		// Yellow Bar on the bar of the shoulder (LEFT)
		glPushMatrix();
		{
			Color::yellow();
			Lightning::yellowMaterial();

			float barRad = 0.02f;
			float barGap = 0.05f;
			float barGap2 = 0.06f;

			glTranslatef(0, 0, torsoWidth / 2.0f + barRad + barGap + barGap2 * 2);

			tempSet.clear(3);
			tempSet.addCoordinate(0.6, 0.225, -barGap2 * 2);
			tempSet.addCoordinate(0.6, 0.225, -barGap2);
			tempSet.addCoordinate(0.55, 0.3, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(0.55, 0.3, -barGap2);
			tempSet.addCoordinate(0.47, 0.38, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(0.47, 0.38, -barGap2);
			tempSet.addCoordinate(0.45, 0.4, -barGap2);
			tempSet.addCoordinate(0.39, 0.395, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(0.39, 0.395, -barGap2);
			tempSet.addCoordinate(-0.09, 0.36, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(-0.09, 0.36, -barGap2);
			tempSet.addCoordinate(-0.15, 0.35, -barGap2);
			tempSet.addCoordinate(-0.16, 0.335, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(-0.16, 0.335, -barGap2);
			tempSet.addCoordinate(-0.24, 0.215, -barGap2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(-0.24, 0.215, -barGap2);
			tempSet.addCoordinate(-0.25, 0.2, -barGap2);
			tempSet.addCoordinate(-0.25, 0.2, -barGap2 - (barGap2) * 0.9f);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
			tempSet.clear(3);
			tempSet.addCoordinate(-0.25, 0.2, -barGap2 - (barGap2) * 0.9f);
			tempSet.addCoordinate(-0.25, 0.199, (-barGap - barGap2) * 2);
			Utility::drawBezierTube(tempSet, 3, 8, barRad, Texture::_yellow);
		}
		glPopMatrix();

		// Two zipline cylinder containers on the back
		glPushMatrix();
		{
			glTranslatef(-0.5, 0.35, 0.3);
			glRotatef(20, 1, 0, 0);
			glRotatef(20, 0, 0, 1);
			zipLineBackTube();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(-0.5, 0.35, -0.3);
			glRotatef(-20, 1, 0, 0);
			glRotatef(20, 0, 0, 1);
			zipLineBackTube();
		}
		glPopMatrix();

		// Yellow lines connecting the two zipline cylinders
		glPushMatrix();
		{
			Color::yellow();
			Lightning::yellowMaterial();

			Texture::on();
			Texture::use(Texture::_yellowRope);
			gluQuadricTexture(torsoObj, GL_TRUE);

			// Modify GL_TEXTURE matrix to get repeating textures on gluCylinder
			glMatrixMode(GL_TEXTURE);
			glScalef(5, 1, 1);
			glRotatef(-90, 0, 0, 1);
			glMatrixMode(GL_MODELVIEW);

			float stringRad = 0.01f;
			float stringLen = 0.6f;
			glTranslatef(-0.67, 0.345, -0.3);
			gluCylinder(torsoObj, stringRad, stringRad, stringLen, 8, 40);
			glTranslatef(0.035, -0.1, 0);
			gluCylinder(torsoObj, stringRad, stringRad, stringLen, 8, 40);
			gluQuadricTexture(torsoObj, GL_FALSE);
			Texture::off();

			// Reset GL_TEXTURE matrix
			glMatrixMode(GL_TEXTURE);
			glLoadIdentity();
			glMatrixMode(GL_MODELVIEW);
		}
		glPopMatrix();

		tempSet.destroy();
		tempSet2.destroy();
	}
	glPopMatrix();
}

void Model::Waist() {
	if (waistObj == NULL) {
		waistObj = gluNewQuadric();
	}

	glPushMatrix();
	{
		glScalef(0.5, 0.5, 0.5);

		// Core ball as the waist
		glPushMatrix();
		{
			float waistSize = 0.2f;
			float waistExtrude = 0.02f;
			float waistGap = 0.07f;
			Color::lightGrey();
			Lightning::greyMaterial();

			gluSphere(waistObj, waistSize, 20, 16);

			Color::lightBlue2();
			Lightning::darkerBlueMaterial();

			glTranslatef(0, waistGap / 2.0f, 0);
			Utility::drawHemisphere(waistSize + waistExtrude, 20, 16, Texture::_blue);
			glTranslatef(0, -waistGap, 0);
			glRotatef(180, 1, 0, 0);
			Utility::drawHemisphere(waistSize + waistExtrude, 20, 16, Texture::_blue);
		}
		glPopMatrix();

		// Two Small Hinge balls that connects to the hip
		glPushMatrix();
		{
			float gap = 0.22f;
			float hingeRadius = 0.07f;

			Color::lightBlue();
			Lightning::blueMaterial();

			Texture::on();
			Texture::use(Texture::_blue);
			gluQuadricTexture(waistObj, GL_TRUE);
			glTranslatef(0, -0.175f, gap);
			gluSphere(waistObj, hingeRadius, 8, 8);

			glTranslatef(0, 0, -2.0f * gap);
			gluSphere(waistObj, hingeRadius, 8, 8);
			gluQuadricTexture(waistObj, GL_FALSE);
			Texture::off();
		}
		glPopMatrix();

		// Two bars that connect to the legs
		glPushMatrix();
		{
			float barWidth = 0.09f;
			float barRadius = 0.07f;
			float gap = 0.02f;
			float gap2 = 0.09f;
			float bar2Width = barWidth / 4.0f;
			Color::lightBlue();
			Lightning::blueMaterial();

			Texture::on();
			Texture::use(Texture::_blue);
			gluQuadricTexture(waistObj, GL_TRUE);
			glTranslatef(0, -0.38f, 0);
			glPushMatrix();
			{
				glPushMatrix();
				{
					glTranslatef(0, 0, gap);
					gluDisk(waistObj, 0, barRadius, 8, 1);
					gluCylinder(waistObj, barRadius, barRadius, barWidth, 8, 1);
					glTranslatef(0, 0, barWidth);
					gluDisk(waistObj, 0, barRadius, 8, 1);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(0, 0, -gap - barWidth);
					gluDisk(waistObj, 0, barRadius, 8, 1);
					gluCylinder(waistObj, barRadius, barRadius, barWidth, 8, 1);
					glTranslatef(0, 0, barWidth);
					gluDisk(waistObj, 0, barRadius, 8, 1);
				}
				glPopMatrix();
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslatef(0, 0.17, 0);
				glPushMatrix();
				{
					glTranslatef(0, 0, gap2);
					glRotatef(90, 1, 0, 0);
					gluCylinder(waistObj, bar2Width, bar2Width, 0.1, 4, 1);
				}
				glPopMatrix();
				glPushMatrix();
				{
					glTranslatef(0, 0, -gap2);
					glRotatef(90, 1, 0, 0);
					gluCylinder(waistObj, bar2Width, bar2Width, 0.1, 4, 1);
				}
				glPopMatrix();
			}
			glPopMatrix();
			gluQuadricTexture(waistObj, GL_FALSE);
			Texture::off();
		}
		glPopMatrix();

		// Connection to torso at the middle
		glPushMatrix();
		{
			Color::lightBlue();
			Lightning::blueMaterial();

			CoordinateSet torso(10);
			torso.addCoordinate(0, 0, 0);
			torso.addCoordinate(-0.3, 0, 0);
			torso.addCoordinate(-0.35, 0.08, 0);
			torso.addCoordinate(-0.31, 0.16, 0);
			torso.addCoordinate(-0.22, 0.19, 0);
			torso.addCoordinate(-0.2, 0.4, 0);
			torso.addCoordinate(0, 0.4, 0);
			glTranslatef(0.23f, 0.04f, -waistWidth / 2.0f);
			glScalef(2, 2, 1);

			float torsoCenter[3] = { -0.1, 0.1 , 0 };
			Utility::extrudePolygon(torso, torsoCenter, zAxis, waistWidth, TextureMap::allBlue());
			torso.destroy();
		}
		glPopMatrix();

		// Butt
		glPushMatrix();
		{
			float buttGap = 0.025f;
			// Left Butt
			glPushMatrix();
			{
				glTranslatef(-0.75, 0, buttGap);
				Buttock();

				glTranslatef(0.2, 0.05, 0.2 + buttWidth);
				glRotatef(180, 0, 1, 0);
				ButtockJet();
			}
			glPopMatrix();

			// Right Butt
			glPushMatrix();
			{
				glTranslatef(-0.75, 0, -buttGap - buttWidth * 2.0f);
				Buttock();

				glTranslatef(0.2, 0.05, -0.1);
				ButtockJet();
			}
			glPopMatrix();

			// Fuel pipes connected to both butts
			glPushMatrix();
			{
				float gap1 = 0.04f;
				CoordinateSet pipe1(10);
				pipe1.addCoordinate(0, 0, gap1);
				pipe1.addCoordinate(0.035, 0.025, gap1);
				pipe1.addCoordinate(0.035, 0.1, gap1);
				pipe1.addCoordinate(0.03, 0.1, 0);

				Color::lightGrey();
				Lightning::greyMaterial();

				glPushMatrix();
				{
					glTranslatef(-0.43, 0.075, 0.1);
					Utility::drawBezierTube(pipe1, 5, 6, 0.02);
				}
				glPopMatrix();

				for (int i = 0; i < pipe1.numberOfCoords; i++) {
					pipe1.zCoords[i] = -pipe1.zCoords[i];
				}

				glTranslatef(-0.43, 0.075, -0.1);
				Utility::drawBezierTube(pipe1, 5, 6, 0.02);

				pipe1.destroy();
			}
			glPopMatrix();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Model::Buttock() {
	glPushMatrix();
	{
		glScalef(2, 2, 2);

		// Main butt
		Color::lightBlue();
		Lightning::blueMaterial();

		CoordinateSet butt(10);
		butt.addCoordinate(0, 0, 0);
		butt.addCoordinate(0.1, 0.1, 0);
		butt.addCoordinate(0.22, -0.02, 0);
		butt.addCoordinate(0.22, -0.14, 0);
		butt.addCoordinate(0.09, -0.2, 0);
		butt.addCoordinate(0.09, -0.11, 0);
		float buttCenter[3] = { 0.11, 0, 0 };
		Utility::extrudePolygon(butt, buttCenter, zAxis, buttWidth, TextureMap::allBlue());

		float ventExtrude = 0.01f;
		// Vent on butt
		Color::darkGrey();
		Lightning::darkerGreyMaterial();

		CoordinateSet vent(10);
		vent.addCoordinate(0, 0, 0);
		vent.addCoordinate(0.1, 0.1, 0);
		vent.addCoordinate(0.1 - ventExtrude, 0.1 + ventExtrude, 0);
		vent.addCoordinate(-ventExtrude, ventExtrude, 0);
		glTranslatef(0.05, 0.05, buttWidth / 2.0f);
		glScalef(0.75, 0.75, 0.75);
		glTranslatef(-0.05, -0.05, -buttWidth / 2.0f);
		float ventCenter[3] = { 0.05 - ventExtrude / 2.0f, 0.05 + ventExtrude / 2.0f, 0 };

		Utility::extrudePolygon(vent, ventCenter, zAxis, buttWidth);

		// Vent texture
		Color::darkGrey();
		Lightning::blackVentMaterial();
		ventExtrude += 0.001f;
		Texture::on();
		Texture::use(Texture::_blackVent);
		glBegin(GL_QUADS);
		float ventTexCoords[4][3] = {
			{0.1 - ventExtrude, 0.1 + ventExtrude, 0},
			{0.1 - ventExtrude, 0.1 + ventExtrude, buttWidth},
			{-ventExtrude, ventExtrude, buttWidth},
			{-ventExtrude, ventExtrude, 0}
		};
		float normalTemp[3];
		glTexCoord2d(1, 1);
		Utility::drawVertex(ventTexCoords[0][0], ventTexCoords[0][1], ventTexCoords[0][2], ventCenter);
		glTexCoord2d(0, 1);
		Utility::drawVertex(ventTexCoords[1][0], ventTexCoords[1][1], ventTexCoords[1][2], ventCenter);
		glTexCoord2d(0, 0);
		Utility::drawVertex(ventTexCoords[2][0], ventTexCoords[2][1], ventTexCoords[2][2], ventCenter);
		glTexCoord2d(1, 0);
		Utility::drawVertex(ventTexCoords[3][0], ventTexCoords[3][1], ventTexCoords[3][2], ventCenter);
		glEnd();
		Texture::off();

		butt.destroy();
		vent.destroy();
	}
	glPopMatrix();
}

void Model::ButtockJet() {
	if (buttockJetObj == NULL) {
		buttockJetObj = gluNewQuadric();
	}

	glPushMatrix();
	{
		float jetRad = 0.05f;
		float jetExtrude = 0.02f;

		glScalef(2, 2, 2);

		glPushMatrix();
		{
			// Hemisphere-shaped jet
			Color::lightBlue2();
			Lightning::darkerBlueMaterial();

			glTranslatef(0, -jetRad, 0);
			glRotatef(22.5, 1, 0, 0);
			Utility::drawHemisphere(jetRad, 12, 6, Texture::_blue);

			// Vent
			Color::black();
			Lightning::blackMaterial();
			glRotatef(90, 1, 0, 0);
			gluCylinder(buttockJetObj, jetRad, jetRad, jetExtrude, 12, 1);
			glTranslatef(0, 0, jetExtrude);
			gluDisk(buttockJetObj, 0, jetRad, 12, 1);
		}
		glPopMatrix();

		// Support bar
		Color::lightBlue2();
		Lightning::darkerBlueMaterial();

		float barRad = 0.015f;
		glTranslatef(0, -barRad, 0);
		Texture::on();
		Texture::use(Texture::_blue);
		gluQuadricTexture(buttockJetObj, GL_TRUE);
		gluCylinder(buttockJetObj, barRad, barRad, 0.075f, 8, 1);
		gluQuadricTexture(buttockJetObj, GL_FALSE);
		Texture::off();
	}
	glPopMatrix();
}

void Model::LegUpper() {
	glPushMatrix();
	{
		// Upper leg
		float radius = 0.45 / 2.0f;
		float center[3] = { radius, -radius, 0 };
		int edges = 12;
		CoordinateSet circle = Utility::circleCoords(center, radius, edges, 30.0f, 235.0f);

		float lastCoordTemp[3];
		circle.getCoords(circle.numberOfCoords, lastCoordTemp);

		CoordinateSet line1(10);
		line1.addCoordinate(lastCoordTemp);
		line1.addCoordinate(0.22, -0.48, 0);

		line1.getCoords(line1.numberOfCoords, lastCoordTemp);
		CoordinateSet tempSet(10);
		tempSet.addCoordinate(lastCoordTemp);
		tempSet.addCoordinate(0.3, -0.7, 0);
		tempSet.addCoordinate(0.3, -0.9, 0);
		CoordinateSet line2 = Utility::bezierCurveCoords(tempSet, 4);

		CoordinateSet line3(10);
		line3.addCoordinate(0.4, -1.0, 0);
		line3.addCoordinate(0.5, -1.0, 0);
		line3.addCoordinate(0.5, -0.5, 0);
		line3.addCoordinate(0.5, -0.32, 0);

		circle.combineCoords(line1);
		circle.combineCoords(line2);
		circle.combineCoords(line3);

		glTranslatef(-center[0], -center[1], -legWidth / 2.0f);

		Color::lightBlue();
		Lightning::blueMaterial();
		float polygonCenter[3] = { 0.35, -0.35, 0 };
		Utility::extrudePolygon(circle, polygonCenter, zAxis, legWidth, TextureMap::allBlue());

		// Hinge
		Color::lightGrey();
		Lightning::greyMaterial();

		glPushMatrix();
		{
			float hingeThickness = 0.015f;
			float hingeCenter[3] = { 0.44, -1.01, -hingeThickness };
			float hingeRad = 0.06f;
			CoordinateSet hinge = Utility::circleCoords(hingeCenter, hingeRad, 16);

			Utility::extrudePolygon(hinge, hingeCenter, zAxis, hingeThickness, true, false);
			glTranslatef(0, 0, hingeThickness + legWidth);
			Utility::extrudePolygon(hinge, hingeCenter, zAxis, hingeThickness, false, true);

			hinge.destroy();
		}
		glPopMatrix();

		circle.destroy();
		line1.destroy();
		line2.destroy();
		line3.destroy();
		tempSet.destroy();
	}
	glPopMatrix();
}

void Model::LegLower() {
	float radius = 0.06;
	float center[3] = { 0.41, -radius, 0 };
	float lastCoordTemp[3];
	CoordinateSet tempCoordSet(10);
	CoordinateSet tempCoordSet2(10);

	Color::lightBlue();
	Lightning::blueMaterial();

	glPushMatrix();
	{
		glTranslatef(-center[0], -center[1], -legWidth / 2.0f);
		// Draw Leg part 1
		CoordinateSet legPart1(10);
		legPart1.addCoordinate(0.34, -0.22, 0);
		tempCoordSet.destroy();
		tempCoordSet = Utility::circleCoords(center, radius, 8, -60, 125);
		legPart1.combineCoords(tempCoordSet);
		legPart1.getCoords(legPart1.numberOfCoords, lastCoordTemp);
		tempCoordSet.clear(3);
		tempCoordSet.addCoordinate(lastCoordTemp);
		tempCoordSet.addCoordinate(0.22, -0.08, 0);
		tempCoordSet.addCoordinate(0.09, -0.08, 0);
		tempCoordSet2.destroy();
		tempCoordSet2 = Utility::bezierCurveCoords(tempCoordSet, 5);
		legPart1.combineCoords(tempCoordSet2);
		legPart1.addCoordinate(0.075, -0.15, 0);
		legPart1.addCoordinate(0.16, -0.19, 0);

		float legP1Center[3] = { 0.25, -0.15, 0 };
		Utility::extrudePolygon(legPart1, legP1Center, zAxis, legWidth, TextureMap::allBlue());

		// Draw Leg Part 2
		tempCoordSet.clear(3);
		tempCoordSet.addCoordinate(0.34, -0.75, 0);
		tempCoordSet.addCoordinate(0.22, -0.415, 0);
		tempCoordSet.addCoordinate(0.34, -0.22, 0);
		CoordinateSet longPart = Utility::bezierCurveCoords(tempCoordSet, 16);
		tempCoordSet.clear(3);
		tempCoordSet.addCoordinate(0.16, -0.19, 0);
		tempCoordSet.addCoordinate(0.12, -0.4, 0);
		tempCoordSet.addCoordinate(0.15, -0.62, 0);
		tempCoordSet2.destroy();
		tempCoordSet2 = Utility::bezierCurveCoords(tempCoordSet, 5);
		longPart.combineCoords(tempCoordSet2);
		tempCoordSet.clear(3);
		tempCoordSet.addCoordinate(0.21, -0.64, 0);
		tempCoordSet.addCoordinate(0.225, -0.7, 0);
		tempCoordSet.addCoordinate(0.27, -0.76, 0);
		tempCoordSet2.destroy();
		tempCoordSet2 = Utility::bezierCurveCoords(tempCoordSet, 4);
		longPart.combineCoords(tempCoordSet2);

		float longPartCenter[3] = { 0.23, -0.45, 0 };
		Utility::extrudePolygon(longPart, longPartCenter, zAxis, legWidth, TextureMap::allBlue());

		CoordinateSet smallLow(4);
		smallLow.addCoordinate(0.27, -0.76, 0);
		smallLow.addCoordinate(0.265, -0.8, 0);
		smallLow.addCoordinate(0.325, -0.8, 0);
		smallLow.addCoordinate(0.34, -0.75, 0);
		float smallLowCenter[3] = { 0.3, -0.78, 0 };
		Utility::extrudePolygon(smallLow, smallLowCenter, zAxis, legWidth, TextureMap::allBlue());

		// Red spring at the bottom
		if (legLowerSpring == NULL) {
			legLowerSpring = gluNewQuadric();
		}
		Color::red();
		Lightning::windRedMaterial();

		{
			glTranslatef(0.295, -0.8, legWidth / 2.0f);
			glRotatef(90, 1, 0, 0);
			gluCylinder(legLowerSpring, 0.018f, 0.018f, 0.08f, 8, 1);
		}

		legPart1.destroy();
		longPart.destroy();
		smallLow.destroy();
	}
	glPopMatrix();

	tempCoordSet.destroy();
	tempCoordSet2.destroy();
}

void Model::LegPlate(float r) {
	float feetWidth = 0.1f;

	if (legPlateObj == NULL) {
		legPlateObj = gluNewQuadric();
	}

	glPushMatrix();
	{
		glTranslatef(0, 0, -legWidth / 2.0f);

		// Leg plate
		glPushMatrix();
		{
			glRotatef(r, 0, 0, 1);
			Color::lightGrey();
			Lightning::greyMaterial();

			CoordinateSet plate(10);
			plate.addCoordinate(0, 0, 0);
			plate.addCoordinate(0, -0.08, 0);
			plate.addCoordinate(0.275, -0.08, 0);
			plate.addCoordinate(0.275, -0.04, 0);
			plate.addCoordinate(0.14, -0.03, 0);
			float plateCenter[3] = { 0.14, -0.06, 0 };
			Utility::extrudePolygon(plate, plateCenter, zAxis, feetWidth);
			plate.destroy();
		}
		glPopMatrix();

		// Ankle
		glPushMatrix();
		{
			float extrusion = 0.01f;
			Color::black();
			Lightning::blackMaterial();

			float ankleCenter[3] = { 0, 0, 0 };
			CoordinateSet ankle = Utility::circleCoords(ankleCenter, 0.025f, 12);
			glTranslatef(0, 0, -extrusion);
			Utility::extrudePolygon(ankle, ankleCenter, zAxis, feetWidth + extrusion * 2);
			ankle.destroy();
		}
		glPopMatrix();

		// Support bar
		glPushMatrix();
		{
			float inset = 0.01f;
			float barWidth = 0.015f;
			float barSmallCircle[3] = { -0.05, 0.05, 0 };
			float barSmallCircleRadius = 0.015f;
			CoordinateSet bar(10);
			bar.addCoordinate(0, 0, 0);
			bar.addCoordinate(0, 0.02, 0);
			CoordinateSet tempCoordSet = Utility::circleCoords(barSmallCircle, barSmallCircleRadius, 8, 80.0f, 260.0f);
			tempCoordSet.reverse();
			bar.combineCoords(tempCoordSet);
			bar.addCoordinate(0.03, 0.035, 0);
			bar.addCoordinate(0.035, 0.03, 0);
			bar.addCoordinate(0.035, -0.01, 0);
			float barCenter[3] = { 0.025, 0.015, 0 };

			Color::lightGrey();
			Lightning::greyMaterial();

			// Right support plate
			glPushMatrix();
			{
				glTranslatef(0, 0, inset);
				Utility::extrudePolygon(bar, barCenter, zAxis, barWidth);
			}
			glPopMatrix();

			// Left support plate
			glPushMatrix();
			{
				glTranslatef(0, 0, feetWidth - inset - barWidth);
				Utility::extrudePolygon(bar, barCenter, zAxis, barWidth);
			}
			glPopMatrix();

			// Horizontal support bar
			float supportBarRadius = 0.008f;
			glPushMatrix();
			{
				Color::gold();
				Lightning::goldMaterial();
				glTranslatef(barSmallCircle[0], barSmallCircle[1], barSmallCircle[2] + inset + barWidth);
				gluCylinder(legPlateObj, supportBarRadius, supportBarRadius, feetWidth - inset * 2 - barWidth * 2, 6, 1);
			}
			glPopMatrix();

			// Large support cylinder
			glPushMatrix();
			{
				Color::black();
				Lightning::blackMaterial();

				glTranslatef(barSmallCircle[0], barSmallCircle[1], barSmallCircle[2] + feetWidth / 2.0f);
				glRotatef(-15, 0, 0, 1);
				glTranslatef(0, supportBarRadius, 0);
				glRotatef(-90, 1, 0, 0);
				gluCylinder(legPlateObj, 0.025, 0.025, 0.16, 12, 1);
			}
			glPopMatrix();

			bar.destroy();
			tempCoordSet.destroy();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Model::HipJoint() {
	if (hipBallObj == NULL) {
		hipBallObj = gluNewQuadric();
	}

	glPushMatrix();
	{
		float jointSize = legWidth * 1.25f;
		float r1 = 0.18f, r2 = 0.12f;
		Color::lightGrey();
		Lightning::greyMaterial();

		gluCylinder(hipBallObj, r1, r2, jointSize, 16, 1);

		glTranslatef(0, 0, jointSize);
		gluDisk(hipBallObj, 0, r2, 16, 1);
	}
	glPopMatrix();
}

void Model::ArmUpper() {
	if (armUpperObj == NULL) {
		armUpperObj = gluNewQuadric();
	}
	glPushMatrix();
	{
		glRotatef(90, 0, 0, 1);
		Color::lightBlue();
		Lightning::blueMaterial();

		CoordinateSet arm(10);
		arm.addCoordinate(0, -0.17f, 0);
		arm.addCoordinate(-0.035f, -0.325f, 0);
		arm.addCoordinate(0.25f, -0.4f, 0);
		arm.addCoordinate(0.66f, -0.475f, 0);
		arm.addCoordinate(0.775f, -0.39f, 0);
		arm.addCoordinate(0.98f, -0.33f, 0);
		arm.addCoordinate(1.02f, -0.14f, 0);
		arm.addCoordinate(0.95f, -0.11f, 0);
		arm.addCoordinate(0.725f, -0.1f, 0);
		arm.addCoordinate(0.66f, -0.065f, 0);
		arm.addCoordinate(0.35f, 0, 0);
		arm.addCoordinate(0.28f, -0.025f, 0);
		arm.addCoordinate(0.2f, -0.1f, 0);
		arm.addCoordinate(0.05f, -0.135f, 0);
		float armCenter[3] = { 0.5f, -0.3f, 0 };
		Utility::extrudePolygon(arm, armCenter, zAxis, armWidth, TextureMap::allBlue());

		arm.destroy();
	}
	glPopMatrix();
}

void Model::ArmLower() {
	if (armLowerObj == NULL) {
		armLowerObj = gluNewQuadric();
	}

	// Hinge
	glPushMatrix();
	{
		float radius1 = 0.1f;
		float radius2 = 0.06f;
		float sideExtrude = 0.015f;
		// Large hinge cylinder
		Color::lightGrey();
		Lightning::greyMaterial();

		gluCylinder(armLowerObj, radius1, radius1, armWidth, 12, 1);
		gluDisk(armLowerObj, 0, radius1, 12, 1);
		glPushMatrix();
		{
			glTranslatef(0, 0, armWidth);
			gluDisk(armLowerObj, 0, radius1, 12, 1);
		}
		glPopMatrix();

		//Smaller side cylinders
		Color::darkGrey();
		Lightning::darkerGreyMaterial();

		glPushMatrix();
		{
			glTranslatef(0, 0, -sideExtrude);
			gluCylinder(armLowerObj, radius2, radius2, sideExtrude, 12, 1);
			gluDisk(armLowerObj, 0, radius2, 12, 1);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0, 0, armWidth);
			gluCylinder(armLowerObj, radius2, radius2, sideExtrude, 12, 1);
			glTranslatef(0, 0, sideExtrude);
			gluDisk(armLowerObj, 0, radius2, 12, 1);
		}
		glPopMatrix();
	}
	glPopMatrix();

	// Main arm
	glPushMatrix();
	{
		glTranslatef(0.1, 0.1, 0);
		Color::lightBlue();
		Lightning::blueMaterial();

		// Main body
		CoordinateSet arm(10);
		arm.addCoordinate(0.75, -0.2, 0);
		arm.addCoordinate(0.8, -0.175, 0);
		arm.addCoordinate(0.8, -0.025, 0);
		arm.addCoordinate(0.75, 0, 0);
		arm.addCoordinate(0.125, 0, 0);
		arm.addCoordinate(0.02, -0.08, 0);
		arm.addCoordinate(-0.01, -0.165, 0);
		arm.addCoordinate(-0.06, -0.2, 0);
		float armCenter[3] = { 0.4, -0.1, 0 };
		Utility::extrudePolygon(arm, armCenter, zAxis, armWidth, TextureMap::allBlue());

		// Hinge cover
		CoordinateSet cover(4);
		cover.addCoordinate(-0.2, -0.2, 0);
		cover.addCoordinate(-0.05, -0.25, 0);
		cover.addCoordinate(0.05, -0.25, 0);
		cover.addCoordinate(0.1, -0.2, 0);
		float coverCenter[3] = { -0.1, -0.22, 0 };
		Utility::extrudePolygon(cover, coverCenter, zAxis, armWidth, TextureMap::allBlue());

		// Joint at the end
		glPushMatrix();
		{
			Color::lightGrey();
			Lightning::greyMaterial();

			float jointRadius = 0.05;
			float plateRadius = 0.065;
			float plateExtrude = 0.01f;

			// Ball joint
			glTranslatef(0.8, -0.1, armWidth / 2.0f);
			glPushMatrix();
			{
				glTranslatef(0.025, 0, 0);
				gluSphere(armLowerObj, jointRadius, 12, 12);
			}
			glPopMatrix();

			// Joint plate
			glPushMatrix();
			{
				glRotatef(90, 0, 1, 0);
				gluCylinder(armLowerObj, plateRadius, plateRadius, plateExtrude, 8, 1);
			}
			glPopMatrix();
			glPushMatrix();
			{
				glTranslatef(plateExtrude, 0, 0);
				glRotatef(90, 0, 1, 0);
				gluDisk(armLowerObj, 0, plateRadius, 8, 1);
			}
			glPopMatrix();
		}
		glPopMatrix();

		arm.destroy();
		cover.destroy();
	}
	glPopMatrix();
}

void Model::Shoulder() {
	float shoulderRad = 0.12f;
	if (shoulderObj == NULL) {
		shoulderObj = gluNewQuadric();
	}

	// Shoulder Hinge
	glPushMatrix();
	{
		Color::lightGrey();
		Lightning::greyMaterial();

		glRotatef(90, 0, 0, 1);
		glTranslatef(1.08f, -0.25f, armWidth / 2.0f);
		gluSphere(shoulderObj, shoulderRad, 8, 8);
	}
	glPopMatrix();
}

void Model::RightHand() {
	glPushMatrix();
	{
		glTranslatef(0, 0.1, 0);

		// Palm
		glPushMatrix();
		{
			Color::lightBlue();
			Lightning::blueMaterial();

			CoordinateSet palm(10);
			palm.addCoordinate(0, -0.02, 0);
			palm.addCoordinate(0, -0.15, 0);
			palm.addCoordinate(-0.01, -0.2, 0);
			palm.addCoordinate(0.14, -0.21, 0);
			palm.addCoordinate(0.25, -0.205, 0);
			palm.addCoordinate(0.285, -0.1, 0);
			palm.addCoordinate(0.225, 0.04, 0);
			float palmCenter[3] = { 0.1, -0.1, 0 };
			Utility::extrudePolygon(palm, palmCenter, zAxis, handThickness, TextureMap::allBlue());

			float palmDetailExtrude = 0.01f;
			glTranslatef(0.15, -0.1, -palmDetailExtrude);
			glScalef(0.65, 0.65, 1);
			glTranslatef(-0.15, 0.1, 0);
			Utility::extrudePolygon(palm, palmCenter, zAxis, palmDetailExtrude, TextureMap::allBlue(), true, false);

			palm.destroy();
		}
		glPopMatrix();

		// Thumb
		glPushMatrix();
		{
			glTranslatef(0, -0.08, 0.05);
			glRotatef(80, 0, 0, 1);
			glRotatef(50, 1, 0, 0);
			Finger(RFingerRot[0][0], RFingerRot[0][1], RFingerRot[0][2]);
		}
		glPopMatrix();

		// Index Finger
		glPushMatrix();
		{
			glTranslatef(0.2, 0.02, 0.005);
			glRotatef(15.5, 0, 0, 1);
			Finger(RFingerRot[1][0], RFingerRot[1][1], RFingerRot[1][2]);
		}
		glPopMatrix();

		// Middle Finger
		glPushMatrix();
		{
			glTranslatef(0.235, -0.035, 0.005);
			glRotatef(14, 0, 0, 1);
			Finger(RFingerRot[2][0], RFingerRot[2][1], RFingerRot[2][2]);
		}
		glPopMatrix();

		// Ring Finger
		glPushMatrix();
		{
			glTranslatef(0.23, -0.095, 0.005);
			glRotatef(10, 0, 0, 1);
			Finger(RFingerRot[3][0], RFingerRot[3][1], RFingerRot[3][2]);
		}
		glPopMatrix();

		// Pinky Finger
		glPushMatrix();
		{
			glTranslatef(0.2, -0.155, 0.005);
			glRotatef(5, 0, 0, 1);
			Finger(RFingerRot[4][0], RFingerRot[4][1], RFingerRot[4][2]);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Model::LeftHand() {
	glPushMatrix();
	{
		glTranslatef(0, 0.1, 0);

		// Palm
		glPushMatrix();
		{
			Color::lightBlue();
			Lightning::blueMaterial();

			CoordinateSet palm(10);
			palm.addCoordinate(0, -0.02, 0);
			palm.addCoordinate(0, -0.15, 0);
			palm.addCoordinate(0.01, -0.2, 0);
			palm.addCoordinate(-0.14, -0.21, 0);
			palm.addCoordinate(-0.25, -0.205, 0);
			palm.addCoordinate(-0.285, -0.1, 0);
			palm.addCoordinate(-0.225, 0.04, 0);
			float palmCenter[3] = { -0.1, -0.1, 0 };
			Utility::extrudePolygon(palm, palmCenter, zAxis, handThickness, TextureMap::allBlue());

			float palmDetailExtrude = 0.01f;
			glTranslatef(-0.15, -0.1, -palmDetailExtrude);
			glScalef(-0.65, 0.65, 1);
			glTranslatef(0.15, 0.1, 0);
			Utility::extrudePolygon(palm, palmCenter, zAxis, palmDetailExtrude, TextureMap::allBlue(), true, false);

			palm.destroy();
		}
		glPopMatrix();

		glTranslatef(-0.01f, -0.05f, 0);

		// Thumb
		glPushMatrix();
		{
			glTranslatef(-0.02, -0.035, 0.01);
			glRotatef(-80 + 180, 0, 0, 1);
			glRotatef(-50, 1, 0, 0);
			Finger(LFingerRot[0][0], LFingerRot[0][1], LFingerRot[0][2]);
		}
		glPopMatrix();

		// Index Finger
		glPushMatrix();
		{
			glTranslatef(-0.2, 0.02, 0.005);
			glRotatef(-15.5 + 180, 0, 0, 1);
			Finger(LFingerRot[1][0], LFingerRot[1][1], LFingerRot[1][2]);
		}
		glPopMatrix();

		// Middle Finger
		glPushMatrix();
		{
			glTranslatef(-0.235, -0.035, 0.005);
			glRotatef(-14 + 180, 0, 0, 1);
			Finger(LFingerRot[2][0], LFingerRot[2][1], LFingerRot[2][2]);
		}
		glPopMatrix();

		// Ring Finger
		glPushMatrix();
		{
			glTranslatef(-0.23, -0.095, 0.005);
			glRotatef(-10 + 180, 0, 0, 1);
			Finger(LFingerRot[3][0], LFingerRot[3][1], LFingerRot[3][2]);
		}
		glPopMatrix();

		// Pinky Finger
		glPushMatrix();
		{
			glTranslatef(-0.145, -0.155, 0.005);
			glRotatef(-5 + 180, 0, 0, 1);
			Finger(LFingerRot[4][0], LFingerRot[4][1], LFingerRot[4][2]);
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Model::Finger(float r1, float r2, float r3) {
	if (fingerObj == NULL) {
		fingerObj = gluNewQuadric();
	}

	float segLength[3] = { 0.7, 0.55, 0.48 };
	float fingerWidth = 0.25f;
	float fingerThickness = 0.2f;
	float jointRadius = fingerThickness / 1.9f;
	float jointExtrude = 0.025f;
	float jointGap = jointRadius * 0.1f;

	glScalef(0.2, 0.2, 0.2);
	glRotatef(r1, 0, -1, 0);
	// Segment #1
	glPushMatrix();
	{
		// Finger segment
		Color::lightBlue();
		Lightning::blueMaterial();

		CoordinateSet seg(4);
		seg.addCoordinate(0, 0, 0);
		seg.addCoordinate(0, -fingerWidth, 0);
		seg.addCoordinate(segLength[0], -fingerWidth, 0);
		seg.addCoordinate(segLength[0], 0, 0);
		float segCenter[3] = { segLength[0] / 2.0f, -fingerWidth / 2.0f, 0 };

		Utility::extrudePolygon(seg, segCenter, zAxis, fingerThickness, TextureMap::allBlue());

		// Finger deco
		float indent = 0.035f;
		float decoExtrude = 0.01f;
		float decoWidth = 0.035f;
		float decoWidth2 = 0.1f;
		float middleIndent = 0.2f;
		glTranslatef(0, 0, -decoExtrude);
		glPushMatrix();
		Color::red();
		Lightning::windRedMaterial();
		{
			CoordinateSet deco(10);
			deco.addCoordinate(indent, -indent, 0);
			deco.addCoordinate(indent, -indent - decoWidth, 0);
			deco.addCoordinate(segLength[0] - indent, -indent - decoWidth, 0);
			deco.addCoordinate(segLength[0] - indent, -indent, 0);
			float decoCenter[3] = { segLength[0] / 2.0f, -indent - decoWidth / 2.0f, 0 };
			Utility::extrudePolygon(deco, decoCenter, zAxis, decoExtrude);
			deco.destroy();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0, -fingerWidth + indent * 2 + decoWidth, 0);
			CoordinateSet deco(10);
			deco.addCoordinate(indent, -indent, 0);
			deco.addCoordinate(indent, -indent - decoWidth, 0);
			deco.addCoordinate(segLength[0] - indent, -indent - decoWidth, 0);
			deco.addCoordinate(segLength[0] - indent, -indent, 0);
			float decoCenter[3] = { segLength[0] / 2.0f, -indent - decoWidth / 2.0f, 0 };
			Utility::extrudePolygon(deco, decoCenter, zAxis, decoExtrude);
			deco.destroy();
		}
		glPopMatrix();
		glPushMatrix();
		{
			CoordinateSet deco(10);
			deco.addCoordinate(middleIndent, -indent - decoWidth, 0);
			deco.addCoordinate(middleIndent, -fingerWidth + indent + decoWidth, 0);
			deco.addCoordinate(middleIndent + decoWidth2, -fingerWidth + indent + decoWidth, 0);
			deco.addCoordinate(middleIndent + decoWidth2, -indent - decoWidth, 0);
			float decoCenter[3] = { middleIndent + (decoWidth2 / 2.0f), (-fingerWidth / 2.0f), 0 };
			Utility::extrudePolygon(deco, decoCenter, zAxis, decoExtrude);
			deco.destroy();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(segLength[0], -fingerWidth, 0);
			glRotatef(180, 0, 0, 1);
			CoordinateSet deco(10);
			deco.addCoordinate(middleIndent, -indent - decoWidth, 0);
			deco.addCoordinate(middleIndent, -fingerWidth + indent + decoWidth, 0);
			deco.addCoordinate(middleIndent + decoWidth2, -fingerWidth + indent + decoWidth, 0);
			deco.addCoordinate(middleIndent + decoWidth2, -indent - decoWidth, 0);
			float decoCenter[3] = { middleIndent + (decoWidth2 / 2.0f), (-fingerWidth / 2.0f), 0 };
			Utility::extrudePolygon(deco, decoCenter, zAxis, decoExtrude);
			deco.destroy();
		}
		glPopMatrix();

		seg.destroy();
	}
	glPopMatrix();

	glTranslatef(segLength[0] + jointGap, 0, 0);
	glRotatef(r2, 0, -1, 0);

	// Joint
	Color::lightGrey();
	Lightning::greyMaterial();

	glPushMatrix();
	{
		glTranslatef(0, jointExtrude, fingerThickness - jointRadius / 2.0f);
		glRotatef(90, 1, 0, 0);
		gluCylinder(fingerObj, jointRadius, jointRadius, fingerWidth + (jointExtrude * 2), 8, 1);
		gluDisk(fingerObj, 0, jointRadius, 8, 1);
		glTranslatef(0, 0, fingerWidth + jointExtrude * 2);
		gluDisk(fingerObj, 0, jointRadius, 8, 1);
	}
	glPopMatrix();

	glTranslatef(jointGap, 0, 0);

	glPushMatrix();
	// Segment #2 
	{
		Color::lightBlue();
		Lightning::blueMaterial();

		CoordinateSet seg(4);
		seg.addCoordinate(0, 0, 0);
		seg.addCoordinate(0, -fingerWidth, 0);
		seg.addCoordinate(segLength[1], -fingerWidth, 0);
		seg.addCoordinate(segLength[1], 0, 0);
		float segCenter[3] = { segLength[1] / 2.0f, -fingerWidth / 2.0f , 0 };
		Utility::extrudePolygon(seg, segCenter, zAxis, fingerThickness, TextureMap::allBlue());

		float indent = 0.08f;
		float decoExtrude = 0.01f;
		// Finger Deco
		glTranslatef(0, 0, -decoExtrude);
		Color::black();
		Lightning::blackMaterial();

		CoordinateSet deco(10);
		deco.addCoordinate(indent, -indent, 0);
		deco.addCoordinate(indent, -fingerWidth + indent, 0);
		deco.addCoordinate(segLength[1] - indent, -fingerWidth + indent, 0);
		deco.addCoordinate(segLength[1] - indent, -indent, 0);
		float decoCenter[3] = { segLength[1] / 2.0f, -fingerWidth / 2.0f, 0 };
		Utility::extrudePolygon(deco, decoCenter, zAxis, decoExtrude);

		seg.destroy();
		deco.destroy();
	}
	glPopMatrix();

	glTranslatef(segLength[1] + jointGap, 0, 0);
	glRotatef(r3, 0, -1, 0);

	// Joint
	glPushMatrix();
	{
		Color::lightGrey();
		Lightning::greyMaterial();

		glTranslatef(0, jointExtrude, fingerThickness - jointRadius / 2.0f);
		glRotatef(90, 1, 0, 0);
		gluCylinder(fingerObj, jointRadius, jointRadius, (double)fingerWidth + (jointExtrude * 2), 8, 1);
		gluDisk(fingerObj, 0, jointRadius, 8, 1);
		glTranslatef(0, 0, fingerWidth + jointExtrude * 2);
		gluDisk(fingerObj, 0, jointRadius, 8, 1);
	}
	glPopMatrix();

	glTranslatef(jointGap, 0, 0);

	// Segment #3
	glPushMatrix();
	{
		Color::lightBlue();
		Lightning::blueMaterial();

		CoordinateSet seg(4);
		seg.addCoordinate(0, 0, 0);
		seg.addCoordinate(0, -fingerWidth, 0);
		seg.addCoordinate(segLength[2], -fingerWidth, 0);
		seg.addCoordinate(segLength[2], 0, 0);
		float segCenter[3] = { segLength[2] / 2.0f, -fingerWidth / 2.0f , 0 };
		Utility::extrudePolygon(seg, segCenter, zAxis, fingerThickness, TextureMap::allBlue());
		seg.destroy();
	}
	glPopMatrix();
}

void drawZipLineTubeBlackDecoPiece() {
	glPushMatrix();
	{
		Color::black();
		Lightning::blackMaterial();

		float center[3] = { 0, 0, 0 };
		float radius1 = 0.05f, radius2 = 0.1005f;
		float decoThickness = 0.05f;
		CoordinateSet temp = Utility::circleCoords(center, radius1, 4, 0, 30);
		CoordinateSet temp2 = Utility::circleCoords(center, radius2, 4, 0, 30);
		temp.reverse();
		temp2.combineCoords(temp);

		// 0.2588 = sin(15deg)
		float polygonCenter[3] = { radius2 - radius1, 0.2588f * (radius2 - radius1), 0 };
		Utility::extrudePolygon(temp2, polygonCenter, zAxis, decoThickness);

		temp.destroy();
		temp2.destroy();
	}
	glPopMatrix();
}

void Model::zipLineTube() {
	if (zipLineTubeObj == NULL) {
		zipLineTubeObj = gluNewQuadric();
	}

	glPushMatrix();
	{
		Color::lightBlue();
		Lightning::blueMaterial();

		float tubeRadius = 0.1f;
		float tubeLength = 0.7f;
		float tubeRedDecoRadius = 0.05f;

		// Tube
		Texture::on();
		Texture::use(Texture::_blue);
		gluQuadricTexture(zipLineTubeObj, GL_TRUE);
		glTranslatef(0, 0, -tubeLength / 2.0f);
		gluCylinder(zipLineTubeObj, tubeRadius, tubeRadius, tubeLength, 24, 12);
		gluQuadricTexture(zipLineTubeObj, GL_FALSE);
		Texture::off();

		// End cover with red deco
		glPushMatrix();
		{
			Color::lightBlue();
			Lightning::blueMaterial();

			Texture::on();
			Texture::use(Texture::_blue);
			gluQuadricTexture(zipLineTubeObj, GL_TRUE);
			gluDisk(zipLineTubeObj, 0, tubeRadius, 24, 4);
			gluQuadricTexture(zipLineTubeObj, GL_FALSE);
			Texture::off();

			Color::red();
			Lightning::windRedMaterial();

			glTranslatef(0, 0, -0.0001f);
			gluDisk(zipLineTubeObj, 0, tubeRedDecoRadius, 24, 4);
		}
		glPopMatrix();

		// End cover with red deco
		glPushMatrix();
		{
			Color::lightBlue();
			Lightning::blueMaterial();

			Texture::on();
			Texture::use(Texture::_blue);
			gluQuadricTexture(zipLineTubeObj, GL_TRUE);
			glTranslatef(0, 0, tubeLength);
			gluDisk(zipLineTubeObj, 0, tubeRadius, 24, 4);
			gluQuadricTexture(zipLineTubeObj, GL_FALSE);
			Texture::off();

			Color::red();
			Lightning::windRedMaterial();

			glTranslatef(0, 0, 0.0001f);
			gluDisk(zipLineTubeObj, 0, tubeRedDecoRadius, 24, 4);
		}
		glPopMatrix();

		// Gray strips
		float stripIndent = 0.05f;
		float stripWidth = 0.025f;
		float stripRadius = tubeRadius + 0.0001f;
		glPushMatrix();
		{
			Color::lightGrey();
			Lightning::greyMaterial();

			glTranslatef(0, 0, stripIndent);
			gluCylinder(zipLineTubeObj, stripRadius, stripRadius, stripWidth, 24, 12);
			glTranslatef(0, 0, tubeLength - 2 * stripIndent - stripWidth);
			gluCylinder(zipLineTubeObj, stripRadius, stripRadius, stripWidth, 24, 12);
		}
		glPopMatrix();

		// Black stripes that looks like casino token borders on both ends
		glPushMatrix();
		{
			glTranslatef(0, 0, -0.0001f);
			for (int i = 0; i < 6; i++) {
				glPushMatrix();
				{
					glRotatef((float)i * 60.0f, 0, 0, 1);
					drawZipLineTubeBlackDecoPiece();
				}
				glPopMatrix();
			}
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0, 0, tubeLength - 0.05f + 0.0001f);
			for (int i = 0; i < 6; i++) {
				glPushMatrix();
				{
					glRotatef((float)i * 60.0f, 0, 0, 1);
					drawZipLineTubeBlackDecoPiece();
				}
				glPopMatrix();
			}
		}
		glPopMatrix();
	}
	glPopMatrix();

	// Connection to arm
	glPushMatrix();
	{
		glTranslatef(0, -0.1, 0);
		glScalef(1.5, 1.5, 3);
		Color::lightBlue2();
		Lightning::darkerBlueMaterial();

		CoordinateSet temp(10);
		temp.addCoordinate(0, 0.05, -0.01);
		temp.addCoordinate(0, 0.03, -0.015);
		temp.addCoordinate(0, 0.03, -0.03);
		temp.addCoordinate(0, 0, -0.05);
		temp.addCoordinate(0, 0, 0.05);
		temp.addCoordinate(0, 0.03, 0.03);
		temp.addCoordinate(0, 0.03, 0.015);
		temp.addCoordinate(0, 0.05, 0.01);
		float faceCenter[3] = { 0, 0.015, 0 };
		Utility::extrudePolygon(temp, faceCenter, xAxis, 0.1, TextureMap::allBlue());
		temp.destroy();

		// Small red bump
		Color::red();
		Lightning::windRedMaterial();

		glTranslatef(0.06, 0.045, 0);
		glRotatef(90, 0, 1, 0);
		gluCylinder(zipLineTubeObj, 0.01, 0.01, 0.03, 6, 3);
	}
	glPopMatrix();
}


void Model::zipLineBackTube() {
	if (zipLineBackTubeObj == NULL) {
		zipLineBackTubeObj = gluNewQuadric();
	}

	glPushMatrix();
	{
		float containerRadius = 0.12f, containerHeight = 0.5f;
		float lidRadius = containerRadius + 0.01f, lidHeight = 0.05f;

		glTranslatef(0, containerHeight / 2.0f, 0);
		glRotatef(-90, 0, 1, 0);
		glRotatef(-90, 1, 0, 0);

		Color::lightGrey();
		Lightning::greyMaterial();

		// Tube itself
		Texture::on();
		Texture::use(Texture::_winder);
		gluQuadricTexture(zipLineBackTubeObj, GL_TRUE);
		glTranslatef(0, 0, -containerHeight / 2.0f);
		gluCylinder(zipLineBackTubeObj, containerRadius, containerRadius, containerHeight, 24, 12);

		gluQuadricTexture(zipLineBackTubeObj, GL_FALSE);
		Texture::off();

		// Lids
		Color::lightBlue();
		Lightning::blueMaterial();

		Texture::on();
		Texture::use(Texture::_blue);
		gluQuadricTexture(zipLineBackTubeObj, GL_TRUE);
		glPushMatrix();
		{
			gluDisk(zipLineBackTubeObj, 0, lidRadius, 24, 6);
			glTranslatef(0, 0, -lidHeight);
			gluCylinder(zipLineBackTubeObj, lidRadius, lidRadius, lidHeight, 24, 1);
			gluDisk(zipLineBackTubeObj, 0, lidRadius, 24, 6);
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0, 0, containerHeight);
			gluDisk(zipLineBackTubeObj, 0, lidRadius, 24, 6);
			gluCylinder(zipLineBackTubeObj, lidRadius, lidRadius, lidHeight, 24, 1);
			glTranslatef(0, 0, lidHeight);
			gluDisk(zipLineBackTubeObj, 0, lidRadius, 24, 6);
		}
		glPopMatrix();
		gluQuadricTexture(zipLineBackTubeObj, GL_FALSE);
		Texture::off();

		// Handle
		float handleExtrude = 0.03f, handleExtrudeTopBottom = 0.1f;
		float handleWidthHalf = 0.035f;
		glPushMatrix();
		{
			glTranslatef(0, 0, -handleExtrudeTopBottom);
			CoordinateSet temp(10);
			temp.addCoordinate(-handleWidthHalf, 0, 0);
			temp.addCoordinate(handleWidthHalf, 0, 0);
			temp.addCoordinate(handleWidthHalf, containerRadius + handleExtrude, 0);
			temp.addCoordinate(-handleWidthHalf, containerRadius + handleExtrude, 0);
			float tempCenter[3] = { 0, containerRadius / 2.0f, 0 };
			Utility::extrudePolygon(temp, tempCenter, zAxis, containerHeight + handleExtrudeTopBottom * 2, TextureMap::allBlue());
			temp.destroy();
		}
		glPopMatrix();

		// Connecter from bottom
		glPushMatrix();
		{
			Texture::on();
			Texture::use(Texture::_blue);
			gluQuadricTexture(zipLineBackTubeObj, GL_TRUE);
			glRotatef(180, 1, 0, 0);
			gluCylinder(zipLineBackTubeObj, 0.035, 0.035, containerHeight * 0.5, 8, 8);
			gluQuadricTexture(zipLineBackTubeObj, GL_FALSE);
			Texture::off();
		}
		glPopMatrix();

		// Line Cycler
		float cyclerConnectionRad = 0.025f;
		float cyclerConnectionLen = 0.035f;
		float cyclerRadius = 0.06f;
		float cyclerThickness = 0.02f;
		glPushMatrix();
		{
			Color::lightBlue2();
			Lightning::darkerBlueMaterial();

			Texture::on();
			Texture::use(Texture::_blue);
			gluQuadricTexture(zipLineBackTubeObj, GL_TRUE);
			glTranslatef(0, -containerRadius, 0);
			glPushMatrix();
			{
				glRotatef(90, 1, 0, 0);
				gluCylinder(zipLineBackTubeObj, cyclerConnectionRad, cyclerConnectionRad, cyclerConnectionLen, 8, 2);
			}
			glPopMatrix();

			glTranslatef(0, -cyclerConnectionLen, 0);
			glPushMatrix();
			{
				Color::lightBlue2();
				Lightning::darkerBlueMaterial();

				glRotatef(90, 1, 0, 0);
				gluDisk(zipLineBackTubeObj, 0, cyclerRadius, 12, 3);
				gluQuadricTexture(zipLineBackTubeObj, GL_FALSE);
				Texture::off();

				Color::yellow();
				Lightning::yellowMaterial();

				glMatrixMode(GL_TEXTURE);
				glScalef(3, 0.5, 1);
				glMatrixMode(GL_MODELVIEW);

				Texture::on();
				Texture::use(Texture::_yellowRope);
				gluQuadricTexture(zipLineBackTubeObj, GL_TRUE);
				gluCylinder(zipLineBackTubeObj, cyclerRadius, cyclerRadius, cyclerThickness, 12, 3);
				gluQuadricTexture(zipLineBackTubeObj, GL_FALSE);
				Texture::off();

				glMatrixMode(GL_TEXTURE);
				glLoadIdentity();
				glMatrixMode(GL_MODELVIEW);
			}
			glPopMatrix();
			glTranslatef(0, -cyclerThickness, 0);
			glRotatef(90, 1, 0, 0);

			Color::lightBlue2();
			Lightning::darkerBlueMaterial();

			Texture::on();
			Texture::use(Texture::_blue);
			gluQuadricTexture(zipLineBackTubeObj, GL_TRUE);
			gluDisk(zipLineBackTubeObj, 0, cyclerRadius, 12, 3);
			gluQuadricTexture(zipLineBackTubeObj, GL_FALSE);
			Texture::off();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

// Weapon Models
void Model::r99() {

	if (weaponObj == NULL) {
		weaponObj = gluNewQuadric();
	}

	float centre1[3] = { 0,0,0 };
	float zAxis[3] = { 0,0,1 };
	float normal1[3] = { 0,0,0 };
	float normal2[3] = { 0,0,0 };

	CoordinateSet barrelCoors(10);
	barrelCoors.addCoordinate(0.06, -0.05, 0);
	barrelCoors.addCoordinate(0.07, -0.04, 0);
	barrelCoors.addCoordinate(0.07, 0.04, 0);
	barrelCoors.addCoordinate(0.06, 0.05, 0);
	barrelCoors.addCoordinate(-0.06, 0.05, 0);
	barrelCoors.addCoordinate(-0.07, 0.04, 0);
	barrelCoors.addCoordinate(-0.07, -0.04, 0);
	barrelCoors.addCoordinate(-0.06, -0.05, 0);

	CoordinateSet barrelBack(10);
	barrelBack.addCoordinate(0.01, -0.03, 0);
	barrelBack.addCoordinate(0.01, 0.03, 0);
	barrelBack.addCoordinate(-0.01, 0.03, 0);
	barrelBack.addCoordinate(-0.01, -0.03, 0);

	CoordinateSet shoulderRest(10);
	shoulderRest.addCoordinate(0.05, -0.1, 0);
	shoulderRest.addCoordinate(0.05, 0.1, 0);
	shoulderRest.addCoordinate(-0.05, 0.1, 0);
	shoulderRest.addCoordinate(-0.05, 0.02, 0);
	shoulderRest.addCoordinate(-0.01, 0, 0);
	shoulderRest.addCoordinate(-0.01, -0.08, 0);

	CoordinateSet barrelSide(10);
	barrelSide.addCoordinate(-0.01, -0.04, 0);
	barrelSide.addCoordinate(0.01, -0.04, 0);
	barrelSide.addCoordinate(0.01, 0.04, 0);
	barrelSide.addCoordinate(-0.01, 0.04, 0);

	CoordinateSet barrelBottom(10);
	barrelBottom.addCoordinate(0.05, -0.025, 0);
	barrelBottom.addCoordinate(0.06, -0.02, 0);
	barrelBottom.addCoordinate(0.06, 0.02, 0);
	barrelBottom.addCoordinate(0.05, 0.025, 0);
	barrelBottom.addCoordinate(-0.05, 0.025, 0);
	barrelBottom.addCoordinate(-0.06, 0.02, 0);
	barrelBottom.addCoordinate(-0.06, -0.02, 0);
	barrelBottom.addCoordinate(-0.05, -0.025, 0);

	CoordinateSet barrelTop(10);
	barrelTop.addCoordinate(-0.04, -0.02, 0);
	barrelTop.addCoordinate(0.04, -0.02, 0);
	barrelTop.addCoordinate(0.04, 0.01, 0);
	barrelTop.addCoordinate(-0.04, 0.01, 0);

	CoordinateSet barrelTopBack(10);
	barrelTopBack.addCoordinate(-0.05, -0.02, 0);
	barrelTopBack.addCoordinate(0.05, -0.02, 0);
	barrelTopBack.addCoordinate(0.05, 0.02, 0);
	barrelTopBack.addCoordinate(-0.05, 0.02, 0);

	CoordinateSet handle1(10);
	handle1.addCoordinate(-0.05, -0.08, 0);
	handle1.addCoordinate(-0.04, -0.1, 0);
	handle1.addCoordinate(0.04, -0.1, 0);
	handle1.addCoordinate(0.05, -0.08, 0);
	handle1.addCoordinate(0.05, 0, 0);
	handle1.addCoordinate(-0.05, 0, 0);

	CoordinateSet magazine(10);
	magazine.addCoordinate(-0.08, -0.15, 0);
	magazine.addCoordinate(-0.06, -0.2, 0);
	magazine.addCoordinate(0.06, -0.2, 0);
	magazine.addCoordinate(0.08, -0.15, 0);
	magazine.addCoordinate(0.08, 0.15, 0);
	magazine.addCoordinate(-0.08, 0.15, 0);

	CoordinateSet handle2(10);
	handle2.addCoordinate(-0.06, -0.10, 0);
	handle2.addCoordinate(-0.05, -0.12, 0);
	handle2.addCoordinate(0.05, -0.12, 0);
	handle2.addCoordinate(0.06, -0.10, 0);
	handle2.addCoordinate(0.03, 0.12, 0);
	handle2.addCoordinate(-0.03, 0.12, 0);

	CoordinateSet sight(10);
	sight.addCoordinate(-0.05, -0.02, 0);
	sight.addCoordinate(0.05, -0.02, 0);
	sight.addCoordinate(0.03, 0.04, 0);
	sight.addCoordinate(0, 0, 0);
	sight.addCoordinate(-0.04, 0, 0);
	glPushMatrix();
	{
		glTranslatef(0, 0, -0.5f);

		glPushMatrix();
		{
			Lightning::yellowMaterial();
			Color::yellow();
			Utility::extrudePolygon(barrelCoors, centre1, zAxis, 1.0f, TextureMap::gunYellow(), true, true);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.055f, 0, 1.0f);
			Lightning::greyMaterial();
			Color::lightGrey();
			Utility::extrudePolygon(barrelBack, centre1, zAxis, 0.5f, TextureMap::gunGrey(), true, true);
		}
		glPopMatrix();

		glPushMatrix();
		{

			glTranslatef(-0.055f, 0, 1.0f);
			Lightning::greyMaterial();
			Color::lightGrey();
			Utility::extrudePolygon(barrelBack, centre1, zAxis, 0.5f, TextureMap::gunGrey(), true, true);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.07f, -0.05f, 1.5f);
			glRotatef(-90, 0, 1, 0);
			Lightning::blackMaterial();
			Color::black();
			Utility::extrudePolygon(shoulderRest, centre1, zAxis, 0.14f, TextureMap::gunBlack(), true, true);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0.069, 0, 0);
			Lightning::blackMaterial();
			Color::black();
			Utility::extrudePolygon(barrelSide, centre1, zAxis, 0.2f, TextureMap::gunBlack(), true, true);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(-0.069, 0, 0);
			Lightning::blackMaterial();
			Color::black();
			Utility::extrudePolygon(barrelSide, centre1, zAxis, 0.2f, TextureMap::gunBlack(), true, true);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0, -0.075, 0.05);
			Lightning::greyMaterial();
			Color::lightGrey();
			Utility::extrudePolygon(barrelBottom, centre1, zAxis, 0.2f, TextureMap::gunGrey(), true, true);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0, 0.07, 0.07);
			Lightning::blackMaterial();
			Color::black();
			Utility::extrudePolygon(barrelTop, centre1, zAxis, 0.35f, TextureMap::gunBlack(), true, true);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0, 0.07, 0.42);
			Lightning::blackMaterial();
			Color::black();
			Utility::extrudePolygon(barrelTopBack, centre1, zAxis, 0.53f, TextureMap::gunBlack(), true, true);
		}
		glPopMatrix();

		glPushMatrix();
		{
			glTranslatef(0, -0.1, 0.05);
			Lightning::greyMaterial();
			Color::lightGrey();
			Utility::extrudePolygon(handle1, centre1, zAxis, 0.03f, TextureMap::gunGrey(), true, true);
		}
		glPopMatrix();

		//magazine
		glPushMatrix();
		{
			glTranslatef(-0.03, -0.13, 0.5);
			glRotatef(-30, 1, 0, 0);
			glRotatef(90, 0, 1, 0);
			Lightning::greyMaterial();
			Color::lightGrey();
			Utility::extrudePolygon(magazine, centre1, zAxis, 0.06, TextureMap::gunGrey(), true, true);
		}
		glPopMatrix();

		//handle2
		glPushMatrix();
		{
			glTranslatef(-0.03, -0.10, 0.9);
			glRotatef(-30, 1, 0, 0);
			glRotatef(90, 0, 1, 0);
			Lightning::greyMaterial();
			Color::lightGrey();
			Utility::extrudePolygon(handle2, centre1, zAxis, 0.06, TextureMap::gunGrey(), true, true);
		}
		glPopMatrix();

		//barrel head
		glPushMatrix();
		{
			glTranslatef(0, 0, -0.1f);
			glRotatef(90, 0, 0, 1);
			Lightning::blackMaterial();
			Color::black();
			Texture::use(Texture::_gunBlack);
			Texture::on();
			gluCylinder(weaponObj, 0.02f, 0.02f, 0.1f, 20, 20);
			gluPartialDisk(weaponObj, 0, 0.02f, 20, 20, 0, 360);
			Texture::off();
		}
		glPopMatrix();

		//lower barrel head
		glPushMatrix();
		{
			glTranslatef(0, -0.075f, -0.15f);
			glRotatef(90, 0, 0, 1);
			Lightning::blackMaterial();
			Color::black();
			Texture::use(Texture::_gunBlack);
			Texture::on();
			gluCylinder(weaponObj, 0.025f, 0.025f, 0.2f, 20, 20);
			gluPartialDisk(weaponObj, 0, 0.025f, 20, 20, 0, 360);
			Texture::off();
		}
		glPopMatrix();


		glPushMatrix();
		{
			glTranslatef(0.05f, 0.11f, 0.88f);
			glRotatef(-90, 0, 1, 0);
			Lightning::greyMaterial();
			Color::lightGrey();
			Utility::extrudePolygon(sight, centre1, zAxis, 0.1, TextureMap::gunGrey(), true, true);
		}
		glPopMatrix();
	}
	glPopMatrix();

	barrelCoors.destroy();
	barrelSide.destroy();
	barrelBottom.destroy();
	barrelTop.destroy();
	barrelBack.destroy();
	barrelTopBack.destroy();
	handle1.destroy();
	magazine.destroy();
	handle2.destroy();
}

void Model::bullet() {
	if (weaponObj == NULL) {
		weaponObj = gluNewQuadric();
	}

	glColor3f(0, 0, 0);
	glPushMatrix();
	{
		Color::lightGrey();
		Lightning::greyMaterial();
		glTranslatef(bulletPos[0], bulletPos[1], bulletPos[2]);
		gluCylinder(weaponObj, 0.001f, 0.02f, 0.05f, 10, 10);
		glTranslatef(0, 0, 0.05f);
		gluCylinder(weaponObj, 0.02f, 0.02f, 0.05f, 10, 10);
		//gluSphere(weaponObj, 0.02f, 10, 10);
	}
	glPopMatrix();

}

void Model::kukriKnife() {
	if (weaponObj == NULL) {
		weaponObj = gluNewQuadric();
	}

	float centre1[3] = { -0.2,0.2,0 };
	float zAxis[3] = { 0,0,1 };

	CoordinateSet temp(10);
	CoordinateSet temp2(10);
	temp.addCoordinate(0, 0.4, 0);
	temp.addCoordinate(-0.6, 0.3, 0);
	temp.addCoordinate(-0.9, -0.1, 0);
	temp.addCoordinate(-1.3, -0.5, 0);
	temp2 = Utility::bezierCurveCoords(temp, 20);

	CoordinateSet knife(10);
	knife.addCoordinate(0.3, 0.4, 0);
	knife.combineCoords(temp2);

	temp.clear(10);

	temp.addCoordinate(-1.3, -0.5, 0);
	temp.addCoordinate(-0.8, -0.7, 0);
	temp.addCoordinate(-0.7, -0.3, 0);
	temp.addCoordinate(-0.4, -0.1, 0);
	temp.addCoordinate(-0.3, 0.2, 0);
	temp.addCoordinate(0.1, 0.2, 0);
	temp2 = Utility::bezierCurveCoords(temp, 20);

	knife.combineCoords(temp2);
	knife.addCoordinate(0.3, 0.2, 0);

	glPushMatrix();
	{
		Color::silver();
		Lightning::silverMaterial();
		glTranslatef(0, -0.02f, 0);
		//Utility::drawLine(knife);
		Utility::extrudePolygon(knife, centre1, zAxis, 0.02, true, true);
	}
	glPopMatrix();

	glPushMatrix();
	{
		glTranslatef(0.3f, 0.27f, 0);
		glRotatef(90, 0, 1, 0);
		glRotatef(90, 0, 0, 1);
		glPushMatrix();
		{
			Color::black();
			Lightning::blackMaterial();
			gluPartialDisk(weaponObj, 0, 0.12f, 10, 10, 0, 360);
			gluCylinder(weaponObj, 0.12f, 0.12f, 0.05f, 20, 20);
			glTranslatef(0, 0, 0.05f);
			gluPartialDisk(weaponObj, 0, 0.12f, 10, 10, 0, 360);
		}
		glPopMatrix();

		glPushMatrix();
		{
			Color::darkGrey();
			Lightning::darkerGreyMaterial();
			glTranslatef(0, 0, 0.05f);
			gluPartialDisk(weaponObj, 0, 0.08f, 10, 10, 0, 360);
			gluCylinder(weaponObj, 0.08f, 0.08f, 0.6f, 20, 20);
			glTranslatef(0, 0, 0.6f);
			gluPartialDisk(weaponObj, 0, 0.08f, 10, 10, 0, 360);
		}
		glPopMatrix();
	}
	glPopMatrix();

	knife.destroy();
	temp.destroy();
	temp2.destroy();

}

void Model::bullet() {
	if (weaponObj == NULL) {
		weaponObj = gluNewQuadric();
	}

	glPushMatrix();
	{
		glScalef(1, 1, 6);
		glTranslatef(0, 0, -0.1);
		Color::lightGrey();
		Lightning::greyMaterial();
		gluCylinder(weaponObj, 0.001f, 0.01f, 0.05f, 10, 10);
		glTranslatef(0, 0, 0.05f);
		gluCylinder(weaponObj, 0.01f, 0.01f, 0.05f, 10, 10);
	}
	glPopMatrix();
}

void Model::initializeBulletPositions() {
	for (int i = 0; i < MAX_BULLETS_ON_SCREEN; i++) {
		bulletPositions[i][0] = 1;
		bulletPositions[i][1] = 0;
		bulletPositions[i][2] = 0;
	}
}

void Model::resetBulletPosition(int pos) {
	bulletPositions[pos][0] = 1;
	bulletPositions[pos][1] = 0;
	bulletPositions[pos][2] = 0;
}