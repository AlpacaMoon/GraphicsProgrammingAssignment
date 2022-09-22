#include "Color.h"
#include <Windows.h>
#include <gl/GL.h>

void Color::lightBlue() {
	//glColor3f(121 / 255.0f, 144 / 255.0f, 160 / 255.0f);
	glColor3f(1, 1, 1);
}

void Color::lightBlue2() {
	//glColor3f(111 / 255.0f, 131 / 255.0f, 152 / 255.0f);
	glColor3f(1, 1, 1);
}

void Color::lightGray() {
	glColor3f(168 / 255.0f, 168 / 255.0f, 168 / 255.0f);
}

void Color::darkGray() {
	glColor3f(80 / 255.0f, 80 / 255.0f, 80 / 255.0f);
}

void Color::gold() {
	glColor3f(255 / 255.0f, 215 / 255.0f, 0);
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