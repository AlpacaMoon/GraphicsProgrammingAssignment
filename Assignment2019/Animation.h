#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include "Time.h"
class Animation
{
public:

    // Main method (runs all the below mentioned animations)
    static void runAnimations();

    // Weapon related methods
    static void switchWeapon(int n);
    static bool switchingWeapon;

    // Methods to Move body parts
    static void hardReset();
    static bool softReset(float speed);
    static bool softResetClamping(float* target, float min, float frustum, float max, float speed);

    // Reset Animation
    static void cancelAllAnimations();
    static void startReset();
    static bool isResetting;

    // Walking Animation
    static int walkSteps, walkDir;
    static float walkSpeed, walkBodyShifting;
    static void startWalking();
    static void stopWalking();
    static void rotateWalk();
    static void walk();

    // Jumping Animation
    static int jumpSteps;
    static float jumpSpeed, movingBodyPartSpeed, jumpHeight;
    static bool isJumping();
    static void startJumping();
    static void jump();


    // Hand Animations
    static float fingerSpeed;
    static bool closeRightHand(float fingerSpeed);
    static bool closeLeftHand(float fingerSpeed);
    static bool openRightHand(float fingerSpeed);
    static bool openLeftHand(float fingerSpeed);

    // Clamping rotations / transformations
    static float clampFloat(float value, float min, float max);
    static void clampHead();
    static void clampHip();
    static void clampBody();
    static void clampRightLeg();
    static void clampLeftLeg();
    static void clampRightArm();
    static void clampLeftArm();
    static void clampRightFingers();
    static void clampLeftFingers();

    // Weapon R99
    static void shootBullet();
    static Time gunFireCooldownTime;
 
    // Weapon Kukri Knife
    static void startSwingKnife();
    static bool isSwingingKnife();
    static void swingKnife();
    static Time knifeCooldownTime;
    static int knifeAttackSteps;

    // TV screen
    static int currentTVscreen;
    static void switchTVscreen(int n);
};