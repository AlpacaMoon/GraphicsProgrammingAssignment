#include "Color.h"

#include <Windows.h>
#include <gl/GL.h>

void Color::lightBlue() {
	glColor3f(121 / 255.0f, 144 / 255.0f, 160 / 255.0f);
}

void Color::lightGray() {
	glColor3f(168 / 255.0f, 168 / 255.0f, 168 / 255.0f);
}


void Color::red() {
	glColor3f(1, 0, 0);
}
void Color::green() {
	glColor3f(0, 0, 1);
}
void Color::blue() {
	glColor3f(0, 0, 1);
}
void Color::cyan() {
	glColor3f(0, 1, 1);
}
void Color::black() {
	glColor3f(0, 0, 0);
}
void Color::white() {
	glColor3f(1, 1, 1);
}
void Color::yellow() {
	glColor3f(1, 1, 0);
}