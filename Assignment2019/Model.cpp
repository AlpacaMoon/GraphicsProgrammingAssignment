#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "Model.h"
#include "Utility.h"
#include "CoordinateSet.h"
#include "Color.h"

float xAxis[3] = { 1, 0, 0 };
float yAxis[3] = { 0, 1, 0 };
float zAxis[3] = { 0, 0, 1 };

float legThickness = 0.1f;
float armThickness = 0.15f;

// GLUquadricObj variables
GLUquadricObj* legLowerSpring;
GLUquadricObj* legPlateObj;

GLUquadricObj* armUpperObj;

void Model::RightLeg() {
	glPushMatrix();
	{
		glPushMatrix();
		{
			glTranslatef(0, 0.85, 0);
			glScalef(0.9, 0.9, 1.0);
			LegUpper();
		}
		glPopMatrix();
		glPushMatrix();
		{
			LegLower();
		}
		glPopMatrix();
		glPushMatrix();
		{
			glTranslatef(0.22, -0.82, 0);
			LegPlate();
		}
		glPopMatrix();
	}
	glPopMatrix();
}

void Model::LegUpper() {
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

	CoordinateSet line2(10);
	line1.getCoords(line1.numberOfCoords, lastCoordTemp);
	line2.addCoordinate(lastCoordTemp);
	line2.addCoordinate(0.3, -0.7, 0);
	line2.addCoordinate(0.3, -0.9, 0);
	line2 = Utility::bezierCurveCoords(line2, 4);

	CoordinateSet line3(10);
	line3.addCoordinate(0.4, -1.0, 0);
	line3.addCoordinate(0.5, -1.0, 0);
	line3.addCoordinate(0.5, -0.5, 0);
	line3.addCoordinate(0.5, -0.32, 0);

	circle.combineCoords(line1);
	circle.combineCoords(line2);
	circle.combineCoords(line3);

	Color::lightBlue();
	float polygonCenter[3] = { 0.35, -0.35, 0 };
	Utility::extrudePolygon(circle, polygonCenter, zAxis, legThickness);

	// Hinge
	Color::lightGray();
	glPushMatrix();
	{
		float hingeThickness = 0.015f;
		float hingeCenter[3] = { 0.44, -1.01, -hingeThickness };
		float hingeRad = 0.06f;
		CoordinateSet hinge = Utility::circleCoords(hingeCenter, hingeRad, 16);
		glPushMatrix();
		{
			Utility::extrudePolygon(hinge, hingeCenter, zAxis, hingeThickness, true, false);
			glTranslatef(0, 0, hingeThickness + legThickness);
			Utility::extrudePolygon(hinge, hingeCenter, zAxis, hingeThickness, false, true);
		}
		glPopMatrix();

		// Only do if got time
		//// Hinge support
		//glTranslatef(hingeCenter[0], hingeCenter[1], hingeCenter[2]);
		//CoordinateSet hingeBar1(10);
		//hingeBar1.addCoordinate(0, 0, 0);
		//hingeBar1.addCoordinate(-0.075f, 0.05f, 0);
		//hingeBar1.addCoordinate(-0.1f, 0.05f, 0);
		//hingeBar1.addCoordinate(-0.05f, 0, 0);
		//Utility::drawPolygon();

		//hingeBar1.addCoordinate(-0.05f, 0, 0);
		//hingeBar1.addCoordinate(0, 0, 0);
		//hingeBar1.addCoordinate(-0.075f, 0.05f, 0);
		//hingeBar1.addCoordinate(-0.072f, 0.16f, 0);
		//float c1[3] = {-0.02f, 0.275f, 0};
		//hingeBar1.combineCoords(Utility::circleCoords(c1, 0.02f, 4, -45, 135));
		//hingeBar1.addCoordinate(-0.1f, 0.18f, 0);
		//hingeBar1.addCoordinate(-0.1f, 0.5f, 0);

	}
	glPopMatrix();
}

void Model::LegLower() {
	float radius = 0.06;
	float center[3] = { 0.41, -radius, 0 };
	float lastCoordTemp[3];
	CoordinateSet tempCoordSet(10);

	Color::lightBlue();

	// Draw Leg part 1
	CoordinateSet line1(10);
	line1.addCoordinate(0.35, -0.19, 0);
	line1.combineCoords(Utility::circleCoords(center, radius, 8, -60, 135));
	line1.addCoordinate(0.25, -0.06, 0);
	line1.getCoords(line1.numberOfCoords, lastCoordTemp);
	tempCoordSet.addCoordinate(0.2, -0.08, 0);
	tempCoordSet.addCoordinate(0.09, -0.08, 0);
	line1.combineCoords(Utility::bezierCurveCoords(tempCoordSet, 4));
	line1.addCoordinate(0.075, -0.15, 0);
	line1.addCoordinate(0.16, -0.19, 0);

	float legP1Center[3] = { 0.25, -0.15, 0 };
	Utility::extrudePolygon(line1, legP1Center, zAxis, legThickness);

	// Draw Leg Part 2
	CoordinateSet longLeft(10);
	CoordinateSet longRight(10);
	tempCoordSet.clear(10);

	line1.getCoords(line1.numberOfCoords, lastCoordTemp);
	tempCoordSet.addCoordinate(lastCoordTemp);
	tempCoordSet.addCoordinate(0.14, -0.45, 0);
	tempCoordSet.addCoordinate(0.15, -0.62, 0);
	longLeft = Utility::bezierCurveCoords(tempCoordSet, 4);
	longLeft.addCoordinate(0.21, -0.64, 0);

	line1.getCoords(0, lastCoordTemp);
	tempCoordSet.clear(10);
	tempCoordSet.addCoordinate(0.35, -0.19, 0);
	tempCoordSet.addCoordinate(0.25, -0.45, 0);
	tempCoordSet.addCoordinate(0.28, -0.64, 0);
	longRight.combineCoords(Utility::bezierCurveCoords(tempCoordSet, 6));

	Utility::extrudeConcavePolygon(longLeft, longRight, 0.25, zAxis, legThickness);

	// Draw leg Part 3
	tempCoordSet.clear(10);

	longLeft.getCoords(longLeft.numberOfCoords, lastCoordTemp);
	tempCoordSet.addCoordinate(lastCoordTemp);
	tempCoordSet.addCoordinate(0.235, -0.7, 0);
	tempCoordSet.addCoordinate(0.27, -0.75, 0);
	longLeft = Utility::bezierCurveCoords(tempCoordSet, 4);
	longLeft.addCoordinate(0.26, -0.8, 0);

	tempCoordSet.clear(10);
	longRight.getCoords(longRight.numberOfCoords, lastCoordTemp);
	tempCoordSet.addCoordinate(lastCoordTemp);
	tempCoordSet.addCoordinate(0.3, -0.695, 0);
	tempCoordSet.addCoordinate(0.34, -0.75, 0);
	longRight = Utility::bezierCurveCoords(tempCoordSet, 4);
	longRight.addCoordinate(0.325, -0.8, 0);

	Utility::extrudeConcavePolygon(longLeft, longRight, 0.275, zAxis, legThickness);

	// Red spring at the bottom
	if (legLowerSpring == NULL) {
		legLowerSpring = gluNewQuadric();
	}
	Color::red();
	{
		glTranslatef(0.295, -0.8, legThickness / 2.0f);
		glRotatef(90, 1, 0, 0);
		gluCylinder(legLowerSpring, 0.018f, 0.018f, 0.08f, 8, 1);
	}
}

void Model::LegPlate() {
	float feetWidth = 0.1f;
	Color::blue();

	if (legPlateObj == NULL) {
		legPlateObj = gluNewQuadric();
	}

	// Leg plate
	CoordinateSet plate(10);
	plate.addCoordinate(0, 0, 0);
	plate.addCoordinate(0, -0.08, 0);
	plate.addCoordinate(0.275, -0.08, 0);
	plate.addCoordinate(0.275, -0.04, 0);
	plate.addCoordinate(0.14, -0.03, 0);

	float plateCenter[3] = { 0.14, -0.06, 0 };
	Utility::extrudePolygon(plate, plateCenter, zAxis, feetWidth);

	// Ankle
	glPushMatrix();
	{
		float extrusion = 0.01f;
		Color::black();
		float ankleCenter[3] = { 0, 0, 0 };
		CoordinateSet ankle = Utility::circleCoords(ankleCenter, 0.025f, 12);
		glTranslatef(0, 0, -extrusion);
		Utility::extrudePolygon(ankle, ankleCenter, zAxis, feetWidth + extrusion * 2);
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
		bar.combineCoords(Utility::circleCoords(barSmallCircle, barSmallCircleRadius, 8, 80.0f, 260.0f).reverse());
		bar.addCoordinate(0.03, 0.035, 0);
		bar.addCoordinate(0.035, 0.03, 0);
		bar.addCoordinate(0.035, -0.01, 0);
		float barCenter[3] = { 0.025, 0.015, 0 };

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
			Color::cyan();
			glTranslatef(0, 0, feetWidth - inset - barWidth);
			Utility::extrudePolygon(bar, barCenter, zAxis, barWidth);
		}
		glPopMatrix();
		// Horizontal support bar
		float supportBarRadius = 0.008f;
		glPushMatrix();
		{
			Color::yellow();
			glTranslatef(barSmallCircle[0], barSmallCircle[1], barSmallCircle[2] + inset + barWidth);
			gluCylinder(legPlateObj, supportBarRadius, supportBarRadius, feetWidth - inset * 2 - barWidth * 2, 6, 1);
		}
		glPopMatrix();
		// Large support cylinder
		glPushMatrix();
		{
			Color::black();
			glTranslatef(barSmallCircle[0], barSmallCircle[1], barSmallCircle[2] + feetWidth / 2.0f);
			glRotatef(-15, 0, 0, 1);
			glTranslatef(0, supportBarRadius, 0);
			glRotatef(-90, 1, 0, 0);
			gluCylinder(legPlateObj, 0.025, 0.025, 0.14, 12, 1);
		}
		glPopMatrix();
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
		Utility::extrudePolygon(arm, armCenter, zAxis, armThickness);

		// Shoulder Hinge
		Color::lightGray();
		float shoulderRad = 0.12f;
		glPushMatrix();
		{
			glTranslatef(1.08f, -0.25f, armThickness * 0.f);
			gluSphere(armUpperObj, shoulderRad, 8, 8);
		}
	}
	glPopMatrix();
}

void Model::ArmLower() {

}

void Model::Hand() {

}

void Model::r99() {
	float centre1[3] = { 0,0,0 };
	float centre2[3] = { 0,0,0 };
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
	barrelTop.addCoordinate(-0.05, -0.02, 0);
	barrelTop.addCoordinate(0.05, -0.02, 0);
	barrelTop.addCoordinate(0.05, 0.02, 0);
	barrelTop.addCoordinate(-0.05, 0.02, 0);

	CoordinateSet handle1(10);
	handle1.addCoordinate(-0.05, -0.08, 0);
	handle1.addCoordinate(-0.04, -0.1, 0);
	handle1.addCoordinate(0.04, -0.1, 0);
	handle1.addCoordinate(0.05, -0.08, 0);
	handle1.addCoordinate(0.05, 0, 0);
	handle1.addCoordinate(-0.05, 0, 0);

	glPushMatrix();
	Utility::extrudePolygon(barrelCoors, centre1, zAxis, 0.6f, true, true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.069, 0, 0);
	Utility::extrudePolygon(barrelSide, centre1, zAxis, 0.2f, true, true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.069, 0, 0);
	Utility::extrudePolygon(barrelSide, centre1, zAxis, 0.2f, true, true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.075, 0.05);
	Utility::extrudePolygon(barrelBottom, centre1, zAxis, 0.2f, true, true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, 0.07, 0.07);
	Utility::extrudePolygon(barrelTop, centre1, zAxis, 0.48f, true, true);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -0.1, 0.05);
	Utility::extrudePolygon(handle1, centre1, zAxis, 0.03f, true, true);
	glPopMatrix();

	//barrel head
	glPushMatrix();
	centre1[0] = 0;
	centre1[1] = 0;
	normal1[2] = 1;

	centre2[2] = 0.08;
	normal2[2] = 1;
	//Utility::drawTube(centre1, normal1, 0.02, centre2, normal2, 0.02, 20);
	glPopMatrix();


}