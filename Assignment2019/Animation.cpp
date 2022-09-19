#include "Animation.h"


// Main animation switch
bool Animation::isPlayingAnim = false;
bool* Animation::currentPlayingAnim = &isIdle;

// Specific animation statuses
bool Animation::isIdle = true;
bool Animation::isWalking = false;

void Animation::resetAnimation() {
	isPlayingAnim = false;
	currentPlayingAnim = &isIdle;

	isIdle = true;
	isWalking = false;

}

void Animation::switchAnimation(bool* target, bool value) {
	// Disallow modifying isPlayingAnim
	if (target == &isPlayingAnim)
		return;


	// If we change a value from false to true
	if (value == true && *target == false && target != currentPlayingAnim) {
		if (target == &isIdle) {
			resetAnimation();
		}
		else {
			isIdle = false;
			*target = value;
		}
		*currentPlayingAnim = false;
		currentPlayingAnim = target;
	}
	// If we change a value from true to false
	else if (value == false && *target == true && target != &isIdle) {
		resetAnimation();
		*target = value;
		isIdle = true;
		*currentPlayingAnim = false;
		currentPlayingAnim = &isIdle;
	}
	// If we chnage nothing, do nothing. 
}

void Animation::runAnimations() {

}

void Animation::addAnimationToQueue() {

}