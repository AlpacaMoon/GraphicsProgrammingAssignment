#include "Lightning.h";
#include <Windows.h>
#include <gl/GL.h>

//Lightning variables 
//blue
GLfloat blueMaterialAmbient[4] = { (120 / 255.0), (142 / 255.0), (163 / 255.0),1 };
GLfloat blueMaterialDiffuse[4] = { (120 / 255.0), (142 / 255.0), (163 / 255.0),1 };
//yellow
GLfloat yellowMaterialAmbient[4] = { (254 / 255.0), (217 / 255.0), (79 / 255.0),1 };
GLfloat yellowMaterialDiffuse[4] = { (254 / 255.0), (217 / 255.0), (79 / 255.0),1 };
//grey
GLfloat greyMaterialAmbient[4] = { (79 / 255.0), (79 / 255.0), (78 / 255.0),1 };
GLfloat greyMaterialDiffuse[4] = { (79 / 255.0), (79 / 255.0), (78 / 255.0),1 };
//wine red
GLfloat windRedMaterialAmbient[4] = { (142 / 255.0), (21 / 255.0), (5 / 255.0),1 };
GLfloat windRedMaterialDiffuse[4] = { (142 / 255.0), (21 / 255.0), (5 / 255.0),1 };
//gold
GLfloat goldMaterialAmbient[4] = { (235 / 255.0), (159 / 255.0), (65 / 255.0),1 };
GLfloat goldMaterialDiffuse[4] = { (235 / 255.0), (159 / 255.0), (65 / 255.0),1 };
//DarkerBlue
GLfloat darkBlueMaterialAmbient[4] = { (111 / 255.0), (131 / 255.0), (152 / 255.0),1 };
GLfloat darkBlueMaterialDiffuse[4] = { (111 / 255.0), (131 / 255.0), (152 / 255.0),1 };
//DarkerGrey
GLfloat darkGreyMaterialAmbient[4] = { (40 / 255.0), (41 / 255.0), (43 / 255.0),1 };
GLfloat darkGreyMaterialDiffuse[4] = { (40 / 255.0), (41 / 255.0), (43 / 255.0),1 };
//Black
GLfloat blackMaterialAmbient[4] = { 0,0,0,1 };
GLfloat blackMaterialDiffuse[4] = { 0,0,0,1 };

void Lightning::blueMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blueMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blueMaterialDiffuse);
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
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, darkBlueMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, darkBlueMaterialDiffuse);
}

void Lightning::darkerGreyMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, darkGreyMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, darkGreyMaterialDiffuse);
}

void Lightning::blackMaterial() {
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blackMaterialAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blackMaterialDiffuse);
}
