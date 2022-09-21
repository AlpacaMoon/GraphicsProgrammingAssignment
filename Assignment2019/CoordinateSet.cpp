#include "CoordinateSet.h"
#include "Utility.h"

/*	Constructor
*/

CoordinateSet::CoordinateSet(int size) {
	this->size = size;
	this->numberOfCoords= 0;
	xCoords = new float[size];
	yCoords = new float[size];
	zCoords = new float[size];
}

/*	Set all coordinates at once
*	The amount of coordinates given must be equal to the size of this CoordinateSet
*/
void CoordinateSet::setAllCoords(float coords[][3]) {
	for (int i = 0; i < this->size; i++) {
		this->xCoords[i] = coords[i][0];
		this->yCoords[i] = coords[i][1];
		this->zCoords[i] = coords[i][2];
	}
	numberOfCoords = size;
}

/*	Add one xyz Coordinate to the set
*/
void CoordinateSet::addCoordinate(float x, float y, float z) {
	if (numberOfCoords == size)
		expandSize(size + 10);
	this->xCoords[numberOfCoords] = x;
	this->yCoords[numberOfCoords] = y;
	this->zCoords[numberOfCoords] = z;
	numberOfCoords++;
}

/*	Add one xyz coordinate to the set
*/
void CoordinateSet::addCoordinate(float xyz[3]) {
	addCoordinate(xyz[0], xyz[1], xyz[2]);
}

/*	Remove the last coordinate from the set
*/
bool CoordinateSet::removeCoordinate() {
	if (numberOfCoords == 0)
		return false;
	this->xCoords[numberOfCoords] = 0;
	this->yCoords[numberOfCoords] = 0;
	this->zCoords[numberOfCoords] = 0;
	numberOfCoords--;
	return true;
}

/*	Remove the coordinate at the given index position from the set
*/
bool CoordinateSet::removeCoordinate(int pos) {
	if (pos < 0 || pos > numberOfCoords) {
		return false;
	}
	for (int j = pos; j < numberOfCoords - 1; j++) {
		this->xCoords[j] = this->xCoords[j + 1];
		this->yCoords[j] = this->yCoords[j + 1];
		this->zCoords[j] = this->zCoords[j + 1];
	}
	this->xCoords[numberOfCoords] = 0;
	this->yCoords[numberOfCoords] = 0;
	this->zCoords[numberOfCoords] = 0;
	numberOfCoords--;
	return true;
}

/*	Returns the xyz coordinates, given its position in the set
*/
void CoordinateSet::getCoords(int pos, float output[3]) {
	pos -= 1;
	output[0] = xCoords[pos];
	output[1] = yCoords[pos];
	output[2] = zCoords[pos];
}


/*	Given another CoordinateSet anotherSet, 
*	Combine thisSet with anotherSet and replace thisSet with the newly combined set
*		The combined set have size = thisSet.size + anotherSet.size
*		The empty/redundant elements in both sets will not be included in the newly combined set
*/
void CoordinateSet::combineCoords(CoordinateSet anotherSet) {
	int newSize = this->size + anotherSet.size;
	this->size = newSize;

	float* newX, * newY, * newZ;
	newX = new float[newSize];
	newY = new float[newSize];
	newZ = new float[newSize];

	for (int i = 0; i < numberOfCoords; i++) {
		newX[i] = this->xCoords[i];
		newY[i] = this->yCoords[i];
		newZ[i] = this->zCoords[i];
	}

	if (xCoords[numberOfCoords - 1] == anotherSet.xCoords[0] &&
		yCoords[numberOfCoords - 1] == anotherSet.yCoords[0] &&
		zCoords[numberOfCoords - 1] == anotherSet.zCoords[0] 
		) {
		numberOfCoords--;
	}

	int i = numberOfCoords;
	for (int j = 0; j < anotherSet.numberOfCoords; j++) {
		newX[i] = anotherSet.xCoords[j];
		newY[i] = anotherSet.yCoords[j];
		newZ[i] = anotherSet.zCoords[j];
		i++;
	}

	this->numberOfCoords += anotherSet.numberOfCoords;

	// Dealloc current memory before assigning new
	destroy();

	this->xCoords = newX;
	this->yCoords = newY;
	this->zCoords = newZ;
}

/*	Return a copy of this CoordinateSet
*/
CoordinateSet CoordinateSet::copy() {
	CoordinateSet another(this->size);
	for (int i = 0; i < this->numberOfCoords; i++) {
		another.addCoordinate(this->xCoords[i], this->yCoords[i], this->zCoords[i]);
	}
	return another;
}


void CoordinateSet::reverse() {
	float* newX = new float[size];
	float* newY = new float[size];
	float* newZ = new float[size];
	int j = 0;
	for (int i = this->numberOfCoords - 1; i >= 0; i--) {
		newX[j] = xCoords[i];
		newY[j] = yCoords[i];
		newZ[j] = zCoords[i];
		j++;
	}
	destroy();
	this->xCoords = newX;
	this->yCoords = newY;
	this->zCoords = newZ;
}

/*	Translate all coordinates based on the given translation
*/
void CoordinateSet::translate(float translation[3]) {
	translate(translation[0], translation[1], translation[2]);
}

/*	Translate all coordinates based on the given translation
*/
void CoordinateSet::translate(float x, float y, float z) {
	for (int i = 0; i < numberOfCoords; i++) {
		xCoords[i] += x;
		yCoords[i] += y;
		zCoords[i] += z;
	}
}

/* Rotates all coordinates around a given axis
*/
void CoordinateSet::rotate(float angleX, float angleY, float angleZ) {
	float temp[3], temp2[3];

	for (int i = 0; i < numberOfCoords; i++) {
		temp[0] = xCoords[i];
		temp[1] = yCoords[i];
		temp[2] = zCoords[i];

		// Apply rotations in this order: Zaxis -> Yaxis -> Xaxis
		// For some reason only this order works
		Utility::rotateAroundZaxis(temp, angleZ, temp2);
		Utility::rotateAroundYaxis(temp2, angleY, temp);
		Utility::rotateAroundXaxis(temp, angleX, temp2);

		xCoords[i] = temp2[0];
		yCoords[i] = temp2[1];
		zCoords[i] = temp2[2];
	}
}

void CoordinateSet::expandSize(int newSize) {
	this->size = newSize;
	float* newX, * newY, * newZ;
	newX = new float[newSize];
	newY = new float[newSize];
	newZ = new float[newSize];

	for (int i = 0; i < numberOfCoords; i++) {
		newX[i] = this->xCoords[i];
		newY[i] = this->yCoords[i];
		newZ[i] = this->zCoords[i];
	}

	// Dealloc current memory before assigning new memory
	destroy();

	this->xCoords = newX;
	this->yCoords = newY;
	this->zCoords = newZ;
}

void CoordinateSet::clear(int size) {
	this->size = size;
	this->numberOfCoords = 0;
	
	// Dealloc current memory before assigning new memory
	destroy();

	xCoords = new float[size];
	yCoords = new float[size];
	zCoords = new float[size];
}

void CoordinateSet::destroy() {
	delete[] xCoords;
	delete[] yCoords;
	delete[] zCoords;
}

float CoordinateSet::getLargestX() {
	if (numberOfCoords < 1) {
		return 0;
	}
	float result = xCoords[0];
	for (int i = 1; i < numberOfCoords; i++) {
		if (xCoords[i] > result)
			result = xCoords[i];
	}
	return result;
}

float CoordinateSet::getLargestY() {
	if (numberOfCoords < 1) {
		return 0;
	}
	float result = yCoords[0];
	for (int i = 1; i < numberOfCoords; i++) {
		if (yCoords[i] > result)
			result = yCoords[i];
	}
	return result;
}

float CoordinateSet::getSmallestX() {
	if (numberOfCoords < 1) {
		return 0;
	}
	float result = xCoords[0];
	for (int i = 1; i < numberOfCoords; i++) {
		if (xCoords[i] < result)
			result = xCoords[i];
	}
	return result;
}

float CoordinateSet::getSmallestY() {
	if (numberOfCoords < 1) {
		return 0;
	}
	float result = yCoords[0];
	for (int i = 1; i < numberOfCoords; i++) {
		if (yCoords[i] < result)
			result = yCoords[i];
	}
	return result;
}

	