
#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <list>

#include "Utility.h"
#include "CoordinateSet.h"

using namespace std;

const float PI = 3.1415926f;

// Convert degrees to radians
float Utility::degToRad(float deg) {
	return deg * PI / 180.0f;
}

// Convert radians to degrees
float Utility::radToDeg(float rad) {
	return rad * 180.0f / PI;
}

void Utility::drawGrids(float interval) {
	float limit = 2.0f;
	for (float i = 0.0f; i < limit; i += interval) {
		// Draw Column lines
		if (i != 0) {
			glLineWidth(1);
			glBegin(GL_LINES);
			glVertex3f(i, -limit, 0);
			glVertex3f(i, limit, 0);
			glVertex3f(-i, -limit, 0);
			glVertex3f(-i, limit, 0);

			glVertex3f(-limit, i, 0);
			glVertex3f(limit, i, 0);
			glVertex3f(-limit, -i, 0);
			glVertex3f(limit, -i, 0);

			glVertex3f(0, -limit, i);
			glVertex3f(0, limit, i);
			glVertex3f(0, -limit, -i);
			glVertex3f(0, limit, -i);

			glVertex3f(0, i, -limit);
			glVertex3f(0, i, limit);
			glVertex3f(0, -i, -limit);
			glVertex3f(0, -i, limit);

			glVertex3f(i, 0, -limit);
			glVertex3f(i, 0, limit);
			glVertex3f(-i, 0, -limit);
			glVertex3f(-i, 0, limit);

			glVertex3f(-limit, 0, i);
			glVertex3f(limit, 0, i);
			glVertex3f(-limit, 0, -i);
			glVertex3f(limit, 0, -i);
			glEnd();
		}
		else {
			glLineWidth(3);
			glBegin(GL_LINES);
			glVertex3f(i, -limit, 0);
			glVertex3f(i, limit, 0);
			glVertex3f(-limit, i, 0);
			glVertex3f(limit, i, 0);

			glVertex3f(0, -limit, i);
			glVertex3f(0, limit, i);
			glVertex3f(0, i, -limit);
			glVertex3f(0, i, limit);

			glVertex3f(i, 0, -limit);
			glVertex3f(i, 0, limit);
			glVertex3f(-limit, 0, i);
			glVertex3f(limit, 0, i);
			glEnd();
		}
	}
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
void Utility::drawCircle(float center[3], float normal[3], float radius) {
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
	for (int i = 0; i < 360; i++) {
		glVertex3f(cos(degToRad(i)) * radius, sin(degToRad(i)) * radius, 0);
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
		drawCircle(thisPoint, tangent, radius);
	}
}

/*	Given the center, normal and radius of two independent circles in 3d space,
*	draw a tube that uses quads as its faces
*		- This function is different from gluCylinder as in this function
*			allows the top and bottom of the cylinder to be non-parallel to each other
*		- Used in drawBezierTube()
*/
/*	Developer's note
[ Rotation in 3D space ]
	- Rotation around Z-axis
		|cos t   -sin t   0| |x|   |x cos t - y sin t |   |x'|
		|sin t    cos t   0| |y| = |x sin t + y cos t | = |y'|
		|  0        0     1| |z|   |		z		  |   |z'|

	- Rotation around Y-axis
		| cos t   0   sin t| |x|   | x cos t + z sin t|   |x'|
		|   0     1     0  | |y| = |		 y		  | = |y'|
		|-sin t   0   cos t| |z|   |-x sin t + z cos t|   |z'|

	-Rotation around X-axis
		|1     0        0  | |x|   |		 x		  |   |x'|
		|0   cos t   -sin t| |y| = |y cos t - z sin t | = |y'|
		|0   sin t    cos t| |z|   |y sin t + z cos t |   |z'|

	*Note that rotation around an axis will not change its position on that axis,
		ie: Rotation around Y-axis doesnt change the Y-coordinate,
		because the rotation only happens on the XZ plane
	*Reference: https://stackoverflow.com/questions/14607640/rotating-a-vector-in-3d-space
*/
void Utility::drawTube(float centerA[3], float normalA[3], float radiusA, float centerB[3], float normalB[3], float radiusB, int faces) {
	// Error handling
	if (faces >= -2 && faces <= 2) return;

	// Calculate the orthogonal basis vectors of normalA and normalB
	float basisA1[3], basisA2[3], basisB1[3], basisB2[3];
	orthogonalBasis(normalA, basisA1, basisA2);
	orthogonalBasis(normalB, basisB1, basisB2);

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
			temp[i] = centerA[i] + radiusA * (basisA1[i] * cos(degToRad(rot)) + basisA2[i] * sin(degToRad(rot)));
		glVertex3f(temp[0], temp[1], temp[2]);

		// Draw #2 that lines on first circle, with angle = rot + increment
		for (int i = 0; i < 3; i++)
			temp[i] = centerA[i] + radiusA * (basisA1[i] * cos(degToRad(rot + increment)) + basisA2[i] * sin(degToRad(rot + increment)));
		glVertex3f(temp[0], temp[1], temp[2]);

		// Draw #3 that lines on second circle, with angle = rot + increment
		for (int i = 0; i < 3; i++)
			temp[i] = centerB[i] + radiusB * (basisB1[i] * cos(degToRad(rot + increment)) + basisB2[i] * sin(degToRad(rot + increment)));
		glVertex3f(temp[0], temp[1], temp[2]);

		// Draw #4 that lines on second circle, with angle = rot
		for (int i = 0; i < 3; i++)
			temp[i] = centerB[i] + radiusB * (basisB1[i] * cos(degToRad(rot)) + basisB2[i] * sin(degToRad(rot)));
		glVertex3f(temp[0], temp[1], temp[2]);

		glEnd();

		// Increment rotation / theta
		rot += increment;
		
	}
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

/*	Draw a polygon using the given coordinate set
*	Draw polygon vertices in the order that the coordinates are given
*		*DOESN'T check if the polygon is flat, use with caution
*/
void Utility::drawPolygon(CoordinateSet coordSet, float center[3]) {
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(center[0], center[1], center[2]);
	for (int i = 0; i < coordSet.numberOfCoords; i++) {
		glVertex3f(coordSet.xCoords[i], coordSet.yCoords[i], coordSet.zCoords[i]);
	}
	glVertex3f(coordSet.xCoords[0], coordSet.yCoords[0], coordSet.zCoords[0]);
	glEnd();
}


void Utility::drawConcavePolygon(CoordinateSet polygonLeft, CoordinateSet polygonRight, float middleLine) {
	// Quad strips for polygonLeft
	for (int i = 0; i < polygonLeft.numberOfCoords - 1; i++) {
		glBegin(GL_QUADS);
		glVertex3f(polygonLeft.xCoords[i], polygonLeft.yCoords[i], polygonLeft.zCoords[i]);
		glVertex3f(polygonLeft.xCoords[i + 1], polygonLeft.yCoords[i + 1], polygonLeft.zCoords[i + 1]);
		glVertex3f(middleLine, polygonLeft.yCoords[i + 1], polygonLeft.zCoords[i + 1]);
		glVertex3f(middleLine, polygonLeft.yCoords[i], polygonLeft.zCoords[i]);
		glEnd();
	}

	glColor3f(1, 0, 0);
	// Quad strips for polygonRight
	for (int i = 0; i < polygonRight.numberOfCoords - 1; i++) {
		glBegin(GL_QUADS);
		glVertex3f(polygonRight.xCoords[i], polygonRight.yCoords[i], polygonRight.zCoords[i]);
		glVertex3f(middleLine, polygonRight.yCoords[i], polygonRight.zCoords[i]);
		glVertex3f(middleLine, polygonRight.yCoords[i + 1], polygonRight.zCoords[i + 1]);
		glVertex3f(polygonRight.xCoords[i + 1], polygonRight.yCoords[i + 1], polygonRight.zCoords[i + 1]);
		glEnd();
	}

}

/*	Extrudes a polygon towards a given direction
*	Will draw a cylindrical shape with the given poygon as the top and bottom
*	The side faces of the cylindrical shape will be quads
*		The "bool drawTop" variable determines whether to draw the given polygon / first polygon
*		The "bool drawBottom" variable determines whether to draw the extruded polygon / second polygon
*/
void Utility::extrudePolygon(CoordinateSet face, float faceCenter[3], float direction[3], float amount, bool drawTop, bool drawBottom) {
	// Obtain CoordinateSet for second polygon
	CoordinateSet another = face.copy();
	another.translate(new float[3]{ amount * direction[0], amount * direction[1], amount * direction[2] });
	float anotherCenter[3];
	for (int i = 0; i < 3; i++) {
		anotherCenter[i] = faceCenter[i] + amount * direction[i];
	}

	// Draw top face
	if (drawTop) {
		drawPolygon(face, faceCenter);
	}

	// Draw bottom face (In reverse direction)
	if (drawBottom) {
		glColor3f(1, 0, 1);
		drawPolygon(another, anotherCenter);
	}

	// Draw quad strips in between
	for (int i = 0; i < face.numberOfCoords - 1; i++) {
		glBegin(GL_QUADS);
		glColor3f(0, 1, 0);
		// Point #1 and #2 (lies on first polygon)
		glVertex3f(face.xCoords[i], face.yCoords[i], face.zCoords[i]);
		glVertex3f(face.xCoords[i + 1], face.yCoords[i + 1], face.zCoords[i + 1]);

		// Point #3 and #4 (lies on second polygon)
		glVertex3f(another.xCoords[i + 1], another.yCoords[i + 1], another.zCoords[i + 1]);
		glVertex3f(another.xCoords[i], another.yCoords[i], another.zCoords[i]);

		glEnd();
	}

	// Draw quad strip between start and end coord
	if (face.numberOfCoords > 1) {
		int n = face.numberOfCoords - 1;
		glBegin(GL_QUADS);
		// Point #1 and #2 (lies on first polygon)
		glVertex3f(face.xCoords[n], face.yCoords[n], face.zCoords[n]);
		glVertex3f(face.xCoords[0], face.yCoords[0], face.zCoords[0]);

		// Point #3 and #4 (lies on second polygon)
		glVertex3f(another.xCoords[0], another.yCoords[0], another.zCoords[0]);
		glVertex3f(another.xCoords[n], another.yCoords[n], another.zCoords[n]);
		glEnd();
	}

}

void Utility::extrudeConcavePolygon(CoordinateSet faceLeft, CoordinateSet faceRight, float middleLine, float direction[3], float amount, bool drawTop, bool drawBottom) {
	// Obtain CoordinateSet for second polygon
	CoordinateSet anotherLeft = faceLeft.copy();
	CoordinateSet anotherRight = faceRight.copy();
	anotherLeft.translate(new float[3]{ amount * direction[0], amount * direction[1], amount * direction[2] });
	anotherRight.translate(new float[3]{ amount * direction[0], amount * direction[1], amount * direction[2] });

	// Draw top face
	if (drawTop) {
		drawConcavePolygon(faceLeft, faceRight, middleLine);
	}

	// Draw bottom face (In reverse direction)
	if (drawBottom) {
		drawConcavePolygon(anotherLeft, anotherRight, middleLine);
	}

	// Draw quad strips in between
	CoordinateSet face(10);
	face.combineCoords(faceLeft);
	face.combineCoords(faceRight.reverse());

	CoordinateSet another = face.copy();
	another.translate(new float[3]{ amount * direction[0], amount * direction[1], amount * direction[2] });
	for (int i = 0; i < face.numberOfCoords - 1; i++) {
		glBegin(GL_QUADS);
		glColor3f(0, 1, 0);
		// Point #1 and #2 (lies on first polygon)
		glVertex3f(face.xCoords[i], face.yCoords[i], face.zCoords[i]);
		glVertex3f(face.xCoords[i + 1], face.yCoords[i + 1], face.zCoords[i + 1]);

		// Point #3 and #4 (lies on second polygon)
		glVertex3f(another.xCoords[i + 1], another.yCoords[i + 1], another.zCoords[i + 1]);
		glVertex3f(another.xCoords[i], another.yCoords[i], another.zCoords[i]);

		glEnd();
	}

	// Draw quad strip between start and end coord
	if (face.numberOfCoords > 1) {
		int n = face.numberOfCoords - 1;
		glBegin(GL_QUADS);
		// Point #1 and #2 (lies on first polygon)
		glVertex3f(face.xCoords[n], face.yCoords[n], face.zCoords[n]);
		glVertex3f(face.xCoords[0], face.yCoords[0], face.zCoords[0]);

		// Point #3 and #4 (lies on second polygon)
		glVertex3f(another.xCoords[0], another.yCoords[0], another.zCoords[0]);
		glVertex3f(another.xCoords[n], another.yCoords[n], another.zCoords[n]);
		glEnd();
	}

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
	float temp[3];

	// Normalize the vector V
	normalizeVector(normNormal);

	// Get dot product between V and X-axis


	// If dot product is too small, then the cross product between them must be parallel to the X-axis
	// If this happens, use another axis (Y-axis) to obtain vectorA
	if (!isParallelVector(normNormal, xAxis)) {
		crossProduct(normNormal, xAxis, basisA);
	}
	else if (!isParallelVector(normNormal, yAxis)) {
		crossProduct(normNormal, yAxis, basisA);
	}
	else {
		crossProduct(normNormal, zAxis, basisA);
	}

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


/* Developer Note
*/
// Rotation applied to first vector need to be applied to second vector as well, the rotation applied is same
// So rotation on v2 = rotation on v1
// To find rotation on v1: 
// Find rotation from original v1 to z-axis
