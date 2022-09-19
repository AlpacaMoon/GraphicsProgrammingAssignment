#include "Controls.h"
#include <Windows.h>
#include <gl/GL.h>
#include "Animation.h"
#include "Model.h"

bool Controls::isIndependentControls = false;
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
	if (wParam >= '1' && wParam <= '9') {
		currentControlPart = wParam - '0';
	}

	switch (currentControlPart) {
		// 1) Head
	case 1:
		switch (wParam) {
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
		}

		Model::headRot[2] = clampFloat(Model::headRot[2], -20, 30);
		Model::headRot[1] = clampFloat(Model::headRot[1], -105, 105);
		Model::headRot[0] = clampFloat(Model::headRot[0], -30, 30);
		break;
		// 2) Waist / Hip
	case 2:
		switch (wParam) {
		case 'W':
			Model::hipRot[2] -= rotateSpeed;
			break;
		case 'S':
			Model::hipRot[2] += rotateSpeed;
			break;
		case 'A':
			Model::hipRot[1] -= rotateSpeed;
			break;
		case 'D':
			Model::hipRot[1] += rotateSpeed;
			break;
		case 'Q':
			Model::hipRot[0] += rotateSpeed;
			break;
		case 'E':
			Model::hipRot[0] -= rotateSpeed;
			break;
		}

		Model::hipRot[2] = clampFloat(Model::hipRot[2], -60, 60);
		Model::hipRot[1] = clampFloat(Model::hipRot[1], -35, 35);
		Model::hipRot[0] = clampFloat(Model::hipRot[0], -5, 5);

		break;
		// 3) Right leg
	case 3:
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
		// 4) Left leg
	case 4:
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
		// 5) Right arm
	case 5:
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
		// 6) Left arm
	case 6:
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
			Model::LArmRot[0][0] += rotateSpeed;
			break;
		case 'E':
			Model::LArmRot[0][0] -= rotateSpeed;
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
		Model::LArmRot[0][0] = clampFloat(Model::LArmRot[0][0], 0, 180);
		Model::LArmRot[1][2] = clampFloat(Model::LArmRot[1][2], 0, 160);
		Model::LArmRot[2][2] = clampFloat(Model::LArmRot[2][2], -60, 60);
		Model::LArmRot[2][1] = clampFloat(Model::LArmRot[2][1], -90, 90);
		Model::LArmRot[2][0] = clampFloat(Model::LArmRot[2][0], -90, 90);
		break;
		// 7) Right hand
	case 7:
		break;
		// 8) Left hand
	case 8:
		break;
		// 9) Entire body
	case 9:
		break;
	}
}

void Controls::presetAnimationControls(WPARAM wParam) {

}