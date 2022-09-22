#include "Environment.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Texture.h"
#include "Color.h"
#include "Lightning.h"

bool Environment::showSkybox = true;
GLUquadricObj* Environment::skyboxObj;

bool Environment::showGridlines = true;

void Environment::skybox() {
	if (!showSkybox)
		return;

	if (skyboxObj == NULL) {
		skyboxObj = gluNewQuadric();
	}
	gluQuadricOrientation(skyboxObj, GLU_INSIDE);

	bool isLightingOn = Lightning::onLightning;
	Lightning::onLightning = false;
	Lightning::lightningSwitch();
	glPushMatrix();
	{
		Texture::on();
		Texture::use(Texture::_skybox);
		Color::white();
		glRotatef(-90, 1, 0, 0);
		gluQuadricTexture(skyboxObj, GL_TRUE);
		gluSphere(skyboxObj, 30, 300, 300);
	}
	glPopMatrix();
	if (isLightingOn) {
		Lightning::onLightning = true;
		Lightning::lightningSwitch();
	}
	Texture::off();
}


void Environment::gridLines(float interval) {
	if (!showGridlines)
		return;
	glColor3f(0.9, 0.9, 0.9);
	glPushMatrix();
	float limit = 2.0f;

	bool isLightingOn = Lightning::onLightning;
	Lightning::onLightning = false;
	Lightning::lightningSwitch();
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
	glPopMatrix();
	if (isLightingOn) {
		Lightning::onLightning = true;
		Lightning::lightningSwitch();
	}
}