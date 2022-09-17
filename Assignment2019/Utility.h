#pragma once

class CoordinateSet;
class Utility {

public:
	// Unit conversion
	static float degToRad(float deg);
	static float radToDeg(float rad);

	// Drawing
	static void drawGrids(float interval = 0.1f);
	static void drawLine(CoordinateSet coords);
	static void drawCircle(float center[3], float normal[3], float radius, int edges);
	static void drawBezierLine(CoordinateSet coordSet, int divisions);
	static void drawCirclesAroundBezier(CoordinateSet coordSet, int divisions, float radius);
	static void drawTube(float centerA[3], float normalA[3], float radiusA, float centerB[3], float normalB[3], float radiusB, int faces);
	static void drawBezierTube(CoordinateSet coordSet, int lineSmoothness, int faces, float radius);
	static void drawAlternatingBezierTube(CoordinateSet coordSet, int lineSmoothness, int faces, float radius1, float radius2);
	static void drawStraightTubes(CoordinateSet points, int smoothnessStraight, int smoothnessTurn, int faces, float radius, float turnMultiplier);
	static void drawPolygon(CoordinateSet coordSet, float center[3]);
	static void drawConcavePolygon(CoordinateSet polygonLeft, CoordinateSet polygonRight, float middleLine);
	static void extrudePolygon(CoordinateSet face, float faceCenter[3], float direction[3], float amount, bool drawTop = true, bool drawBottom = true);
	static void extrudeConcavePolygon(CoordinateSet faceLeft, CoordinateSet faceRight, float middleLine, float direction[3], float amount, bool drawTop = true, bool drawBottom = true);
	static void drawHemisphere(float radius, int slices, int stack);
	static void connectTwoFaces(CoordinateSet f1, CoordinateSet f2);
	static void rotateAroundXaxis(float vector[3], float angle, float output[3]);
	static void rotateAroundYaxis(float vector[3], float angle, float output[3]);
	static void rotateAroundZaxis(float vector[3], float angle, float output[3]);

	// Coordinate calculations
	static CoordinateSet circleCoords(float center[3], float radius, int edges, float startDegree = 0.0f, float endDegree = 360.0f);
	static CoordinateSet bezierCurveCoords(CoordinateSet points, int divisions);

private:	
	// Bezier curve functions
	static float bezier(float t, float coords[], int points);
	static float bezierDerivative(float t, float coords[], int points);
	static void bezier3d(float t, CoordinateSet coordSet, float output[3]);
	static void bezierDerivative3d(float t, CoordinateSet coordSet, float output[3]);

	// Utility functions for vector calculations
	static void crossProduct(float v1[3], float v2[3], float output[3]);
	static float dotProduct(float v1[3], float v2[3]);
	static float vectorMagnitude(float v[3]);
	static float angleBetweenTwoVectors(float v1[3], float v2[3]);
	static void normalizeVector(float vector[3]);
	static void orthogonalBasis(float vector[3], float basisA[3], float basisB[3]);
	static bool isParallelVector(float v1[3], float v2[3]);
	static float distanceBetweenTwoPoints(float p1[3], float p2[3]);
	static void vectorFromTwoPoints(float p1[3], float p2[3], float output[3]);
};