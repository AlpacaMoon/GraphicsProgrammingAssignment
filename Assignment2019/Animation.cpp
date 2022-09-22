#include "Animation.h"
#include "Model.h"
#include "Time.h"

void Animation::hardReset() {
	for (int i = 0; i < 3; i++) {
		Model::headRot[i] = 0;
		Model::RLegUpperRot[i] = 0;
		Model::LLegUpperRot[i] = 0;
		Model::hipRot[i] = 0;
		Model::bodyRot[i] = 0;
		for (int j = 0; j < 3; j++) {
			Model::RArmRot[i][j] = 0;
			Model::LArmRot[i][j] = 0;
		}
		for (int j = 0; j < 5; j++) {
			Model::RFingerRot[j][i] = Model::openedFingerRot[j][i];
		}
	}

	Model::RLegHingeRot = 0;
	Model::RFeetRot = 0;

	Model::LLegHingeRot = 0;
	Model::LFeetRot = 0;

	Model::RArmRot[0][0] = 10;
	Model::RArmRot[0][1] = -15;
	Model::RArmRot[1][2] = 15;

	Model::LArmRot[0][0] = -10;
	Model::LArmRot[0][1] = 15;
	Model::LArmRot[1][2] = 15;
}

bool Animation::softResetClamping(float* target, float min, float frustum, float max, float speed) {
	bool stopped = true;
	if (*target > frustum) {
		*target -= speed;
		*target = clampFloat(*target, frustum, max);
		if (*target != frustum)
			stopped = false;
	}
	else if (*target < frustum) {
		*target += speed;
		*target = clampFloat(*target, min, frustum);
		if (*target != frustum)
			stopped = false;
	}
	return stopped;
}

// Resets animation to original rotation and position, 
// But translates/rotates them instead of displacing them like hardReset()
// Returns true if softReset is finished, false if not. 
bool Animation::softReset(float speed) {
	boolean stopped = true;

	// Ltr do

	return stopped;
}

void Animation::runAnimations() {

	if (Animation::walkSteps != 0)
		Animation::walk();
}

// WalkSteps values: 
/*	
*	0 - Finish walking / Stopped walking
*	1 = Start walking loop
*	2 - First step finished (Move right leg first)
*	3 - Second step finished (Move left leg second)
*	4 - Walking loop 
*	5 - Stop walking loop
*/
int Animation::walkSteps = 0;
int walkDir = 0;
int walkSpeed = 200.0f;
float walkBodyShifting = 0.001f;

void Animation::startWalking() {
	walkSteps = 1;
	walkDir = 0;
}

void Animation::stopWalking() {
	walkSteps = 5;

}

void Animation::walk() {
	float speed = walkSpeed * Time::elapsedSeconds;

	// Stop walking
	if (walkSteps == 0)
		return;
	else if (walkSteps == 5) {
		bool stopped = true;

		openRightHand(speed);
		openLeftHand(speed);

		if (softResetClamping(&Model::RArmRot[0][2], -360, 15, 360, speed) == false)
			stopped = false;
		if (softResetClamping(&Model::LArmRot[0][2], -360, 15, 360, speed) == false)
			stopped = false;

		if (softResetClamping(&Model::RLegUpperRot[2], -360, 0, 360, speed) == false)
			stopped = false;
		if (softResetClamping(&Model::LLegUpperRot[2], -360, 0, 360, speed) == false)
			stopped = false;

		if (softResetClamping(&Model::RLegHingeRot, -360, 0, 360, speed) == false)
			stopped = false;
		if (softResetClamping(&Model::LLegHingeRot, -360, 0, 360, speed) == false)
			stopped = false;

		if (softResetClamping(&Model::RFeetRot, -360, 0, 360, speed) == false)
			stopped = false;
		if (softResetClamping(&Model::LFeetRot, -360, 0, 360, speed) == false)
			stopped = false;

		if (softResetClamping(&Model::bodyPos[1], -10, 0, 10, speed) == false)
			stopped = false;
	
		if (softResetClamping(&Model::bodyRot[0], -360, 0, 360, 0.1 * speed) == false)
			stopped = false;
		if (softResetClamping(&Model::bodyRot[2], -360, 0, 360, 0.05 * speed) == false)
			stopped = false;

		if (stopped) {
			walkSteps = 0;
		}
		return;
	}

	// Close hands
	closeRightHand(speed);
	closeLeftHand(speed);

	// Walking animation loop
	if (walkDir == 0) {
		if (walkSteps == 1) {
			Model::RLegUpperRot[2] += speed;
			Model::bodyRot[0] -= 0.2 * speed;
			Model::RLegHingeRot -= 0.3 * speed;
			Model::RFeetRot += 0.1 * speed;
			Model::bodyRot[2] -= 0.01 * speed;
		}
		else {
			Model::RLegUpperRot[2] += speed;
			Model::LLegUpperRot[2] -= speed;
			Model::RLegHingeRot += 0.4 * speed;
			Model::RFeetRot += 0.25 * speed;
			Model::bodyRot[0] -= 0.05 * speed;
			if (Model::RLegUpperRot[2] >= 0) {
				Model::bodyPos[1] += walkBodyShifting * speed;
			}
			else {
				Model::bodyPos[1] -= walkBodyShifting * speed;
			}
			Model::bodyPos[1] = clampFloat(Model::bodyPos[1], -0.025, 0.025);

			Model::bodyRot[2] -= 0.02 * speed;
		}

		Model::RArmRot[0][2] += (-15 - Model::RArmRot[0][2]) * 0.6 * Time::elapsedSeconds * 15;
		Model::RArmRot[0][2] = clampFloat(Model::RArmRot[0][2], -15, 45);
		Model::LArmRot[0][2] += (45 - Model::LArmRot[0][2]) * 0.6 * Time::elapsedSeconds * 15;
		Model::LArmRot[0][2] = clampFloat(Model::LArmRot[0][2], -15, 45);

		Model::LFeetRot -= 0.25 * speed;
		Model::LLegHingeRot -= 0.4 * speed;
		clampRightLeg();
		clampLeftLeg();
		if (Model::RLegUpperRot[2] >= 45.0f) {
			walkDir = 1;
			if (walkSteps == 1)
				walkSteps = 2;
			else
				walkSteps = 4;
		}
	}
	else {
		if (walkSteps == 2) {
			Model::RLegUpperRot[2] -= speed;
			Model::LLegUpperRot[2] += 0.5f * speed;
			Model::LLegHingeRot -= 0.5 * 0.3 * speed;
			Model::LFeetRot += 0.5f * 0.25 * speed;
			if (Model::RLegUpperRot[2] >= 0) {
				Model::bodyPos[1] -= 0.5 * walkBodyShifting * speed;
			}
			else {
				Model::bodyPos[1] += 0.5 * walkBodyShifting * speed;
			}
		}
		else {
			Model::RLegUpperRot[2] -= speed;
			Model::LLegUpperRot[2] += speed;
			Model::LLegHingeRot += 0.4 * speed;
			Model::LFeetRot += 0.25 * speed;
			if (Model::LLegUpperRot[2] >= 0) {
				Model::bodyPos[1] += walkBodyShifting * speed;
			}
			else {
				Model::bodyPos[1] -= walkBodyShifting * speed;
			}
			Model::bodyPos[1] = clampFloat(Model::bodyPos[1], -0.025, 0.025);

		}

		Model::bodyRot[2] += 0.02 * speed;
		Model::RFeetRot -= 0.25 * speed;
		Model::RLegHingeRot -= 0.4 * speed;
		Model::bodyRot[0] += 0.05 * speed;


		Model::RArmRot[0][2] += (45 - Model::RArmRot[0][2]) * 0.6 * Time::elapsedSeconds * 15;
		Model::RArmRot[0][2] = clampFloat(Model::RArmRot[0][2], -15, 45);
		Model::LArmRot[0][2] += (-15 - Model::LArmRot[0][2]) * 0.6 * Time::elapsedSeconds * 15;
		Model::LArmRot[0][2] = clampFloat(Model::LArmRot[0][2], -15, 45);

		clampRightLeg();
		clampLeftLeg();
		if (Model::LLegUpperRot[2] >= 45.0f) {
			walkDir = 0;
			if (walkSteps == 2)
				walkSteps = 3;
			else
				walkSteps = 4;
		}
	}
}

void Animation::rotateWalk(WPARAM key) {
	float rotSpeed = 150 * Time::elapsedSeconds;
	switch (key) {
	case 'A':
		Model::bodyRot[1] -= rotSpeed;
		break;
	case 'D':
		Model::bodyRot[1] += rotSpeed;
		break;
	}
}


// General Animations

void Animation::closeRightHand(float fingerSpeed) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			Model::RFingerRot[i][j] += fingerSpeed;
			if (Model::RFingerRot[i][j] > Model::closedFingerRot[i][j])
				Model::RFingerRot[i][j] = Model::closedFingerRot[i][j];
		}
	}
}

void Animation::closeLeftHand(float fingerSpeed) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			if (Model::LFingerRot[i][j] < Model::closedFingerRot[i][j]) {
				Model::LFingerRot[i][j] += fingerSpeed;
				if (Model::LFingerRot[i][j] > Model::closedFingerRot[i][j])
					Model::LFingerRot[i][j] = Model::closedFingerRot[i][j];
			}
		}
	}
}

void Animation::openRightHand(float fingerSpeed) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			if (Model::RFingerRot[i][j] > Model::openedFingerRot[i][j]) {
				Model::RFingerRot[i][j] -= fingerSpeed;
				if (Model::RFingerRot[i][j] < Model::openedFingerRot[i][j])
					Model::RFingerRot[i][j] = Model::openedFingerRot[i][j];
			}
		}
	}
}

void Animation::openLeftHand(float fingerSpeed) {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			if (Model::LFingerRot[i][j] > Model::openedFingerRot[i][j]) {
				Model::LFingerRot[i][j] -= fingerSpeed;
				if (Model::LFingerRot[i][j] < Model::openedFingerRot[i][j])
					Model::LFingerRot[i][j] = Model::openedFingerRot[i][j];
			}
		}
	}
}



// Clamping
float Animation::clampFloat(float value, float min, float max) {
	if (value > max)
		return max;
	else if (value < min)
		return min;
	return value;
}


void Animation::clampHead() {
	Model::headRot[2] = clampFloat(Model::headRot[2], -20, 30);
	Model::headRot[1] = clampFloat(Model::headRot[1], -105, 105);
	Model::headRot[0] = clampFloat(Model::headRot[0], -30, 30);
}

void Animation::clampHip() {
	Model::hipRot[2] = clampFloat(Model::hipRot[2], -60, 60);
	Model::hipRot[1] = clampFloat(Model::hipRot[1], -35, 35);
	Model::hipRot[0] = clampFloat(Model::hipRot[0], -5, 5);
}
void Animation::clampBody() {
	// No clamping required
}
void Animation::clampRightLeg() {
	Model::RLegUpperRot[2] = clampFloat(Model::RLegUpperRot[2], -90, 110);
	Model::RLegUpperRot[1] = clampFloat(Model::RLegUpperRot[1], -30, 30);
	Model::RLegUpperRot[0] = clampFloat(Model::RLegUpperRot[0], -10, 10);
	Model::RLegHingeRot = clampFloat(Model::RLegHingeRot, -90, 0);
	Model::RFeetRot = clampFloat(Model::RFeetRot, -20, 20);
}
void Animation::clampLeftLeg() {
	Model::LLegUpperRot[2] = clampFloat(Model::LLegUpperRot[2], -90, 110);
	Model::LLegUpperRot[1] = clampFloat(Model::LLegUpperRot[1], -30, 30);
	Model::LLegUpperRot[0] = clampFloat(Model::LLegUpperRot[0], -10, 10);
	Model::LLegHingeRot = clampFloat(Model::LLegHingeRot, -100, 0);
	Model::LFeetRot = clampFloat(Model::LFeetRot, -20, 20);
}
void Animation::clampRightArm() {
	Model::RArmRot[0][2] = clampFloat(Model::RArmRot[0][2], -180, 180);
	Model::RArmRot[0][1] = clampFloat(Model::RArmRot[0][1], -75, 75);
	Model::RArmRot[0][0] = clampFloat(Model::RArmRot[0][0], 0, 180);
	Model::RArmRot[1][2] = clampFloat(Model::RArmRot[1][2], 0, 160);
	Model::RArmRot[2][2] = clampFloat(Model::RArmRot[2][2], -60, 60);
	Model::RArmRot[2][1] = clampFloat(Model::RArmRot[2][1], -90, 90);
	Model::RArmRot[2][0] = clampFloat(Model::RArmRot[2][0], -90, 90);
}
void Animation::clampLeftArm() {
	Model::LArmRot[0][2] = clampFloat(Model::LArmRot[0][2], -180, 180);
	Model::LArmRot[0][1] = clampFloat(Model::LArmRot[0][1], -75, 75);
	Model::LArmRot[0][0] = clampFloat(Model::LArmRot[0][0], -180, 0);
	Model::LArmRot[1][2] = clampFloat(Model::LArmRot[1][2], 0, 160);
	Model::LArmRot[2][2] = clampFloat(Model::LArmRot[2][2], -60, 60);
	Model::LArmRot[2][1] = clampFloat(Model::LArmRot[2][1], -90, 90);
	Model::LArmRot[2][0] = clampFloat(Model::LArmRot[2][0], -90, 90);
}
void Animation::clampRightFingers() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			Model::RFingerRot[i][j] = clampFloat(Model::RFingerRot[i][j], Model::openedFingerRot[i][j], Model::closedFingerRot[i][j]);
		}
	}
}
void Animation::clampLeftFingers() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 3; j++) {
			Model::LFingerRot[i][j] = clampFloat(Model::LFingerRot[i][j], Model::openedFingerRot[i][j], Model::closedFingerRot[i][j]);
		}
	}
}

void Animation::shootBullet() {
	if (Model::isFired && Model::bulletPos[0] > -2 && Model::bulletPos[1] > -2 && Model::bulletPos[2] > -2) {
		Model::bulletPos[2] -= 0.005f;
	}
	else {
		Model::bulletPos[0] = Model::LArmRot[2][0];
		Model::bulletPos[1] = Model::LArmRot[2][1];
		Model::bulletPos[2] = Model::LArmRot[2][2];
	}
}

/*	Texture files
	1)	"Textures/tvAngry.bmp",
	2)	"Textures/tvBlank.bmp",
	3)	"Textures/tvDead.bmp",
	4)	"Textures/tvExclamation.bmp",
	5)	"Textures/tvGlitched.bmp",
	6)	"Textures/tvHappy.bmp",
	7)	"Textures/tvKO.bmp",
	8)	"Textures/tvLove.bmp",
	9)	"Textures/tvQuestion-mark.bmp",
	10)	"Textures/tvSad.bmp",
	11)	"Textures/tvThumbs-Up.bmp",
*/
int Animation::currentTVscreen = 0;
void Animation::switchTVscreen() {
	currentTVscreen = (currentTVscreen + 1) % 11;
}