#include "Animation.h"
#include "Model.h"
#include "Time.h"

void Animation::reset(float seconds) {
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
		boolean stopped = true;
		if (Model::RLegUpperRot[2] > 0) {
			Model::RLegUpperRot[2] -= speed;
			Model::RLegUpperRot[2] = clampFloat(Model::RLegUpperRot[2], 0, 180);
			if (Model::RLegUpperRot[2] != 0)
				stopped = false;
		}
		else if (Model::RLegUpperRot[2] < 0) {
			Model::RLegUpperRot[2] += speed;
			Model::RLegUpperRot[2] = clampFloat(Model::RLegUpperRot[2], -180, 0);
			if (Model::RLegUpperRot[2] != 0)
				stopped = false;
		}

		if (Model::LLegUpperRot[2] > 0) {
			Model::LLegUpperRot[2] -= speed;
			Model::LLegUpperRot[2] = clampFloat(Model::LLegUpperRot[2], 0, 180);
			if (Model::LLegUpperRot[2] != 0)
				stopped = false;
		}
		else if (Model::LLegUpperRot[2] < 0) {
			Model::LLegUpperRot[2] += speed;
			Model::LLegUpperRot[2] = clampFloat(Model::LLegUpperRot[2], -180, 0);
			if (Model::LLegUpperRot[2] != 0)
				stopped = false;
		}

		if (Model::RLegHingeRot > 0) {
			Model::RLegHingeRot -= 0.3 * speed;
			Model::RLegHingeRot = clampFloat(Model::RLegHingeRot, 0, 180);
			if (Model::RLegHingeRot != 0)
				stopped = false;
		}
		else if (Model::RLegHingeRot < 0) {
			Model::RLegHingeRot += 0.3 * speed;
			Model::RLegHingeRot = clampFloat(Model::RLegHingeRot, -180, 0);
			if (Model::RLegHingeRot != 0)
				stopped = false;
		}

		if (Model::LLegHingeRot > 0) {
			Model::LLegHingeRot -= 0.3 * speed;
			Model::LLegHingeRot = clampFloat(Model::LLegHingeRot, 0, 180);
			if (Model::LLegHingeRot != 0)
				stopped = false;
		}
		else if (Model::LLegHingeRot < 0) {
			Model::LLegHingeRot += 0.3 * speed;
			Model::LLegHingeRot = clampFloat(Model::LLegHingeRot, -180, 0);
			if (Model::LLegHingeRot != 0)
				stopped = false;
		}

		if (Model::RFeetRot > 0) {
			Model::RFeetRot -= 0.25 * speed;
			Model::RFeetRot = clampFloat(Model::RFeetRot, 0, 180);
			if (Model::RFeetRot != 0)
				stopped = false;
		}
		else if (Model::RFeetRot < 0) {
			Model::RFeetRot += 0.25 * speed;
			Model::RFeetRot = clampFloat(Model::RFeetRot, -180, 0);
			if (Model::RFeetRot != 0)
				stopped = false;
		}

		if (Model::LFeetRot > 0) {
			Model::LFeetRot -= 0.25 * speed;
			Model::LFeetRot = clampFloat(Model::LFeetRot, 0, 180);
			if (Model::LFeetRot != 0)
				stopped = false;
		}
		else if (Model::LFeetRot < 0) {
			Model::LFeetRot += 0.25 * speed;
			Model::LFeetRot = clampFloat(Model::LFeetRot, -180, 0);
			if (Model::LFeetRot != 0)
				stopped = false;
		}

		if (Model::bodyPos[1] > 0) {
			Model::bodyPos[1] -= walkBodyShifting * speed;
			Model::bodyPos[1] = clampFloat(Model::bodyPos[1], 0, 10);
			if (Model::bodyPos[1] != 0)
				stopped = false;
		}
		else if (Model::bodyPos[1] < 0) {
			Model::bodyPos[1] += walkBodyShifting * speed;
			Model::bodyPos[1] = clampFloat(Model::bodyPos[1], -10, 0);
			if (Model::bodyPos[1] != 0)
				stopped = false;
		}

		Model::bodyRot[0] += 0.1 * speed;
		Model::bodyRot[0] = clampFloat(Model::bodyRot[0], -180, 0);
		if (Model::bodyRot[0] != 0)
			stopped = false;

		if (stopped) {
			walkSteps = 0;
		}
		return;
	}

	// Walking animation loop
	if (walkDir == 0) {
		if (walkSteps == 1) {
			Model::RLegUpperRot[2] += speed;
			Model::bodyRot[0] -= 0.1 * speed;
			Model::RLegHingeRot -= 0.2 * speed;
			Model::RFeetRot += 0.1 * speed;
		}
		else {
			Model::RLegUpperRot[2] += speed;
			Model::LLegUpperRot[2] -= speed;
			Model::RLegHingeRot += 0.3 * speed;
			Model::RFeetRot += 0.25 * speed;
			if (Model::RLegUpperRot[2] >= 0) {
				Model::bodyPos[1] += walkBodyShifting * speed;
			}
			else {
				Model::bodyPos[1] -= walkBodyShifting * speed;
			}
		}

		Model::LFeetRot -= 0.25 * speed;
		Model::LLegHingeRot -= 0.3 * speed;
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
			Model::LLegHingeRot -= 0.5f * 0.2 * speed;
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
			Model::LLegHingeRot += 0.3 * speed;
			Model::LFeetRot += 0.25 * speed;
			if (Model::LLegUpperRot[2] >= 0) {
				Model::bodyPos[1] += walkBodyShifting * speed;
			}
			else {
				Model::bodyPos[1] -= walkBodyShifting * speed;
			}
		}
		Model::RFeetRot -= 0.25 * speed;

		Model::RLegHingeRot -= 0.3 * speed;

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