#include "Color.h"
#include "Lightning.h"
#include "Texture.h"
#include <Windows.h>
#include <gl/GL.h>
#include "Texture.h"

void Color::lightBlue() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else if (Texture::currentSkin == 1) {
		glColor3f(121 / 255.0f, 144 / 255.0f, 160 / 255.0f);
	}
	else if (Texture::currentSkin == 2) {
		glColor3f((68 / 255.0), (129 / 255.0), (68 / 255.0));
	}
	else if (Texture::currentSkin == 3) {
		glColor3f((186 / 255.0), (73 / 255.0), (76 / 255.0));
	}
}

void Color::lightBlue2() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else if (Texture::currentSkin == 1) {
		glColor3f(111 / 255.0f, 131 / 255.0f, 152 / 255.0f);
	}
	else if (Texture::currentSkin == 2) {
		glColor3f((58 / 255.0), (117 / 255.0), (60 / 255.0));
	}
	else if (Texture::currentSkin == 3) {
		glColor3f((176 / 255.0), (62 / 255.0), (68 / 255.0));
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
		glColor3f(255 / 255.0f, 215 / 255.0f, 0);
	}
}

void Color::red() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(1, 0, 0);
	}
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
	if (Lightning::onLightning || Texture::onTexture) {
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
	if (Lightning::onLightning || Texture::onTexture) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(1, 1, 0);
	}
}

void Color::silver() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(186 / 255.0f, 186 / 255.0f, 186 / 255.0f);
	}
}

void Color::darkerBrown() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(26 / 255.0f, 21 / 255.0f, 15 / 255.0f);
	}
}

void Color::brown() {
	if (Lightning::onLightning) {
		glColor3f(1, 1, 1);
	}
	else {
		glColor3f(71 / 255.0f, 41 / 255.0f, 17 / 255.0f);
	}
}