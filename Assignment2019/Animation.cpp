#include "Animation.h"
#include "Model.h"
#include "Time.h"
#include "Controls.h"

int Animation::playingCutscene = 0;

void Animation::hardReset() {
	for (int i = 0; i < 3; i++) {
		Model::headRot[i] = 0;
		Model::RLegUpperRot[i] = 0;
		Model::LLegUpperRot[i] = 0;
		Model::hipRot[i] = 0;
		Model::bodyRot[i] = 0;
		for (int j = 0; j < 3; j++) {
			Model::RArmRot[i][j] = Model::defaultRArmRot[i][j];
			Model::LArmRot[i][j] = Model::defaultLArmRot[i][j];
		}
		for (int j = 0; j < 5; j++) {
			Model::RFingerRot[j][i] = Model::openedFingerRot[j][i];
			Model::LFingerRot[j][i] = Model::openedFingerRot[j][i];
		}
	}

	Model::RLegHingeRot = 0;
	Model::RFeetRot = 0;

	Model::LLegHingeRot = 0;
	Model::LFeetRot = 0;

	cancelAllAnimations();
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

bool Animation::isResetting = false;
void Animation::startReset() {
	isResetting = true;
}

// Resets animation to original rotation and position, 
// But translates/rotates them instead of displacing them like hardReset()
// Returns true if softReset is finished, false if not. 
bool Animation::softReset(float speed) {
	boolean stopped = true;
	
	for (int i = 0; i < 3; i++) {
		if (softResetClamping(&Model::bodyPos[i], -360, Model::defaultBodyPos[i], 360, speed / 100.0f) == false)
			stopped = false;
		if (softResetClamping(&Model::bodyRot[i], -360, Model::defaultBodyRot[i], 360, speed) == false)
			stopped = false;
		if (softResetClamping(&Model::headRot[i], -360, Model::defaultHeadRot[i], 360, speed) == false)
			stopped = false;
		if (softResetClamping(&Model::RLegUpperRot[i], -360, Model::defaultRLegUpperRot[i], 360, speed) == false)
			stopped = false;
		if (softResetClamping(&Model::LLegUpperRot[i], -360, Model::defaultLLegUpperRot[i], 360, speed) == false)
			stopped = false;
		if (softResetClamping(&Model::hipRot[i], -360, Model::defaultHipRot[i], 360, speed) == false)
			stopped = false;

		for (int j = 0; j < 3; j++) {
			if (softResetClamping(&Model::RArmRot[i][j], -360, Model::defaultRArmRot[i][j], 360, speed) == false)
				stopped = false;
			if (softResetClamping(&Model::LArmRot[i][j], -360, Model::defaultLArmRot[i][j], 360, speed) == false)
				stopped = false;
		}

		for (int j = 0; j < 5; j++) {
			if (softResetClamping(&Model::RFingerRot[i][j], -360, Model::openedFingerRot[i][j], 360, speed) == false)
				stopped = false;
			if (softResetClamping(&Model::LFingerRot[i][j], -360, Model::openedFingerRot[i][j], 360, speed) == false)
				stopped = false;
		}
	}
	if (softResetClamping(&Model::RLegHingeRot, -360, Model::defaultRLegHingeRot, 360, speed) == false)
		stopped = false;
	if (softResetClamping(&Model::LLegHingeRot, -360, Model::defaultLLegHingeRot, 360, speed) == false)
		stopped = false;
	if (softResetClamping(&Model::RFeetRot, -360, Model::defaultRFeetRot, 360, speed) == false)
		stopped = false;
	if (softResetClamping(&Model::LFeetRot, -360, Model::defaultLFeetRot, 360, speed) == false)
		stopped = false;

	return stopped;
}

void Animation::cancelAllAnimations() {
	walkSteps = 0;
	jumpSteps = 0;
}

void Animation::runAnimations() {
	// Disallow input if resetting body parts
	if (isResetting) {
		if (softReset(250.0f * Time::elapsedSeconds))
			isResetting = false;
		return;
	}
	// Disable animations in Independent controls / Posing mode
	else if (Controls::isIndependentControls) {
		return;
	}

	// if not playing cutscene
	if (playingCutscene == 0) {

		if (!isJumping() && Controls::isPressingWalk()) {
			if (walkSteps == 0)
				startWalking();
			else
				rotateWalk();
		}

		if (walkSteps != 0) {
			Animation::walk();
		}

		if (isJumping()) {
			jump();
		}
		
		Animation::shootBullet();
	}
	// If cutscene == Zipline away
	else if (playingCutscene == 1) {

	}

}

// ==================== WALKING ====================
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
int Animation::walkDir = 0;
float Animation::walkSpeed = 200.0f;
float Animation::walkBodyShifting = 0.001f;

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

void Animation::rotateWalk() {
	float rotSpeed = 150 * Time::elapsedSeconds;
	if (Controls::pressingWalkKeys[1]) {
		Model::bodyRot[1] -= rotSpeed;
	}
	else if (Controls::pressingWalkKeys[2]) {
		Model::bodyRot[1] += rotSpeed;
	}
}


// ==================== JUMPING ====================
/* Jumpstep values: 
*	0 - Not jumping
*	1 - start jumping (Going up)
*	2 - falling down
*	3 - Finished jumping
*/
int Animation::jumpSteps = 0;
float Animation::jumpSpeed = 5.0;
float jumpFallSpeed = 0.005, jumpFallSpeedMultiplier = 1.08;
float Animation::movingBodyPartSpeed = 100.0f;
float Animation::jumpHeight = 0.5f;
Time jumpTimeVar(0.0f);
float jumpRecoverSpeed = 30.0f, jumpRecoverMultiplier;
bool Animation::isJumping() {
	return jumpSteps != 0;
}

void Animation::startJumping() {
	jumpSteps = 1;
	walkSteps = 0;
}

void Animation::jump() {
	float thisJumpSpeed = jumpSpeed * Time::elapsedSeconds;
	float speed = movingBodyPartSpeed * Time::elapsedSeconds;
	float jumpMarginalError = 0.005f;
	// Jumping up
	if (jumpSteps == 1) {

		openRightHand(speed * 0.5);
		openLeftHand(speed * 0.5);

		softResetClamping(&Model::RLegUpperRot[0], -360, -5, 360, speed);
		softResetClamping(&Model::RLegUpperRot[1], -360, 10, 360, speed);
		softResetClamping(&Model::RLegUpperRot[2], -360, 40, 360, speed);
		softResetClamping(&Model::RLegHingeRot, -360, -90, 360, speed * 1.5);
		softResetClamping(&Model::RFeetRot, -360, 20, 360, speed);

		softResetClamping(&Model::LLegUpperRot[0], -360, 5, 360, speed);
		softResetClamping(&Model::LLegUpperRot[1], -360, -10, 360, speed);
		softResetClamping(&Model::LLegUpperRot[2], -360, 40, 360, speed);
		softResetClamping(&Model::LLegHingeRot, -360, -90, 360, speed * 1.5);
		softResetClamping(&Model::LFeetRot, -360, 20, 360, speed);

		softResetClamping(&Model::RArmRot[0][0], -360, Model::defaultRArmRot[0][0] + 40.0f, 360, speed / 2.0f);
		softResetClamping(&Model::RArmRot[0][1], -360, Model::defaultRArmRot[0][1] + 20.0f, 360, speed / 2.0f);
		softResetClamping(&Model::RArmRot[0][2], -360, Model::defaultRArmRot[0][2], 360, speed / 2.0f);
		softResetClamping(&Model::RArmRot[1][2], -360, Model::defaultRArmRot[1][2], 360, speed / 2.0f);
		softResetClamping(&Model::RArmRot[2][0], -360, Model::defaultRArmRot[2][0], 360, speed / 2.0f);
		softResetClamping(&Model::RArmRot[2][1], -360, Model::defaultRArmRot[2][1], 360, speed / 2.0f);
		softResetClamping(&Model::RArmRot[2][2], -360, Model::defaultRArmRot[2][2], 360, speed / 2.0f);

		softResetClamping(&Model::LArmRot[0][0], -360, Model::defaultLArmRot[0][0] - 40.0f, 360, speed / 2.0f);
		softResetClamping(&Model::LArmRot[0][1], -360, Model::defaultLArmRot[0][1] - 20.0f, 360, speed / 2.0f);
		softResetClamping(&Model::LArmRot[0][2], -360, Model::defaultLArmRot[0][2], 360, speed / 2.0f);
		softResetClamping(&Model::LArmRot[1][2], -360, Model::defaultLArmRot[1][2], 360, speed / 2.0f);
		softResetClamping(&Model::LArmRot[2][0], -360, Model::defaultLArmRot[2][0], 360, speed / 2.0f);
		softResetClamping(&Model::LArmRot[2][1], -360, Model::defaultLArmRot[2][1], 360, speed / 2.0f);
		softResetClamping(&Model::LArmRot[2][2], -360, Model::defaultLArmRot[2][2], 360, speed / 2.0f);

		softResetClamping(&Model::bodyRot[0], -360, Model::defaultBodyRot[0], 360, speed / 2.0f);
		softResetClamping(&Model::bodyRot[2], -360, Model::defaultBodyRot[2], 360, speed / 2.0f);

		Model::bodyPos[1] += (jumpHeight - Model::bodyPos[1]) * thisJumpSpeed;
		if (Model::bodyPos[1] > jumpHeight - jumpMarginalError) {
			jumpSteps = 2;
		}
	}
	// Falling down
	else if (jumpSteps == 2) {
		jumpFallSpeed = 0.005;
		jumpSteps = 3;
	}
	float thisFallSpeed = jumpFallSpeed * Time::elapsedSeconds;
	
	if (jumpSteps == 3) {
		softResetClamping(&Model::RArmRot[0][0], -360, Model::defaultRArmRot[0][0] + 80.0f, 360, speed / 2.0f);
		softResetClamping(&Model::RArmRot[0][1], -360, Model::defaultRArmRot[0][1] + 40.0f, 360, speed / 2.0f);
		softResetClamping(&Model::LArmRot[0][0], -360, Model::defaultLArmRot[0][0] - 80.0f, 360, speed / 2.0f);
		softResetClamping(&Model::LArmRot[0][1], -360, Model::defaultLArmRot[0][1] - 40.0f, 360, speed / 2.0f);

		Model::bodyPos[1] -= jumpFallSpeed;
		jumpFallSpeed *= jumpFallSpeedMultiplier;
		if (Model::bodyPos[1] < -0.3 + jumpMarginalError) {
			Model::bodyPos[1] = -0.3;
			jumpSteps = 4;
			jumpRecoverMultiplier = 5.0f;
		}
	}
	else if (jumpSteps == 4) {
		float thisSpeed = jumpRecoverMultiplier * Time::elapsedSeconds;
		jumpRecoverMultiplier * 1.1f;

		Model::RLegUpperRot[2] += (60 - Model::RLegUpperRot[2]) * thisSpeed * 2.25f;
		Model::LLegUpperRot[2] += (60 - Model::LLegUpperRot[2]) * thisSpeed * 2.25f;

		Model::hipRot[2] += (-20 - Model::hipRot[2]) * thisSpeed;
		Model::bodyPos[1] += (-0.45 - Model::bodyPos[1]) * thisSpeed;

		thisSpeed *= 15;
		softResetClamping(&Model::RArmRot[0][0], -360, Model::defaultRArmRot[0][0] - 5, 360, thisSpeed * 2);
		softResetClamping(&Model::RArmRot[0][1], -360, Model::defaultRArmRot[0][1], 360, thisSpeed);
		softResetClamping(&Model::RArmRot[1][2], -360, 20, 360, thisSpeed);
		softResetClamping(&Model::LArmRot[0][0], -360, Model::defaultLArmRot[0][0] + 5, 360, thisSpeed * 2);
		softResetClamping(&Model::LArmRot[0][1], -360, Model::defaultLArmRot[0][1], 360, thisSpeed);
		softResetClamping(&Model::LArmRot[1][2], -360, 20, 360, thisSpeed);

		if (Model::RLegUpperRot[2] > 60 - jumpMarginalError && Model::LLegUpperRot[2] > 60 - jumpMarginalError) {
			jumpSteps = 5;
		}
	}
	else if (jumpSteps == 5) {
		float thisSpeed = jumpRecoverSpeed * Time::elapsedSeconds;
		bool stopped = true;
		if (softResetClamping(&Model::RArmRot[0][0], -360, Model::defaultRArmRot[0][0], 360, thisSpeed) == false)
			stopped = false;
		if(softResetClamping(&Model::RArmRot[0][1], -360, Model::defaultRArmRot[0][1], 360, thisSpeed) == false)
			stopped = false;
		if(softResetClamping(&Model::RArmRot[1][2], -360, Model::defaultRArmRot[1][2], 360, thisSpeed * 5) == false)
			stopped = false;
		if(softResetClamping(&Model::LArmRot[0][0], -360, Model::defaultLArmRot[0][0], 360, thisSpeed) == false)
			stopped = false;
		if(softResetClamping(&Model::LArmRot[0][1], -360, Model::defaultLArmRot[0][1], 360, thisSpeed) == false)
			stopped = false;
		if(softResetClamping(&Model::LArmRot[1][2], -360, Model::defaultLArmRot[1][2], 360, thisSpeed * 5) == false)
			stopped = false;
		if(softResetClamping(&Model::RLegUpperRot[0], -360, Model::defaultRLegUpperRot[0], 360, thisSpeed) == false)
			stopped = false;
		if(softResetClamping(&Model::RLegUpperRot[1], -360, Model::defaultRLegUpperRot[1], 360, thisSpeed) == false)
			stopped = false;
		if(softResetClamping(&Model::RLegUpperRot[2], -360, Model::defaultRLegUpperRot[2], 360, speed * 1.35) == false)
			stopped = false;
		if(softResetClamping(&Model::RLegHingeRot, -360, Model::defaultRLegHingeRot, 360, speed * 1.8) == false)
			stopped = false;
		if(softResetClamping(&Model::RFeetRot, -360, Model::defaultRFeetRot, 360, speed) == false)
			stopped = false;
		if(softResetClamping(&Model::LLegUpperRot[0], -360, Model::defaultLLegUpperRot[0], 360, thisSpeed) == false)
			stopped = false;
		if(softResetClamping(&Model::LLegUpperRot[1], -360, Model::defaultLLegUpperRot[1], 360, thisSpeed) == false)
			stopped = false;
		if(softResetClamping(&Model::LLegUpperRot[2], -360, Model::defaultLLegUpperRot[2], 360, speed * 1.35) == false)
			stopped = false;
		if(softResetClamping(&Model::LLegHingeRot, -360, Model::defaultLLegHingeRot, 360, speed * 1.8) == false)
			stopped = false;
		if(softResetClamping(&Model::LFeetRot, -360, Model::defaultLFeetRot, 360, speed) == false)
			stopped = false;
		if(softResetClamping(&Model::bodyPos[1], -360, 0, 360, speed * 0.01) == false)
			stopped = false;
		if(softResetClamping(&Model::hipRot[2], -360, 0, 360, speed * 0.5) == false)
			stopped = false;

		if (stopped) {
			jumpSteps = 0;
		}
		
	}
}


// ==================== CUTSCENE ANIMATIONS ====================

// ==================== #1 - Zipline away ====================


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
		Model::bulletPos[2] -= 0.05f;
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