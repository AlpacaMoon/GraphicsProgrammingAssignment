#pragma once

class CoordinateSet {
public:
	float* xCoords, * yCoords, * zCoords;
	int size, numberOfCoords;
	CoordinateSet(int size);

	void setAllCoords(float coords[][3]);

	void addCoordinate(float x, float y, float z);
	void addCoordinate(float xyz[3]);
	bool removeCoordinate();
	bool removeCoordinate(int pos);
	void getCoords(int pos, float output[3]);

	void combineCoords(CoordinateSet anotherSet);

	CoordinateSet copy();

	void translate(float translation[3]);
	void translate(float x, float y, float z);
	void rotate(float angleX, float angleY, float angleZ);

	void expandSize(int newSize);

	void clear(int size);

	void reverse();

	void destroy();
};