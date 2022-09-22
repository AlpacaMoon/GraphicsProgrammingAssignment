#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

class Environment
{
public:

	static bool showSkybox;
	static GLUquadricObj* skyboxObj;
	static void skybox();

	static bool showGridlines;
	static void gridLines(float interval = 0.1f);
};

