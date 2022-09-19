#include "Controls.h"
#include <Windows.h>
#include <gl/GL.h>
#include "Animation.h"
#include "Model.h"

bool Controls::isIndependentControls = true;
int Controls::currentControlPart = 1;
float Controls::rotateSpeed = 5;

float clampFloat(float value, float min, float max) {
	if (value > max)
		return max;
	else if (value < min)
		return min;
	return value;
}

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
	if (wParam >= '1' && wParam <= '6') {
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

		// Head clamping
		Model::headRot[2] = clampFloat(Model::headRot[2], -20, 30);
		Model::headRot[1] = clampFloat(Model::headRot[1], -105, 105);
		Model::headRot[0] = clampFloat(Model::headRot[0], -30, 30);

		// Hip clamping
		Model::hipRot[2] = clampFloat(Model::hipRot[2], -60, 60);
		Model::hipRot[1] = clampFloat(Model::hipRot[1], -35, 35);
		Model::hipRot[0] = clampFloat(Model::hipRot[0], -5, 5);

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
			Model::RLegUpperRot[1] -= rotateSpeed;
			break;
		case 'D':
			Model::RLegUpperRot[1] += rotateSpeed;
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
		Model::RLegUpperRot[2] = clampFloat(Model::RLegUpperRot[2], -90, 110);
		Model::RLegUpperRot[1] = clampFloat(Model::RLegUpperRot[1], -30, 30);
		Model::RLegUpperRot[0] = clampFloat(Model::RLegUpperRot[0], -10, 10);
		Model::RLegHingeRot = clampFloat(Model::RLegHingeRot, -90, 0);
		Model::RFeetRot = clampFloat(Model::RFeetRot, -20, 20);
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
		Model::LLegUpperRot[2] = clampFloat(Model::LLegUpperRot[2], -90, 110);
		Model::LLegUpperRot[1] = clampFloat(Model::LLegUpperRot[1], -30, 30);
		Model::LLegUpperRot[0] = clampFloat(Model::LLegUpperRot[0], -10, 10);
		Model::LLegHingeRot = clampFloat(Model::LLegHingeRot, -100, 0);
		Model::LFeetRot = clampFloat(Model::LFeetRot, -20, 20);
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
		Model::RArmRot[0][2] = clampFloat(Model::RArmRot[0][2], -180, 180);
		Model::RArmRot[0][1] = clampFloat(Model::RArmRot[0][1], -75, 75);
		Model::RArmRot[0][0] = clampFloat(Model::RArmRot[0][0], 0, 180);
		Model::RArmRot[1][2] = clampFloat(Model::RArmRot[1][2], 0, 160);
		Model::RArmRot[2][2] = clampFloat(Model::RArmRot[2][2], -60, 60);
		Model::RArmRot[2][1] = clampFloat(Model::RArmRot[2][1], -90, 90);
		Model::RArmRot[2][0] = clampFloat(Model::RArmRot[2][0], -90, 90);
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
		Model::LArmRot[0][2] = clampFloat(Model::LArmRot[0][2], -180, 180);
		Model::LArmRot[0][1] = clampFloat(Model::LArmRot[0][1], -75, 75);
		Model::LArmRot[0][0] = clampFloat(Model::LArmRot[0][0], -180, 0);
		Model::LArmRot[1][2] = clampFloat(Model::LArmRot[1][2], 0, 160);
		Model::LArmRot[2][2] = clampFloat(Model::LArmRot[2][2], -60, 60);
		Model::LArmRot[2][1] = clampFloat(Model::LArmRot[2][1], -90, 90);
		Model::LArmRot[2][0] = clampFloat(Model::LArmRot[2][0], -90, 90);
		break;
	// 6) Right and left fingers
	case 6:
		switch (wParam) {
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
		// Right hand

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

		for (int i = 0; i < 5; i++) {
			for (int j = 0; j < 3; j++) {
				Model::LFingerRot[i][j] = clampFloat(Model::LFingerRot[i][j], Model::openedFingerRot[i][j], Model::closedFingerRot[i][j]);
				Model::RFingerRot[i][j] = clampFloat(Model::RFingerRot[i][j], Model::openedFingerRot[i][j], Model::closedFingerRot[i][j]);
			}
		}
		break;
	}
}

void Controls::presetAnimationControls(WPARAM wParam) {

}