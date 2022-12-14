#include "Lightning.h";
#include "Texture.h";
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

//Lightning variables 

bool Lightning::onLightning = true;

GLfloat Lightning::ambientLight[4];
GLfloat Lightning::diffuseLight[4];
GLfloat Lightning::positionLight[4];
bool Lightning::visualizeLightPos = false;

//blue
//GLfloat blueMaterialAmbient[4] = { (120 / 255.0), (142 / 255.0), (163 / 255.0),1 };
//GLfloat blueMaterialDiffuse[4] = { (120 / 255.0), (142 / 255.0), (163 / 255.0),1 };
GLfloat blueMaterialAmbient[4] = { (122 / 255.0), (138 / 255.0), (154 / 255.0),1 };
GLfloat blueMaterialDiffuse[4] = { (152 / 255.0), (168 / 255.0), (184 / 255.0),1 };
GLfloat greenMaterialAmbient[4] = { (68 / 255.0), (129 / 255.0), (68 / 255.0),1 };
GLfloat greenMaterialDiffuse[4] = { (68 / 255.0), (129 / 255.0), (68 / 255.0),1 };
GLfloat redMaterialAmbient[4] = { (186 / 255.0), (73 / 255.0), (76 / 255.0),1 };
GLfloat redMaterialDiffuse[4] = { (186 / 255.0), (73 / 255.0), (76 / 255.0),1 };
//yellow
GLfloat yellowMaterialAmbient[4] = { (254 / 255.0), (217 / 255.0), (79 / 255.0),1 };
GLfloat yellowMaterialDiffuse[4] = {0, 0, 0,1 };
//grey
GLfloat greyMaterialAmbient[4] = { (99 / 255.0), (99 / 255.0), (98 / 255.0),1 };
GLfloat greyMaterialDiffuse[4] = { (99 / 255.0), (99 / 255.0), (98 / 255.0),1 };
//wine red
GLfloat windRedMaterialAmbient[4] = { (142 / 255.0), (21 / 255.0), (5 / 255.0),1 };
GLfloat windRedMaterialDiffuse[4] = { (142 / 255.0), (21 / 255.0), (5 / 255.0),1 };
//gold
GLfloat goldMaterialAmbient[4] = { (235 / 255.0), (159 / 255.0), (65 / 255.0),1 };
GLfloat goldMaterialDiffuse[4] = { (235 / 255.0), (159 / 255.0), (65 / 255.0),1 };
//DarkerBlue
GLfloat darkBlueMaterialAmbient[4] = { (111 / 255.0), (131 / 255.0), (152 / 255.0),1 };
GLfloat darkBlueMaterialDiffuse[4] = { (111 / 255.0), (131 / 255.0), (152 / 255.0),1 };
////DarkerGrey
GLfloat darkGreyMaterialAmbient[4] = { (70 / 255.0), (71 / 255.0), (73 / 255.0),1 };
GLfloat darkGreyMaterialDiffuse[4] = { (70 / 255.0), (71 / 255.0), (73 / 255.0),1 };
//Black
GLfloat blackMaterialAmbient[4] = { 0,0,0,1 };
GLfloat blackMaterialDiffuse[4] = { 0,0,0,1 };
//silver
GLfloat silverMaterialAmbient[4] = { (126 / 255.0), (126 / 255.0), (126 / 255.0),1 };
GLfloat silverMaterialDiffuse[4] = { (216 / 255.0), (216 / 255.0), (216 / 255.0),1 };
//dark brown
GLfloat darkBrownMaterialAmbient[4] = { (26 / 255.0), (21 / 255.0), (15 / 255.0),1 };
GLfloat darkBrownMaterialDiffuse[4] = { (26 / 255.0), (21 / 255.0), (15 / 255.0),1 };
//brown
GLfloat brownMaterialAmbient[4] = { (71 / 255.0), (41 / 255.0), (17 / 255.0),1 };
GLfloat brownMaterialDiffuse[4] = { (71 / 255.0), (41 / 255.0), (17 / 255.0),1 };

// Specific materials for certain body parts
//Black Vent
GLfloat blackVentMaterialAmbient[4] = { (170 / 255.0), (171 / 255.0), (173 / 255.0),1 };
GLfloat blackVentMaterialDiffuse[4] = { (170 / 255.0), (171 / 255.0), (173 / 255.0),1 };


// Default
GLfloat defaultMatAmbient[4] = { 0.2, 0.2, 0.2, 1 };
GLfloat defaultMatDiffuse[4] = { 0.8, 0.8, 0.8, 1 };

GLUquadricObj* lightConcrete;

void Lightning::initialize() {
	for (int i = 0; i < 4; i++) {
		ambientLight[i] = 1;
		diffuseLight[i] = 1;
	}
	positionLight[0] = 0;
	positionLight[1] = 5;
	positionLight[2] = 0;
	positionLight[3] = 0;
}

void Lightning::lightningSwitch() {

	// Update light position
	glLightfv(GL_LIGHT0, GL_POSITION, Lightning::positionLight);

	// Visualize light position using a small sphere
	if (visualizeLightPos) {
		if (lightConcrete == NULL)
			lightConcrete = gluNewQuadric();
		glPushMatrix();
		{
			glDisable(GL_LIGHTING);
			glColor3f(1, 0.8, 0);
			glTranslatef(positionLight[0], positionLight[1], positionLight[2]);
			gluSphere(lightConcrete, 0.1, 16, 16);
		}
		glPopMatrix();
	}

	// Light switch
	if (onLightning) {
		//enable which type of light
		glEnable(GL_LIGHT0);
		//enable lighting
		glEnable(GL_LIGHTING);
	}
	else {
		//enable which type of light
		glDisable(GL_LIGHT0);
		//enable lighting
		glDisable(GL_LIGHTING);
	}
}

void Lightning::defaultMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, defaultMatAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, defaultMatDiffuse);
}

void Lightning::bodyMainMaterial() {
	switch (Texture::currentSkin) {
	case 1:
		blueSkinMaterial();
		break;
	case 2:
		greenSkinMaterial();
		break;
	case 3:
		redSkinMaterial();
		break;
	}
}

void Lightning::goldMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, goldMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, goldMaterialDiffuse);
}


void Lightning::yellowMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, yellowMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, yellowMaterialDiffuse);
}

void Lightning::greyMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, greyMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, greyMaterialDiffuse);
}

void Lightning::windRedMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, windRedMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, windRedMaterialDiffuse);
}

void Lightning::darkerBlueMaterial() {
	switch (Texture::currentSkin) {
	case 1:
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, darkBlueMaterialAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, darkBlueMaterialDiffuse);
		break;
	case 2:
		greenSkinMaterial();
		break;
	case 3:
		redSkinMaterial();
		break;
	}
}

void Lightning::darkerGreyMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, darkGreyMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, darkGreyMaterialDiffuse);
}

void Lightning::blackMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackMaterialDiffuse);
}

void Lightning::silverMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, silverMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, silverMaterialDiffuse);
}

void Lightning::darkerBrownMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, darkBrownMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, darkBrownMaterialDiffuse);
}

void Lightning::brownMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, brownMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, brownMaterialDiffuse);
}

void Lightning::blackVentMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackVentMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackVentMaterialDiffuse);
}


void Lightning::blueSkinMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blueMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blueMaterialDiffuse);
}
void Lightning::greenSkinMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, greenMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, greenMaterialDiffuse);
}
void Lightning::redSkinMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, redMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, redMaterialDiffuse);
}
void Lightning::tvScreenMaterial() {
	defaultMaterial();
}