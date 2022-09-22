#include "Controls.h"
#include <Windows.h>
#include <gl/GL.h>
#include "Animation.h"
#include "Model.h"

bool Controls::isIndependentControls = true;
int Controls::currentControlPart = 1;
float Controls::rotateSpeed = 5;
int Controls::currentPlayingAnimation = 1;

bool Controls::pressingWalkKeys[3] = {false, false, false};

void Controls::initialize() {
	isIndependentControls = true;
	currentControlPart = 1;
}

void Controls::reset() {
	if (isIndependentControls) {
		currentControlPart = 1;
	}
	else {

	}
}

void Controls::independentControls(WPARAM wParam) {
	if (!isIndependentControls)
		return;

	// Switch control parts
	if (wParam >= '1' && wParam <= '7') {
		currentControlPart = wParam - '0';
	}

	switch (currentControlPart) {
	// 1) Head, Hip and Body control
	case 1:
		switch (wParam) {
			// Head
		case 'W':
			Model::headRot[2] += rotateSpeed;
			break;
		case 'S':
			Model::headRot[2] -= rotateSpeed;
			break;
		case 'A':
			Model::headRot[1] -= rotateSpeed;
			break;
		case 'D':
			Model::headRot[1] += rotateSpeed;
			break;
		case 'Q':
			Model::headRot[0] += rotateSpeed;
			break;
		case 'E':
			Model::headRot[0] -= rotateSpeed;
			break;
			// Hip
		case 'T':
			Model::hipRot[2] -= rotateSpeed;
			break;
		case 'G':
			Model::hipRot[2] += rotateSpeed;
			break;
		case 'F':
			Model::hipRot[1] -= rotateSpeed;
			break;
		case 'H':
			Model::hipRot[1] += rotateSpeed;
			break;
		case 'R':
			Model::hipRot[0] += rotateSpeed;
			break;
		case 'Y':
			Model::hipRot[0] -= rotateSpeed;
			break;
			// Body
		case 'I':
			Model::bodyRot[2] -= rotateSpeed;
			break;
		case 'K':
			Model::bodyRot[2] += rotateSpeed;
			break;
		case 'J':
			Model::bodyRot[1] -= rotateSpeed;
			break;
		case 'L':
			Model::bodyRot[1] += rotateSpeed;
			break;
		case 'U':
			Model::bodyRot[0] += rotateSpeed;
			break;
		case 'O':
			Model::bodyRot[0] -= rotateSpeed;
			break;
		}

		// Clamping rotations
		Animation::clampHead();
		Animation::clampHip();
		Animation::clampBody();

		break;
	// 2) Right leg
	case 2:
		switch (wParam) {
		case 'W':
			Model::RLegUpperRot[2] += rotateSpeed;
			break;
		case 'S':
			Model::RLegUpperRot[2] -= rotateSpeed;
			break;
		case 'A':
			Model::RLegUpperRot[1] += rotateSpeed;
			break;
		case 'D':
			Model::RLegUpperRot[1] -= rotateSpeed;
			break;
		case 'Q':
			Model::RLegUpperRot[0] += rotateSpeed;
			break;
		case 'E':
			Model::RLegUpperRot[0] -= rotateSpeed;
			break;
		case 'R':
			Model::RLegHingeRot += rotateSpeed;
			break;
		case 'F':
			Model::RLegHingeRot -= rotateSpeed;
			break;
		case 'T':
			Model::RFeetRot += rotateSpeed;
			break;
		case 'G':
			Model::RFeetRot -= rotateSpeed;
			break;
		}
		Animation::clampRightLeg();
		break;
	// 3) Left leg
	case 3:
		switch (wParam) {
		case 'W':
			Model::LLegUpperRot[2] += rotateSpeed;
			break;
		case 'S':
			Model::LLegUpperRot[2] -= rotateSpeed;
			break;
		case 'A':
			Model::LLegUpperRot[1] -= rotateSpeed;
			break;
		case 'D':
			Model::LLegUpperRot[1] += rotateSpeed;
			break;
		case 'Q':
			Model::LLegUpperRot[0] += rotateSpeed;
			break;
		case 'E':
			Model::LLegUpperRot[0] -= rotateSpeed;
			break;
		case 'R':
			Model::LLegHingeRot += rotateSpeed;
			break;
		case 'F':
			Model::LLegHingeRot -= rotateSpeed;
			break;
		case 'T':
			Model::LFeetRot += rotateSpeed;
			break;
		case 'G':
			Model::LFeetRot -= rotateSpeed;
			break;
		}
		Animation::clampLeftLeg();
		break;
	// 4) Right arm
	case 4:
		switch (wParam) {
		case 'W':
			Model::RArmRot[0][2] += rotateSpeed;
			break;
		case 'S':
			Model::RArmRot[0][2] -= rotateSpeed;
			break;
		case 'A':
			Model::RArmRot[0][1] -= rotateSpeed;
			break;
		case 'D':
			Model::RArmRot[0][1] += rotateSpeed;
			break;
		case 'Q':
			Model::RArmRot[0][0] -= rotateSpeed;
			break;
		case 'E':
			Model::RArmRot[0][0] += rotateSpeed;
			break;
		case 'R':
			Model::RArmRot[1][2] += rotateSpeed;
			break;
		case 'F':
			Model::RArmRot[1][2] -= rotateSpeed;
			break;
		case 'I':
			Model::RArmRot[2][2] += rotateSpeed;
			break;
		case 'K':
			Model::RArmRot[2][2] -= rotateSpeed;
			break;
		case 'J':
			Model::RArmRot[2][1] -= rotateSpeed;
			break;
		case 'L':
			Model::RArmRot[2][1] += rotateSpeed;
			break;
		case 'U':
			Model::RArmRot[2][0] += rotateSpeed;
			break;
		case 'O':
			Model::RArmRot[2][0] -= rotateSpeed;
			break;
		}
		Animation::clampRightArm();
		break;
	// 5) Left arm
	case 5:
		switch (wParam) {
		case 'W':
			Model::LArmRot[0][2] += rotateSpeed;
			break;
		case 'S':
			Model::LArmRot[0][2] -= rotateSpeed;
			break;
		case 'A':
			Model::LArmRot[0][1] -= rotateSpeed;
			break;
		case 'D':
			Model::LArmRot[0][1] += rotateSpeed;
			break;
		case 'Q':
			Model::LArmRot[0][0] -= rotateSpeed;
			break;
		case 'E':
			Model::LArmRot[0][0] += rotateSpeed;
			break;
		case 'R':
			Model::LArmRot[1][2] += rotateSpeed;
			break;
		case 'F':
			Model::LArmRot[1][2] -= rotateSpeed;
			break;
		case 'I':
			Model::LArmRot[2][2] += rotateSpeed;
			break;
		case 'K':
			Model::LArmRot[2][2] -= rotateSpeed;
			break;
		case 'J':
			Model::LArmRot[2][1] -= rotateSpeed;
			break;
		case 'L':
			Model::LArmRot[2][1] += rotateSpeed;
			break;
		case 'U':
			Model::LArmRot[2][0] += rotateSpeed;
			break;
		case 'O':
			Model::LArmRot[2][0] -= rotateSpeed;
			break;
		}
		Animation::clampLeftArm();
		break;
	// 6) Right and left fingers
	case 6:
		switch (wParam) {
			// ===== LEFT FINGERS =====
		// Left pinky
		case 'Q':
			for (int i = 0; i < 3; i++)
				Model::LFingerRot[4][i] -= rotateSpeed;
			break;
		case 'A':
			for (int i = 0; i < 3; i++)
				Model::LFingerRot[4][i] += rotateSpeed;
			break;
		// Left ring
		case 'W':
			for (int i = 0; i < 3; i++)
				Model::LFingerRot[3][i] -= rotateSpeed;
			break;
		case 'S':
			for (int i = 0; i < 3; i++)
				Model::LFingerRot[3][i] += rotateSpeed;
			break;
		// Left middle
		case 'E':
			for (int i = 0; i < 3; i++)
				Model::LFingerRot[2][i] -= rotateSpeed;
			break;
		case 'D':
			for (int i = 0; i < 3; i++)
				Model::LFingerRot[2][i] += rotateSpeed;
			break;
		// Left index
		case 'R':
			for (int i = 0; i < 3; i++)
				Model::LFingerRot[1][i] -= rotateSpeed;
			break;
		case 'F':
			for (int i = 0; i < 3; i++)
				Model::LFingerRot[1][i] += rotateSpeed;
			break;
		// Left thumb
		case 'T':
			for (int i = 0; i < 3; i++)
				Model::LFingerRot[0][i] -= rotateSpeed;
			break;
		case 'G':
			for (int i = 0; i < 3; i++)
				Model::LFingerRot[0][i] += rotateSpeed;
			break;

			// ===== RIGHT FINGERS =====
		// Right thumb
		case 'Y':
			for (int i = 0; i < 3; i++)
				Model::RFingerRot[0][i] -= rotateSpeed;
			break;
		case 'H':
			for (int i = 0; i < 3; i++)
				Model::RFingerRot[0][i] += rotateSpeed;
			break;
		// Right index
		case 'U':
			for (int i = 0; i < 3; i++)
				Model::RFingerRot[1][i] -= rotateSpeed;
			break;
		case 'J':
			for (int i = 0; i < 3; i++)
				Model::RFingerRot[1][i] += rotateSpeed;
			break;
		// Right middle
		case 'I':
			for (int i = 0; i < 3; i++)
				Model::RFingerRot[2][i] -= rotateSpeed;
			break;
		case 'K':
			for (int i = 0; i < 3; i++)
				Model::RFingerRot[2][i] += rotateSpeed;
			break;
		// Right ring
		case 'O':
			for (int i = 0; i < 3; i++)
				Model::RFingerRot[3][i] -= rotateSpeed;
			break;
		case 'L':
			for (int i = 0; i < 3; i++)
				Model::RFingerRot[3][i] += rotateSpeed;
			break;
		// Right pinky
		case 'P':
			for (int i = 0; i < 3; i++)
				Model::RFingerRot[4][i] -= rotateSpeed;
			break;
		case VK_OEM_1:
			for (int i = 0; i < 3; i++)
				Model::RFingerRot[4][i] += rotateSpeed;
			break;
		}
		Animation::clampRightFingers();
		Animation::clampLeftFingers();
		break;
		// Miscellaneous
	case 7:
		switch (wParam) {
		case 'Q':
			Animation::switchTVscreen();
		}
		break;
	}
}

void Controls::presetAnimationKeyDown(WPARAM wParam) {

	switch (wParam) {
	case 'W':
		pressingWalkKeys[0] = true;
		break;
	case 'A':
		pressingWalkKeys[1] = true;
		break;
	case 'D':
		pressingWalkKeys[2] = true;
		break;

	case VK_SPACE:
		// Initiate jump if currently is not in the middle of jumping
		if (!Animation::isJumping()) {
			Animation::cancelAllAnimations();
			Animation::startJumping();
		}
		break;

	case VK_OEM_PERIOD:
		Animation::stopWalking();
		Animation::hardReset();
		break;

	default:
		break;
	}


}

void Controls::presetAnimationKeyUp(WPARAM wParam) {
	
	switch (wParam) {
	case 'W':
		pressingWalkKeys[0] = false;
		break;
	case 'A':
		pressingWalkKeys[1] = false;
		break;
	case 'D':
		pressingWalkKeys[2] = false;
		break;
	default:
		break;
	}

	if (!Animation::isJumping() && !isPressingWalk()) {
		Animation::stopWalking();
	}
}

bool Controls::isPressingWalk() {
	for (int i = 0; i < 3; i++) {
		if (pressingWalkKeys[i] == true) {
			return true;
		}
	}
	return false;
}