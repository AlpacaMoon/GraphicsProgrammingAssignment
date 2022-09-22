#include "Color.h"
#include "Lightning.h"
#include <Windows.h>
#include <gl/GL.h>

void Color::lightBlue() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(121 / 255.0f, 144 / 255.0f, 160 / 255.0f);
	}
}

void Color::lightBlue2() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(111 / 255.0f, 131 / 255.0f, 152 / 255.0f);
	}
}

void Color::lightGrey() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(168 / 255.0f, 168 / 255.0f, 168 / 255.0f);
	}
}

void Color::darkGrey() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(80 / 255.0f, 80 / 255.0f, 80 / 255.0f);
	}
	
}

void Color::gold() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(168 / 255.0f, 168 / 255.0f, 168 / 255.0f);
	}
	glColor3f(255 / 255.0f, 215 / 255.0f, 0);
}

void Color::red() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(168 / 255.0f, 168 / 255.0f, 168 / 255.0f);
	}
	glColor3f(1, 0, 0);
}
void Color::green() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(0, 0, 1);
	}
	
}
void Color::blue() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(0, 0, 1);
	}
	
}
void Color::cyan() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(0, 1, 1);
	}
	
}
void Color::black() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(0, 0, 0);
	}
	
}

void Color::white() {
	glColor3f(1, 1, 1);
}

void Color::yellow() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(1, 1, 0);
	}
}