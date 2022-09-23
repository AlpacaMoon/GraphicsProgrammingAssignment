
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include <math.h>
#include <list>

#include "Utility.h"
#include "CoordinateSet.h"
#include "TextureMap.h"
#include "Texture.h"

using namespace std;

const float PI = 3.1415926f;
float worldOrigin[3] = { 0, 0, 0 };

GLUquadricObj* hemiSphereObj;

// Convert degrees to radians
float Utility::degToRad(float deg) {
	return deg * PI / 180.0f;
}

// Convert radians to degrees
float Utility::radToDeg(float rad) {
	return rad * 180.0f / PI;
}

void Utility::drawLine(CoordinateSet coords) {
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < coords.numberOfCoords; i++)
		glVertex3f(coords.xCoords[i], coords.yCoords[i], coords.zCoords[i]);
	glEnd();
}

/*	Given the center, normal and radius,
*	Draw a circle that faces towards the normal using GL_LINE_LOOP
*/
void Utility::drawCircle(float center[3], float normal[3], float radius, int edges) {
	glPushMatrix();
	// Translate to correct position
	glTranslatef(center[0], center[1], center[2]);

	// Rotate the circle to face the normal
	float t[3] = { 0, 0, 1 };
	float rotateAxis[3];
	crossProduct(t, normal, rotateAxis);
	glRotatef(angleBetweenTwoVectors(t, normal), rotateAxis[0], rotateAxis[1], rotateAxis[2]);

	// Draw the circle
	glBegin(GL_LINE_LOOP);
	float rot = 0;
	float inc = 360.0f / edges;
	for (int i = 0; i < edges; i++) {
		glVertex3f(cos(degToRad(rot)) * radius, sin(degToRad(rot)) * radius, 0);
		rot += inc;
	}
	glEnd();
	glPopMatrix();
}

/*	Draw bezier line based on coordinates, points and divisions given
*		- Coordinates = The coordinates of the control points
*		- Points = The number of control points
*		- Divisions = The smoothness of the bezier line / The number of straight line segments on the bezier line
*/
void Utility::drawBezierLine(CoordinateSet coordSet, int divisions) {
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < divisions; i++) {
		float t = (float)i / divisions;
		float output[3];
		bezier3d(t, coordSet, output);
		glVertex3f(output[0], output[1], output[2]);
	}
	glEnd();
}

/*	Draw circles at intervals around a bezier line
*		- The circles drawn will be facing the bezier line's tangent at that point
*		- Used mainly for debugging purposes
*/
void Utility::drawCirclesAroundBezier(CoordinateSet coordSet, int divisions, float radius) {
	float thisPoint[3], tangent[3];
	for (int i = 0; i < divisions + 1; i++) {
		// Calculate current point using bezier curve formula
		float t = (float)i / divisions;
		if (t == 0)
			t += 0.001f;
		else if (t == 1)
			t -= 0.001f;
		bezier3d(t, coordSet, thisPoint);

		// Obtain tangent of this point
		bezierDerivative3d(t, coordSet, tangent);

		float l = 0.25f;
		glColor3f(1, 1, 0);
		glBegin(GL_LINES);
		glVertex3f(thisPoint[0], thisPoint[1], thisPoint[2]);
		glVertex3f(thisPoint[0] + tangent[0] * l, thisPoint[1] + tangent[1] * l, thisPoint[2] + tangent[2] * l);
		glEnd();

		// Draw circle using tangent as normal
		glColor3f(0, 1, 0);
		drawCircle(thisPoint, tangent, radius, 16);
	}
}

/*	Given the center, normal and radius of two independent circles in 3d space,
*	draw a tube that uses quads as its faces
*		- This function is different from gluCylinder as in this function
*			allows the top and bottom of the cylinder to be non-parallel to each other
*/
void Utility::drawTube(float centerA[3], float normalA[3], float radiusA, float centerB[3], float normalB[3], float radiusB, int faces) {
	// Error handling
	if (faces >= -2 && faces <= 2) return;

	float basisA1[3], basisA2[3], basisB1[3], basisB2[3];
	float thisCenterA[3], thisCenterB[3];
	glPushMatrix();
	
	// For some reason the tube will get shrinked to infinity when it is parallel to Z-axis (the bug is probably in orthogonalBasis())
	// So instead of solving the bug, here I just rotate the tube 90 degrees so that it isn't parallel to Z-axis
	float zAxis[3] = { 0, 0, 1 };
	if (isParallelVector(normalA, zAxis) || isParallelVector(normalB, zAxis)) {
		glRotatef(-90, 0, 1, 0);

		float newNormalA[3], newNormalB[3], newCenterA[3], newCenterB[3];
		rotateAroundYaxis(normalA, 90, newNormalA);
		rotateAroundYaxis(normalB, 90, newNormalB);
		rotateAroundYaxis(centerA, 90, newCenterA);
		rotateAroundYaxis(centerB, 90, newCenterB);

		// Calculate the orthogonal basis vectors of normalA and normalB
		orthogonalBasis(newNormalA, basisA1, basisA2);
		orthogonalBasis(newNormalB, basisB1, basisB2);

		for (int i = 0; i < 3; i++) {
			thisCenterA[i] = newCenterA[i];
			thisCenterB[i] = newCenterB[i];
		}
	}
	// Doesn't need to rotate if it isn't parallel to Z-axis
	else {
		// Calculate the orthogonal basis vectors of normalA and normalB
		orthogonalBasis(normalA, basisA1, basisA2);
		orthogonalBasis(normalB, basisB1, basisB2);

		for (int i = 0; i < 3; i++) {
			thisCenterA[i] = centerA[i];
			thisCenterB[i] = centerB[i];
		}
	}

	// Calculate rotation increment between faces
	float increment = 360.0f / faces;
	float rot = 0.0f;
	float temp[3];

	// Draw n amount of faces, where n = the int variable "faces"
	// Each face have 4 points, #1, #2, #3 and #4
	for (int i = 0; i < faces; i++) {
		glBegin(GL_QUADS);

		// Draw #1 that lies on first circle, with angle = rot
		for (int i = 0; i < 3; i++)
			temp[i] = thisCenterA[i] + radiusA * (basisA1[i] * cos(degToRad(rot)) + basisA2[i] * sin(degToRad(rot)));
		drawVertex(temp[0], temp[1], temp[2], thisCenterA);

		// Draw #2 that lines on first circle, with angle = rot + increment
		for (int i = 0; i < 3; i++)
			temp[i] = thisCenterA[i] + radiusA * (basisA1[i] * cos(degToRad(rot + increment)) + basisA2[i] * sin(degToRad(rot + increment)));
		drawVertex(temp[0], temp[1], temp[2], thisCenterA);

		// Draw #3 that lines on second circle, with angle = rot + increment
		for (int i = 0; i < 3; i++)
			temp[i] = thisCenterB[i] + radiusB * (basisB1[i] * cos(degToRad(rot + increment)) + basisB2[i] * sin(degToRad(rot + increment)));
		drawVertex(temp[0], temp[1], temp[2], thisCenterB);

		// Draw #4 that lines on second circle, with angle = rot
		for (int i = 0; i < 3; i++)
			temp[i] = thisCenterB[i] + radiusB * (basisB1[i] * cos(degToRad(rot)) + basisB2[i] * sin(degToRad(rot)));
		drawVertex(temp[0], temp[1], temp[2], thisCenterB);

		glEnd();

		// Increment rotation / theta
		rot += increment;
	}

	glPopMatrix();
}

void Utility::drawTube(float centerA[3], float normalA[3], float radiusA, float centerB[3], float normalB[3], float radiusB, int faces, GLuint texture) {
	// Error handling
	if (faces >= -2 && faces <= 2) return;

	float basisA1[3], basisA2[3], basisB1[3], basisB2[3];
	float thisCenterA[3], thisCenterB[3];
	glPushMatrix();

	// For some reason the tube will get shrinked to infinity when it is parallel to Z-axis (the bug is probably in orthogonalBasis())
	// So instead of solving the bug, here I just rotate the tube 90 degrees so that it isn't parallel to Z-axis
	float zAxis[3] = { 0, 0, 1 };
	if (isParallelVector(normalA, zAxis) || isParallelVector(normalB, zAxis)) {
		glRotatef(-90, 0, 1, 0);

		float newNormalA[3], newNormalB[3], newCenterA[3], newCenterB[3];
		rotateAroundYaxis(normalA, 90, newNormalA);
		rotateAroundYaxis(normalB, 90, newNormalB);
		rotateAroundYaxis(centerA, 90, newCenterA);
		rotateAroundYaxis(centerB, 90, newCenterB);

		// Calculate the orthogonal basis vectors of normalA and normalB
		orthogonalBasis(newNormalA, basisA1, basisA2);
		orthogonalBasis(newNormalB, basisB1, basisB2);

		for (int i = 0; i < 3; i++) {
			thisCenterA[i] = newCenterA[i];
			thisCenterB[i] = newCenterB[i];
		}
	}
	// Doesn't need to rotate if it isn't parallel to Z-axis
	else {
		// Calculate the orthogonal basis vectors of normalA and normalB
		orthogonalBasis(normalA, basisA1, basisA2);
		orthogonalBasis(normalB, basisB1, basisB2);

		for (int i = 0; i < 3; i++) {
			thisCenterA[i] = centerA[i];
			thisCenterB[i] = centerB[i];
		}
	}

	// Calculate rotation increment between faces
	float increment = 360.0f / faces;
	float rot = 0.0f;
	float temp[3];

	// Draw n amount of faces, where n = the int variable "faces"
	// Each face have 4 points, #1, #2, #3 and #4
	for (int i = 0; i < faces; i++) {
		Texture::on();
		Texture::use(texture);
		glBegin(GL_QUADS);

		// Draw #1 that lies on first circle, with angle = rot
		for (int i = 0; i < 3; i++)
			temp[i] = thisCenterA[i] + radiusA * (basisA1[i] * cos(degToRad(rot)) + basisA2[i] * sin(degToRad(rot)));
		glTexCoord2f(0, 0);
		drawVertex(temp[0], temp[1], temp[2], thisCenterA);

		// Draw #2 that lines on first circle, with angle = rot + increment
		for (int i = 0; i < 3; i++)
			temp[i] = thisCenterA[i] + radiusA * (basisA1[i] * cos(degToRad(rot + increment)) + basisA2[i] * sin(degToRad(rot + increment)));
		glTexCoord2f(0, 1);
		drawVertex(temp[0], temp[1], temp[2], thisCenterA);

		// Draw #3 that lines on second circle, with angle = rot + increment
		for (int i = 0; i < 3; i++)
			temp[i] = thisCenterB[i] + radiusB * (basisB1[i] * cos(degToRad(rot + increment)) + basisB2[i] * sin(degToRad(rot + increment)));
		glTexCoord2f(1, 1);
		drawVertex(temp[0], temp[1], temp[2], thisCenterB);

		// Draw #4 that lines on second circle, with angle = rot
		for (int i = 0; i < 3; i++)
			temp[i] = thisCenterB[i] + radiusB * (basisB1[i] * cos(degToRad(rot)) + basisB2[i] * sin(degToRad(rot)));
		glTexCoord2f(1, 0);
		drawVertex(temp[0], temp[1], temp[2], thisCenterB);

		glEnd();
		Texture::off();

		// Increment rotation / theta
		rot += increment;
	}

	glPopMatrix();
}

/*	Given the bezier line coords,
*	Draw a cylindrical tube that follows the bezier line
*		- All radii on the tube are the same
*		- int lineSmoothness = the amount of divisions on the bezier line itself
*		- float radius = the radius of the tube
*		- int faces = the amount of faces on one segment of tube
*/
void Utility::drawBezierTube(CoordinateSet coordSet, int lineSmoothness, int faces, float radius) {
	// Error handling
	if (lineSmoothness < 1) return;

	float pointA[3], pointB[3], tangentA[3], tangentB[3];
	float t;
	float control = 0.001f;

	// Calculate first point's coordinate and its tangent
	//	* Note that we're using 0.0001f instead of 0.0f to avoid the tangent = 0, which results in tangent = Z-axis
	bezier3d(control, coordSet, pointA);
	bezierDerivative3d(control, coordSet, tangentA);

	for (int i = 1; i < lineSmoothness; i++) {
		t = (float)i / lineSmoothness;

		// Calculate second point's coordinate and its tangent
		bezier3d(t, coordSet, pointB);
		bezierDerivative3d(t, coordSet, tangentB);

		// Draw tube segment
		drawTube(pointA, tangentA, radius, pointB, tangentB, radius, faces);

		// Copy second point to first point (to be used in the next loop & save resources)
		for (int i = 0; i < 3; i++) {
			pointA[i] = pointB[i];
			tangentA[i] = tangentB[i];
		}
	}

	// Calculate last point's coordinate and its tangent
	//	* Note that we're using 0.9999f instead of 1.0f to avoid the tangent = 0, which results in tangent = Z-axis
	bezier3d(1.0f - control, coordSet, pointB);
	bezierDerivative3d(1.0f - control, coordSet, tangentB);

	// Draw the last tube and voila!
	drawTube(pointA, tangentA, radius, pointB, tangentB, radius, faces);
}

void Utility::drawBezierTube(CoordinateSet coordSet, int lineSmoothness, int faces, float radius, GLuint texture) {
	// Error handling
	if (lineSmoothness < 1) return;

	float pointA[3], pointB[3], tangentA[3], tangentB[3];
	float t;
	float control = 0.001f;

	// Calculate first point's coordinate and its tangent
	//	* Note that we're using 0.0001f instead of 0.0f to avoid the tangent = 0, which results in tangent = Z-axis
	bezier3d(control, coordSet, pointA);
	bezierDerivative3d(control, coordSet, tangentA);

	for (int i = 1; i < lineSmoothness; i++) {
		t = (float)i / lineSmoothness;

		// Calculate second point's coordinate and its tangent
		bezier3d(t, coordSet, pointB);
		bezierDerivative3d(t, coordSet, tangentB);

		// Draw tube segment
		drawTube(pointA, tangentA, radius, pointB, tangentB, radius, faces, texture);

		// Copy second point to first point (to be used in the next loop & save resources)
		for (int i = 0; i < 3; i++) {
			pointA[i] = pointB[i];
			tangentA[i] = tangentB[i];
		}
	}

	// Calculate last point's coordinate and its tangent
	//	* Note that we're using 0.9999f instead of 1.0f to avoid the tangent = 0, which results in tangent = Z-axis
	bezier3d(1.0f - control, coordSet, pointB);
	bezierDerivative3d(1.0f - control, coordSet, tangentB);

	// Draw the last tube and voila!
	drawTube(pointA, tangentA, radius, pointB, tangentB, radius, faces, texture);
}

/*	Given the bezier line coords,
*	Draw a cylindrical tube that follows the bezier line
*		- The radii on the tube alternates between radius1 and radius2
*/
void Utility::drawAlternatingBezierTube(CoordinateSet coordSet, int lineSmoothness, int faces, float radius1, float radius2) {
	// Error handling
	if (lineSmoothness < 1) return;

	float pointA[3], pointB[3], tangentA[3], tangentB[3];
	float t;
	bool reverse = false;

	// Calculate first point's coordinate and its tangent
	//	* Note that we're using 0.0001f instead of 0.0f to avoid the tangent = 0, which results in tangent = Z-axis
	bezier3d(0.0001f, coordSet, pointA);
	bezierDerivative3d(0.0001f, coordSet, tangentA);

	for (int i = 1; i < lineSmoothness; i++) {
		t = (float)i / lineSmoothness;

		// Calculate second point's coordinate and its tangent
		bezier3d(t, coordSet, pointB);
		bezierDerivative3d(t, coordSet, tangentB);

		// Draw tube segment with alternating radii
		if (reverse)
			drawTube(pointA, tangentA, radius2, pointB, tangentB, radius1, faces);
		else
			drawTube(pointA, tangentA, radius1, pointB, tangentB, radius2, faces);

		reverse = !reverse;

		// Copy second point to first point (to be used in the next loop & save resources)
		for (int i = 0; i < 3; i++) {
			pointA[i] = pointB[i];
			tangentA[i] = tangentB[i];
		}
	}

	// Calculate last point's coordinate and its tangent
	//	* Note that we're using 0.9999f instead of 1.0f to avoid the tangent = 0, which results in tangent = Z-axis
	bezier3d(0.9999f, coordSet, pointB);
	bezierDerivative3d(0.9999f, coordSet, tangentB);

	// Draw the last tube and voila!
	if (reverse)
		drawTube(pointA, tangentA, radius2, pointB, tangentB, radius1, faces);
	else
		drawTube(pointA, tangentA, radius1, pointB, tangentB, radius2, faces);
}

/*	Draws multiple straight tubes, but the connections between them are made of auto generated bezier curves. 
*	Each control points given (Except the first and last point) will be automatically converted into a 3-point bezier curve
*	that will be drawn as a turning segment that connects the tube. 
*	The degree / curvature of turn segments depends on the "turnMultiplier" variable. 
* 
*	@param	turnMultipler: The larger the value, the more curvy each turn segment is, maximum 0.5
*/
void Utility::drawStraightTubes(CoordinateSet points, int smoothnessStraight, int smoothnessTurn, int faces, float radius, float turnMultiplier) {
	if (points.numberOfCoords <= 2) {
		drawBezierTube(points, smoothnessStraight, faces, radius);
		return;
	}
	
	float coord1[3], coord2[3], coord3[3];
	CoordinateSet tempSet(3);

	// Prepare first tube's coordinates
	points.getCoords(1, coord1);

	// For each control point that are not first or last point in the sequence:
	for (int i = 2; i < points.numberOfCoords; i++) {
		// Draw a straight tube from previous point to current point
		{
			// Start point of straight tube is obtain from previous calculations. (coord1)
			// Get end point of the straight tube and assign to coord3
			points.getCoords(i, coord2);
			for (int j = 0; j < 3; j++) {
				coord3[j] = coord2[j] - (coord2[j] - coord1[j]) * turnMultiplier;
			}

			// Draw straight tube (coord1 -> coord3)
			tempSet.clear(3);
			tempSet.addCoordinate(coord1);
			tempSet.addCoordinate(coord3);
			drawBezierTube(tempSet, smoothnessStraight, faces, radius);
		}

		// Then draw a turning segment at the current point
		{
			// Start point of turning tube is obtained from previous calculations, (coord3)
			// Use current point as pivot point, (coord2)
			// Get the end point of the turning segment and assign to coord1
			points.getCoords(i + 1, coord1);
			for (int j = 0; j < 3; j++) {
				coord1[j] = coord2[j] + (coord1[j] - coord2[j]) * turnMultiplier;
			}
			
			// Draw the turning tube (coord3 -> coord2 -> coord1)
			tempSet.clear(3);
			tempSet.addCoordinate(coord3);
			tempSet.addCoordinate(coord2);
			tempSet.addCoordinate(coord1);
			drawBezierTube(tempSet, smoothnessTurn, faces, radius);
		}
	}

	// Draw last straight tube
	// Start point of straight tube is obtain from previous calculations. (coord1)
	points.getCoords(points.numberOfCoords, coord3);
	tempSet.clear(3);
	tempSet.addCoordinate(coord1);
	tempSet.addCoordinate(coord3);
	drawBezierTube(tempSet, smoothnessStraight, faces, radius);

	tempSet.clear(3);

	tempSet.destroy();
}


/*	Draw a polygon using the given coordinate set
*	Draw polygon vertices in the order that the coordinates are given
*		*DOESN'T check if the polygon is flat, use with caution
*/
void Utility::drawPolygon(CoordinateSet coordSet, float center[3], float volumeCenter[3]) {
	glBegin(GL_TRIANGLE_FAN);
	drawVertex(center[0], center[1], center[2], volumeCenter);
	for (int i = 0; i < coordSet.numberOfCoords; i++) {
		drawVertex(coordSet.xCoords[i], coordSet.yCoords[i], coordSet.zCoords[i], volumeCenter);
	}
	drawVertex(coordSet.xCoords[0], coordSet.yCoords[0], coordSet.zCoords[0], volumeCenter);
	glEnd();
}

void Utility::drawPolygon(CoordinateSet coordSet, float center[3], float volumeCenter[3], GLuint texture) {
	glPushMatrix();
	{
		Texture::use(texture);
		Texture::on();
		glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(center[0], center[1]);
		drawVertex(center[0], center[1], center[2], volumeCenter);

		for (int i = 0; i < coordSet.numberOfCoords; i++) {
			glTexCoord2f(coordSet.xCoords[i], coordSet.yCoords[i]);
			drawVertex(coordSet.xCoords[i], coordSet.yCoords[i], coordSet.zCoords[i], volumeCenter);
		}
		glTexCoord2f(coordSet.xCoords[0], coordSet.yCoords[0]);
		drawVertex(coordSet.xCoords[0], coordSet.yCoords[0], coordSet.zCoords[0], volumeCenter);
		glEnd();
		Texture::off();
	}
	glPopMatrix();
}


/*	Extrudes a polygon towards a given direction
*	Will draw a cylindrical shape with the given poygon as the top and bottom
*	The side faces of the cylindrical shape will be quads
*		The "bool drawTop" variable determines whether to draw the given polygon / first polygon
*		The "bool drawBottom" variable determines whether to draw the extruded polygon / second polygon
*/
void Utility::extrudePolygon(CoordinateSet face, float faceCenter[3], float direction[3], float amount, bool drawTop, bool drawBottom, float volumeCenter[3]) {
	// Obtain CoordinateSet for second polygon
	CoordinateSet another = face.copy();
	float newTranslation[3] = { amount * direction[0], amount * direction[1], amount * direction[2] };
	another.translate(newTranslation);
	float anotherCenter[3];
	for (int i = 0; i < 3; i++) {
		anotherCenter[i] = faceCenter[i] + amount * direction[i];
	}

	// Calculate Center of the volume to calculate vertex normals for lighting
	bool volCenterGiven = true;
	if (volumeCenter == NULL) {
		volCenterGiven = false;
		volumeCenter = new float[3];
		for (int i = 0; i < 3; i++) {
			volumeCenter[i] = faceCenter[i] + direction[i] * amount / 2.0f;
		}
	}

	// Draw top face
	if (drawTop) {
		drawPolygon(face, faceCenter, volumeCenter);
	}

	// Draw bottom face (In reverse direction)
	if (drawBottom) {
		drawPolygon(another, anotherCenter, volumeCenter);
	}

	// Draw quad strips in between
	for (int i = 0; i < face.numberOfCoords - 1; i++) {
		glBegin(GL_QUADS);
		// Point #1 and #2 (lies on first polygon)
		drawVertex(face.xCoords[i], face.yCoords[i], face.zCoords[i], volumeCenter);
		drawVertex(face.xCoords[i + 1], face.yCoords[i + 1], face.zCoords[i + 1], volumeCenter);

		// Point #3 and #4 (lies on second polygon)
		drawVertex(another.xCoords[i + 1], another.yCoords[i + 1], another.zCoords[i + 1], volumeCenter);
		drawVertex(another.xCoords[i], another.yCoords[i], another.zCoords[i], volumeCenter);

		glEnd();
	}

	// Draw quad strip between start and end coord
	if (face.numberOfCoords > 1) {
		int n = face.numberOfCoords - 1;
		glBegin(GL_QUADS);
		// Point #1 and #2 (lies on first polygon)
		drawVertex(face.xCoords[n], face.yCoords[n], face.zCoords[n], volumeCenter);
		drawVertex(face.xCoords[0], face.yCoords[0], face.zCoords[0], volumeCenter);

		// Point #3 and #4 (lies on second polygon)
		glVertex3f(another.xCoords[0], another.yCoords[0], another.zCoords[0]);
		glVertex3f(another.xCoords[n], another.yCoords[n], another.zCoords[n]);
		glEnd();
	}

	// Dealloc unused memory
	another.destroy();

	delete[] volumeCenter;
}

void Utility::extrudePolygon(CoordinateSet face, float faceCenter[3], float direction[3], float amount, TextureMap tMap, bool drawTop, bool drawBottom, float volumeCenter[3]) {
	// Obtain CoordinateSet for second polygon
	CoordinateSet another = face.copy();
	float newTranslation[3] = { amount * direction[0], amount * direction[1], amount * direction[2] };
	another.translate(newTranslation);
	float anotherCenter[3];
	for (int i = 0; i < 3; i++) {
		anotherCenter[i] = faceCenter[i] + amount * direction[i];
	}

	// Calculate Center of the volume to calculate vertex normals for lighting
	bool volCenterGiven = true;
	if (volumeCenter == NULL) {
		volCenterGiven = false;
		volumeCenter = new float[3];
		for (int i = 0; i < 3; i++) {
			volumeCenter[i] = faceCenter[i] + direction[i] * amount / 2.0f;
		}
	}

	// Draw top face
	if (drawTop) {
		if (tMap.has(1))
			drawPolygon(face, faceCenter, volumeCenter, tMap.get(1));
		else
			drawPolygon(face, faceCenter, volumeCenter);
	}

	// Draw bottom face (In reverse direction)
	if (drawBottom) {
		if (tMap.has(2))
			drawPolygon(another, anotherCenter, volumeCenter, tMap.get(2));
		else
			drawPolygon(another, anotherCenter, volumeCenter);
	}

	// Draw quad strips in between
	boolean useTexture = tMap.has(3);
	if (useTexture) {
		Texture::use(tMap.get(3));
		Texture::on();
	}
	float xCum[2] = { 0, 0 };
	float yCum[2] = { 0, 0 };
	float xCumDiff, yCumDiff;
	for (int i = 0; i < face.numberOfCoords - 1; i++) {
		
		glBegin(GL_QUADS);
		// Point #1 and #2 (lies on first polygon)
		if (useTexture) {
			if (i == 0) {
				xCumDiff = -face.zCoords[i];
				yCumDiff = -face.xCoords[i];
			}
			else {
				xCumDiff = -(face.zCoords[i] - face.zCoords[i - 1]);
				yCumDiff = -(sqrtf(pow((face.xCoords[i] - face.xCoords[i - 1]), 2) + pow((face.yCoords[i] - face.yCoords[i - 1]), 2)));
			}
			xCum[0] += xCumDiff;
			yCum[0] += yCumDiff;
			glTexCoord2f(xCum[0], yCum[0]);
		}
		drawVertex(face.xCoords[i], face.yCoords[i], face.zCoords[i], volumeCenter);

		if (useTexture) {
			xCumDiff = -(face.zCoords[i + 1] - face.zCoords[i]);
			yCumDiff = -(sqrtf(pow((face.xCoords[i + 1] - face.xCoords[i]), 2) + pow((face.yCoords[i + 1] - face.yCoords[i]), 2)));
			xCum[0] += xCumDiff;
			yCum[0] += yCumDiff;
			glTexCoord2f(xCum[0], yCum[0]);
		}
		drawVertex(face.xCoords[i + 1], face.yCoords[i + 1], face.zCoords[i + 1], volumeCenter);
		if (useTexture) {
			xCum[0] -= xCumDiff;
			yCum[0] -= yCumDiff;
		}

		// Point #3 and #4 (lies on second polygon)
		if (useTexture) {
			if (i == 0) {
				xCumDiff = -another.zCoords[i];
				yCumDiff = -another.xCoords[i];
			}
			else {
				xCumDiff = -(another.zCoords[i] - another.zCoords[i - 1]);
				yCumDiff = -(sqrtf(pow((another.xCoords[i] - another.xCoords[i - 1]), 2) + pow((another.yCoords[i] - another.yCoords[i - 1]), 2)));
			}
			xCum[1] += xCumDiff;
			yCum[1] += yCumDiff;
			xCumDiff = -(another.zCoords[i + 1] - another.zCoords[i]);
			yCumDiff = -(sqrtf(pow((another.xCoords[i + 1] - another.xCoords[i]), 2) + pow((another.yCoords[i + 1] - another.yCoords[i]), 2)));
			xCum[1] += xCumDiff;
			yCum[1] += yCumDiff;
			glTexCoord2f(xCum[1], yCum[1]);
		}
		drawVertex(another.xCoords[i + 1], another.yCoords[i + 1], another.zCoords[i + 1], volumeCenter);
		if (useTexture) {
			xCum[1] -= xCumDiff;
			yCum[1] -= yCumDiff;
			glTexCoord2f(xCum[1], yCum[1]);
		}
		drawVertex(another.xCoords[i], another.yCoords[i], another.zCoords[i], volumeCenter);

		glEnd();
	}

	// Draw quad strip between start and end coord
	if (face.numberOfCoords > 1) {
		int n = face.numberOfCoords - 1;
		glBegin(GL_QUADS);
		// Point #1 and #2 (lies on first polygon)
		if (useTexture) {
			xCumDiff = -(face.zCoords[n] - face.zCoords[n - 1]);
			yCumDiff = -(sqrtf(pow((face.xCoords[n] - face.xCoords[n - 1]), 2) + pow((face.yCoords[n] - face.yCoords[n - 1]), 2)));
			xCum[0] += xCumDiff;
			yCum[0] += yCumDiff;
			glTexCoord2f(xCum[0], yCum[0]);
		}
		drawVertex(face.xCoords[n], face.yCoords[n], face.zCoords[n], volumeCenter);
		if (useTexture) {
			xCumDiff = -(face.zCoords[0] - face.zCoords[n]);
			yCumDiff = -(sqrtf(pow((face.xCoords[0] - face.xCoords[n]), 2) + pow((face.yCoords[0] - face.yCoords[n]), 2)));
			xCum[0] += xCumDiff;
			yCum[0] += yCumDiff;
			glTexCoord2f(-face.zCoords[0], -face.xCoords[0]);
		}
		drawVertex(face.xCoords[0], face.yCoords[0], face.zCoords[0], volumeCenter);

		// Point #3 and #4 (lies on second polygon)
		if (useTexture) {
			xCumDiff = -(another.zCoords[n] - another.zCoords[n - 1]);
			yCumDiff = -(sqrtf(pow((another.xCoords[n] - another.xCoords[n - 1]), 2) + pow((another.yCoords[n] - another.yCoords[n - 1]), 2)));
			xCum[1] += xCumDiff;
			yCum[1] += yCumDiff;
			xCumDiff = -(another.zCoords[0] - another.zCoords[n]);
			yCumDiff = -(sqrtf(pow((another.xCoords[0] - another.xCoords[n]), 2) + pow((another.yCoords[0] - another.yCoords[n]), 2)));
			xCum[1] += xCumDiff;
			yCum[1] += yCumDiff;
			glTexCoord2f(xCum[1], yCum[1]);
		}
		drawVertex(another.xCoords[0], another.yCoords[0], another.zCoords[0], volumeCenter);
		if (useTexture) {
			xCum[1] -= xCumDiff;
			yCum[1] -= yCumDiff;
			glTexCoord2f(xCum[1], yCum[1]);
		}
		drawVertex(another.xCoords[n], another.yCoords[n], another.zCoords[n], volumeCenter);
		glEnd();
	}
	if (useTexture)
		Texture::off();

	// Dealloc unused memory
	another.destroy();
	delete[] volumeCenter;
}


/* Draw hemisphere at location 0, 0, 0
*/
void Utility::drawHemisphere(float radius, int slices, int stacks) {
	if (hemiSphereObj == NULL) {
		hemiSphereObj = gluNewQuadric();
	}

	// Draw cylinders stack by stack
	glPushMatrix();
	{
		float currentStack = 0;
		float stackIncrement = 90.0f / stacks;
		glRotatef(-90, 1, 0, 0);
		for (int i = 0; i < stacks; i++) {
			float currentHeight = radius * sin(degToRad(currentStack + stackIncrement));
			float previousHeight = radius * sin(degToRad(currentStack));
			float thisHeight = currentHeight - previousHeight;
			gluCylinder(hemiSphereObj, radius * cos(degToRad(currentStack)), radius * cos(degToRad(currentStack + stackIncrement)), thisHeight, slices, 1);
			glTranslatef(0, 0, thisHeight);
			currentStack += stackIncrement;
		}
	}
	glPopMatrix();
}

void Utility::drawHemisphere(float radius, int slices, int stacks, GLuint texture) {
	if (hemiSphereObj == NULL) {
		hemiSphereObj = gluNewQuadric();
	}

	// Texture
	Texture::on();
	Texture::use(texture);
	gluQuadricTexture(hemiSphereObj, GL_TRUE);

	// Draw cylinders stack by stack
	glPushMatrix();
	{
		float currentStack = 0;
		float stackIncrement = 90.0f / stacks;
		glRotatef(-90, 1, 0, 0);
		for (int i = 0; i < stacks; i++) {
			float currentHeight = radius * sin(degToRad(currentStack + stackIncrement));
			float previousHeight = radius * sin(degToRad(currentStack));
			float thisHeight = currentHeight - previousHeight;
			gluCylinder(hemiSphereObj, radius * cos(degToRad(currentStack)), radius * cos(degToRad(currentStack + stackIncrement)), thisHeight, slices, 1);
			glTranslatef(0, 0, thisHeight);
			currentStack += stackIncrement;
		}
	}
	glPopMatrix();
	Texture::off();
}


/*	Draw the connections between two faces
*	Doesn't draw the given faces itself
*	Draw the connections using GL_QUADS
*	The amount of vertices in both faces must be the same
*	f1 must be in same rotational direction with f2
*/
void Utility::connectTwoFaces(CoordinateSet f1, CoordinateSet f2, float volumeCenter[3]) {
	if (f1.numberOfCoords != f2.numberOfCoords) {
		throw exception("Vertices mismatch: Utility::connectTwoFaces()");
		return;
	}

	int n = f1.numberOfCoords;

	// Draw connection faces
	for (int i = 0; i < n - 1; i++) {
		glBegin(GL_QUADS);
		drawVertex(f1.xCoords[i], f1.yCoords[i], f1.zCoords[i], volumeCenter);
		drawVertex(f2.xCoords[i], f2.yCoords[i], f2.zCoords[i], volumeCenter);
		drawVertex(f2.xCoords[i + 1], f2.yCoords[i + 1], f2.zCoords[i + 1], volumeCenter);
		drawVertex(f1.xCoords[i + 1], f1.yCoords[i + 1], f1.zCoords[i + 1], volumeCenter);
		glEnd();
	}

	// Draw connection faces between end and start vertices
	glBegin(GL_QUADS);
	drawVertex(f1.xCoords[n - 1], f1.yCoords[n - 1], f1.zCoords[n - 1], volumeCenter);
	drawVertex(f2.xCoords[n - 1], f2.yCoords[n - 1], f2.zCoords[n - 1], volumeCenter);
	drawVertex(f2.xCoords[0], f2.yCoords[0], f2.zCoords[0], volumeCenter);
	drawVertex(f1.xCoords[0], f1.yCoords[0], f1.zCoords[0], volumeCenter);
	glEnd();
}

void Utility::connectTwoFaces(CoordinateSet f1, CoordinateSet f2, GLuint texture, float volumeCenter[3]) {
	if (f1.numberOfCoords != f2.numberOfCoords) {
		throw exception("Vertices mismatch: Utility::connectTwoFaces()");
		return;
	}

	int n = f1.numberOfCoords;

	Texture::use(texture);
	Texture::on();
	// Draw connection faces
	for (int i = 0; i < n - 1; i++) {
		glBegin(GL_QUADS);
		glTexCoord2f(-f1.zCoords[i], -f1.xCoords[i]);
		drawVertex(f1.xCoords[i], f1.yCoords[i], f1.zCoords[i], volumeCenter);
		glTexCoord2f(-f2.zCoords[i], -f2.xCoords[i]);
		drawVertex(f2.xCoords[i], f2.yCoords[i], f2.zCoords[i], volumeCenter);
		glTexCoord2f(-f2.zCoords[i + 1], -f2.xCoords[i + 1]);
		drawVertex(f2.xCoords[i + 1], f2.yCoords[i + 1], f2.zCoords[i + 1], volumeCenter);
		glTexCoord2f(-f1.zCoords[i + 1], -f1.xCoords[i + 1]);
		drawVertex(f1.xCoords[i + 1], f1.yCoords[i + 1], f1.zCoords[i + 1], volumeCenter);
		glEnd();
	}

	// Draw connection faces between end and start vertices
	glBegin(GL_QUADS);
	glTexCoord2f(-f1.zCoords[n - 1], -f1.xCoords[n - 1]);
	drawVertex(f1.xCoords[n - 1], f1.yCoords[n - 1], f1.zCoords[n - 1], volumeCenter);
	glTexCoord2f(-f2.zCoords[n - 1], -f2.xCoords[n - 1]);
	drawVertex(f2.xCoords[n - 1], f2.yCoords[n - 1], f2.zCoords[n - 1], volumeCenter);
	glTexCoord2f(-f2.zCoords[0], -f2.xCoords[0]);
	drawVertex(f2.xCoords[0], f2.yCoords[0], f2.zCoords[0], volumeCenter);
	glTexCoord2f(-f1.zCoords[0], -f1.xCoords[0]);
	drawVertex(f1.xCoords[0], f1.yCoords[0], f1.zCoords[0], volumeCenter);
	glEnd();

	Texture::off();
}

/*	[ Rotation in 3D space ]
	-Rotation around X-axis
		|1     0        0  | |x|   |		 x		  |   |x'|
		|0   cos t   -sin t| |y| = |y cos t - z sin t | = |y'|
		|0   sin t    cos t| |z|   |y sin t + z cos t |   |z'|

	- Rotation around Y-axis
		| cos t   0   sin t| |x|   | x cos t + z sin t|   |x'|
		|   0     1     0  | |y| = |		 y		  | = |y'|
		|-sin t   0   cos t| |z|   |-x sin t + z cos t|   |z'|

	- Rotation around Z-axis
		|cos t   -sin t   0| |x|   |x cos t - y sin t |   |x'|
		|sin t    cos t   0| |y| = |x sin t + y cos t | = |y'|
		|  0        0     1| |z|   |		z		  |   |z'|

	*Note that rotation around an axis will not change its position on that axis,
		ie: Rotation around Y-axis doesnt change the Y-coordinate,
		because the rotation only happens on the XZ plane
	*Reference: https://stackoverflow.com/questions/14607640/rotating-a-vector-in-3d-space
*/
void Utility::rotateAroundXaxis(float vector[3], float angle, float output[3]) {
	float sinT = sin(degToRad(angle));
	float cosT = cos(degToRad(angle));
	output[0] = vector[0];
	output[1] = vector[1] * cosT - vector[2] * sinT;
	output[2] = vector[1] * sinT + vector[2] * cosT;
}

void Utility::rotateAroundYaxis(float vector[3], float angle, float output[3]) {
	float sinT = sin(degToRad(angle));
	float cosT = cos(degToRad(angle));
	output[0] = vector[0] * cosT + vector[2] * sinT;
	output[1] = vector[1];
	output[2] = -vector[0] * sinT + vector[2] * cosT;
}

void Utility::rotateAroundZaxis(float vector[3], float angle, float output[3]) {
	float sinT = sin(degToRad(angle));
	float cosT = cos(degToRad(angle));
	output[0] = vector[0] * cosT - vector[1] * sinT;
	output[1] = vector[0] * sinT + vector[1] * cosT;
	output[2] = vector[2];
}


/*	Returns a CoordinateSet that contains the coordinates that lies on a circle's parameter
*	Used in drawing flat circles
*/
CoordinateSet Utility::circleCoords(float center[3], float radius, int edges, float startDegree, float endDegree) {
	CoordinateSet result(edges + 1);
	float rot = startDegree;
	float increment = (endDegree - startDegree) / edges;
	for (int i = 0; i <= edges; i++) {
		float x = center[0] + radius * cos(degToRad(rot));
		float y = center[1] + radius * sin(degToRad(rot));
		float z = center[2];
		result.addCoordinate(x, y, z);
		rot += increment;
	}
	return result;
}

/*	Returns a CoordinateSet that contains the coordinates that lies on the given bezier curve
*	The variable "int divisions" determines how many segments the curve should have / how smooth the curve should be
*/
CoordinateSet Utility::bezierCurveCoords(CoordinateSet points, int divisions) {
	CoordinateSet output(divisions + 1);
	for (int i = 0; i <= divisions; i++) {
		float t = (float)i / divisions;
		float thisPoint[3];
		bezier3d(t, points, thisPoint);
		output.addCoordinate(thisPoint);
	}
	return output;
}

void Utility::drawVertex(float vertex[3], float origin[3]) {
	float normal[3];
	if (origin == NULL) {
		for (int i = 0; i < 3; i++)
			origin[i] = worldOrigin[i];
	}
	vectorFromTwoPoints(origin, vertex, normal);
	glNormal3f(normal[0], normal[1], normal[2]);
	glVertex3f(vertex[0], vertex[1], vertex[2]);
}

void Utility::drawVertex(float vector[3], float q1, float q2, float q3) {
	float temp[3] = {q1, q2, q3};
	drawVertex(vector, temp);
}

void Utility::drawVertex(float p1, float p2, float p3, float origin[3]) {
	float temp[3] = {p1, p2, p3};
	drawVertex(temp, origin);
}

void Utility::drawVertex(float p1, float p2, float p3, float q1, float q2, float q3) {
	float temp1[3] = { p1, p2, p3 };
	float temp2[3] = { q1, q2, q3 };
	drawVertex(temp1, temp2);
}

/*	Bezier Curve formula:
*		2 Points curve: P = (1-t)a + (t)b
*		3 Points curve: P = [(1-t)^2 * a] + [2(1-t)t*b] + [t^2 * c]
*		4 Points curve: P = [(1-t)^3 * a] + [3(1-t)^2 * t * b] + [3(1-t) * t^2 * c] + [t^3 * d]
*		5 Points and above: Not recommended as it is computatively expensive and rarely used
*		(Where a, b, c, d, ... are Control Point #1, #2, #3, #4... on a bezier curve)
*/
float Utility::bezier(float t, float coords[], int points) {
	double result = 0;
	for (int i = 0; i < points; i++) {
		int coefficient = points - 1;
		if (i == 0 || i == coefficient)
			coefficient = 1;
		result += pow(1.0f - (double)t, points - i - 1) * pow((double)t, i) * coords[i] * coefficient;
	}
	return (float)result;
}

/*	Derivative of the Bezier Curve Formula
*		To obtain dP/dt of the bezier curve formula
*		2 Points curve derivative: dP/dt = 0
*		3 Points curve derivative: dP/dt = (2t - 2)a + (-4t - 2)b + (2t)c
*		4 Points curve derivative: dP/dt = (-3t^2 + 6t - 3)a + (9t^2 - 12t + 3)b + (-9t^2 + 6t)c + (3t^2)d
*		5 Points and above: Not recommended as it is computatively expensive and rarely used
*		(Where a, b, c, d, ... are Control Point #1, #2, #3, #4... on a bezier curve)
*/
float Utility::bezierDerivative(float t, float coords[], int points) {
	float result = 0;
	for (int i = 0; i < points; i++) {
		int coefficient = points - 1;
		if (i == 0 || i == coefficient)
			coefficient = 1;

		// d(uv)/dx = u(dv/dx) + v(du/dx
		// u = (1-t)^(points - i - 1)
		// v = t^(i)			(			u					)(		dv/dx		) + (	v	)(					du/dx								)
		result += coefficient * coords[i] * ((pow(1.0f - t, points - i - 1) * i * pow(t, i - 1)) + (pow(t, i) * (points - i - 1) * (-1) * pow(1.0f - t, points - i - 2)));
	}
	return result;
}

//	Bezier curve formula with 3 axes: x, y and z
void Utility::bezier3d(float t, CoordinateSet coordSet, float output[3]) {
	output[0] = bezier(t, coordSet.xCoords, coordSet.numberOfCoords);
	output[1] = bezier(t, coordSet.yCoords, coordSet.numberOfCoords);
	output[2] = bezier(t, coordSet.zCoords, coordSet.numberOfCoords);
}

//	The derivative of bezier curve formula with 3 axes: x, y and z
void Utility::bezierDerivative3d(float t, CoordinateSet coordSet, float output[3]) {
	output[0] = bezierDerivative(t, coordSet.xCoords, coordSet.numberOfCoords);
	output[1] = bezierDerivative(t, coordSet.yCoords, coordSet.numberOfCoords);
	output[2] = bezierDerivative(t, coordSet.zCoords, coordSet.numberOfCoords);
}

/*	Get Cross Product between two given vectors A and B
*		A x B is a vector that is perpendicular to both A and B
*/
void Utility::crossProduct(float v1[3], float v2[3], float output[3]) {
	output[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
	output[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
	output[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
}

/*	Get Dot Product between two given vectors, A and B
*		A . B = (x1 * x2) + (y1 * y2) + (z1 * z2)
*/
float Utility::dotProduct(float v1[3], float v2[3]) {
	float result = 0;
	for (int i = 0; i < 3; i++) {
		result += v1[i] * v2[i];
	}
	return result;
}

/*	Get the magnitude of a given vector
*		Magnitude = sqrt( x^2 + y^2 + z^2 )
*/
float Utility::vectorMagnitude(float v[3]) {
	float squareSum = 0;
	for (int i = 0; i < 3; i++) {
		squareSum += v[i] * v[i];
	}
	return sqrtf(squareSum);
}

/*	Get the angle between two given vectors, A and B
*/
float Utility::angleBetweenTwoVectors(float v1[3], float v2[3]) {
	return radToDeg(acos(dotProduct(v1, v2) / (vectorMagnitude(v1) * vectorMagnitude(v2))));
}

/*	Normalize a given vector
*	A normalized vector has magnitude = 1
*		Normalized vector = (Original vector) / Magnitude
*/
void Utility::normalizeVector(float vector[3]) {
	float mag = vectorMagnitude(vector);
	if (mag == 0) return;
	for (int i = 0; i < 3; i++)
		vector[i] /= mag;
}

/*	Given a vector V,
*	Calculate the two other orthogonal basis vectors
*	that are both perpendicular to V
*		- Assume V is also one of the orthogonal basis vectors
*		- Used in drawTube() to obtain individual point coordinates on each tube segment without using openGL transformations
*/
void Utility::orthogonalBasis(float vector[3], float basisA[3], float basisB[3]) {
	// Make a copy to avoid modifying the original vector
	float normNormal[3];
	for (int i = 0; i < 3; i++)
		normNormal[i] = vector[i];

	float zAxis[3] = { 0, 0, 1 };
	float yAxis[3] = { 0, 1, 0 };
	float xAxis[3] = { 1, 0, 0 };
	float tempAxis[3] = {-1, -1, -1};
	float temp[3];

	// Normalize the vector V
	normalizeVector(normNormal);

	// Obtain the first orthogonal basis
	// For some reason when normNormal is parallel to zAxis, the entire tube gets shrinked to infinity
	// Tried to use crossproduct with other axes but still doesn't work. 
	// This problem has a workaround at drawTube();
	crossProduct(normNormal, zAxis, basisA);

	// Get cross product between V and vectorA
	// to obtain the second basis vector, vectorB
	crossProduct(normNormal, basisA, basisB);

	// Normalize them to prevent weird stuff happening when the vector approaches Z-axis
	normalizeVector(basisA);
	normalizeVector(basisB);
}

bool Utility::isParallelVector(float v1[3], float v2[3]) {
	float mag1 = vectorMagnitude(v1);
	float mag2 = vectorMagnitude(v2);
	// First check to reduce computation
	if (mag1 == 0 || mag2 == 0) return true;

	float sinT = sin(degToRad(angleBetweenTwoVectors(v1, v2)));
	return mag1 * mag2 * sinT == 0.0f;
}

float Utility::distanceBetweenTwoPoints(float p1[3], float p2[3]) {
	float result = 0;
	for (int i = 0; i < 3; i++) {
		result += pow(p1[i] - p2[i], 2);
	}
	return sqrtf(result);
}

void Utility::vectorFromTwoPoints(float p1[3], float p2[3], float output[3]) {
	for (int i = 0; i < 3; i++) {
		output[i] = p2[i] - p1[i];
	}
}

void Utility::midpointBetweenTwoPoints(float p1[3], float p2[3], float output[3]) {
	for (int i = 0; i < 3; i++) {
		output[i] = (p1[i] + p2[i]) / 2.0f;
	}
}

/* Developer Note
*/
// Rotation applied to first vector need to be applied to second vector as well, the rotation applied is same
// So rotation on v2 = rotation on v1
// To find rotation on v1: 
// Find rotation from original v1 to z-axis
