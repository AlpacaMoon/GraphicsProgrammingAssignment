#pragma once
#include <Windows.h>
#include <gl/GL.h>
class Lightning {
public: 
	static bool onLightning;
	static void lightningSwitch();

	static GLfloat ambientLight[4];
	static GLfloat diffuseLight[4];
	static GLfloat positionLight[4];
	static void initialize();
	static bool visualizeLightPos;
	

	static void defaultMaterial();

	static void bodyMainMaterial();
	
	static void blueSkinMaterial();
	static void greenSkinMaterial();
	static void redSkinMaterial();

	static void yellowMaterial();

	static void greyMaterial();

	static void windRedMaterial();

	static void goldMaterial();

	static void darkerBlueMaterial();

	static void darkerGreyMaterial();

	static void blackMaterial();

	static void silverMaterial();

	static void brownMaterial();

	static void darkerBrownMaterial();

	static void blackVentMaterial();

	static void tvScreenMaterial();

};

