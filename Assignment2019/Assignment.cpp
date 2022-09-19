
#include <Windows.h>
#include <gl/GL.h>
#include <math.h>

#include "Utility.h"
#include "CoordinateSet.h"
#include "Model.h"
#include "Animation.h"
#include "Color.h"
#include "Controls.h"

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"
const float PI = 3.1415926f;
const int X_AXIS[3] = { 1, 0, 0 };
const int Y_AXIS[3] = { 0, 1, 0 };
const int Z_AXIS[3] = { 0, 0, 1 };

float camRotation[3] = { 0, 0, 0 };
float camRotateSpeed = 5.0f;

bool showGrid = true;

bool onLightning = false;

//lighting test
GLfloat ambientLight[4] = { 1,1,1,1 }; //RGBA
//if want diffuse light change to all 1
GLfloat diffuseLight[4] = { 0,0,0,1 }; //RGBA
GLfloat positionLight[4] = { 0,10,0,0 }; //x,y,z,0

// ambient = no direction (apply to every model seperately based on color)
GLfloat materialAmbient[4] = { 64/255.0,95/255.0,107/255.0,1 }; //default value
//diffuse = directioned light (light of bulb)
GLfloat materialDiffuse[4] = { 64 / 255.0,95 / 255.0,107 / 255.0,1 }; //default value

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool inputting = false;
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) PostQuitMessage(0);
		else {
			switch (wParam) {
			case VK_UP: 
				camRotation[0] -= camRotateSpeed;
				inputting = true;
				break;
			case VK_DOWN:
				camRotation[0] += camRotateSpeed;
				inputting = true;
				break;
			case VK_LEFT: 
				camRotation[1] -= camRotateSpeed;
				inputting = true;
				break;
			case VK_RIGHT: 
				camRotation[1] += camRotateSpeed;
				inputting = true;
				break;
			case VK_OEM_COMMA: 
				camRotation[2] += camRotateSpeed;
				inputting = true;
				break;
			case VK_OEM_PERIOD: 
				camRotation[2] -= camRotateSpeed;
				inputting = true;
				break;
			case VK_SPACE:
				camRotation[0] = 0;
				camRotation[1] = 0;
				camRotation[2] = 0;
				break;

			case VK_NUMPAD2:
				camRotation[0] = 0;
				camRotation[1] = 0;
				camRotation[2] = 0;
				break;
			case VK_NUMPAD8:
				camRotation[0] = 0;
				camRotation[1] = 180;
				camRotation[2] = 0;
				break;
			case VK_NUMPAD4:
				camRotation[0] = 0;
				camRotation[1] = 270;
				camRotation[2] = 0;
				break;
			case VK_NUMPAD6:
				camRotation[0] = 0;
				camRotation[1] = 90;
				camRotation[2] = 0;
				break;
			case VK_NUMPAD5:
				camRotation[0] = -90;
				camRotation[1] = 0;
				camRotation[2] = 0;
				break;

			case VK_F1:
				onLightning=!onLightning;
				break;

			case VK_F2:
				showGrid = !showGrid;
				break;

			case VK_NUMPAD0:
				Controls::isIndependentControls = !Controls::isIndependentControls;
				break;
			}

			if (Controls::isIndependentControls) {
				Controls::independentControls(wParam);
			}
			else {
				Controls::presetAnimationControls(wParam);
			}
		}
		break;

	case WM_KEYUP:

	default:
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}
//--------------------------------------------------------------------


bool initPixelFormat(HDC hdc)
{
	PIXELFORMATDESCRIPTOR pfd;
	ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));

	pfd.cAlphaBits = 8;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 0;

	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;

	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;

	// choose pixel format returns the number most similar pixel format available
	int n = ChoosePixelFormat(hdc, &pfd);

	// set pixel format returns whether it sucessfully set the pixel format
	if (SetPixelFormat(hdc, n, &pfd))
	{
		return true;
	}
	else
	{
		return false;
	}
}
//--------------------------------------------------------------------

void display()
{
	//--------------------------------
	//	OpenGL drawing
	//--------------------------------
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(1, 1, 1, 1);

	if (onLightning) {
		//enable which type of light
		glEnable(GL_LIGHT0);
		//enable lighting
		glEnable(GL_LIGHTING);
		//GL_FRONT_AND_BACK = outside and inside of polygon
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
	}
	else {
		//enable which type of light
		glDisable(GL_LIGHT0);
		//enable lighting
		glDisable(GL_LIGHTING);
	}

	glLoadIdentity();

	glRotatef(camRotation[0], 1, 0, 0);
	glRotatef(camRotation[1], 0, 1, 0);
	glRotatef(camRotation[2], 0, 0, 1);

	glScalef(0.8, 0.8, 0.8);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	{
		Model::Pathfinder();
	}
	glPopMatrix();


	// Gridlines
	if (showGrid) {
		glColor3f(0.9,0.9, 0.9);
		Utility::drawGrids();
	}

	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------
void setupEnvironmentLightning() {
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, positionLight);
}

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow)
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = WindowProcedure;
	wc.lpszClassName = WINDOW_TITLE;
	wc.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClassEx(&wc)) return false;

	HWND hWnd = CreateWindow(WINDOW_TITLE, WINDOW_TITLE, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1000, 1000,
		NULL, NULL, wc.hInstance, NULL);

	//--------------------------------
	//	Initialize window for OpenGL
	//--------------------------------

	HDC hdc = GetDC(hWnd);

	//	initialize pixel format for the window
	initPixelFormat(hdc);

	//	get an openGL context
	HGLRC hglrc = wglCreateContext(hdc);

	//	make context current
	if (!wglMakeCurrent(hdc, hglrc)) return false;

	//--------------------------------
	//	End initialization
	//--------------------------------

	// Enable depth test AFTER initialization
	glEnable(GL_DEPTH_TEST);

	//Lightning setup
	setupEnvironmentLightning();

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------