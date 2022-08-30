
#include <Windows.h>
#include <gl/GL.h>
#include <math.h>

#include "Utility.h"
#include "CoordinateSet.h"
#include "Model.h"
#include "Animation.h"

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"
const float PI = 3.1415926f;
const int X_AXIS[3] = { 1, 0, 0 };
const int Y_AXIS[3] = { 0, 1, 0 };
const int Z_AXIS[3] = { 0, 0, 1 };

float camRotation[3] = { 0, 0, 0 };
float camRotateSpeed = 2.0f;

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
	glClearColor(0.69, 0.69, 0.69, 1.0);

	glLoadIdentity();

	glRotatef(camRotation[0], 1, 0, 0);
	glRotatef(camRotation[1], 0, 1, 0);
	glRotatef(camRotation[2], 0, 0, 1);

	glScalef(0.8, 0.8, 0.8);
	
	glPushMatrix();
	{
		// Draw square + extrude square
		//CoordinateSet temp(10);
		//temp.addCoordinate(0, 0, 0);
		//temp.addCoordinate(0.5, 0, 0);
		//temp.addCoordinate(0.5, 0.5, 0);
		//temp.addCoordinate(0, 0.5, 0);
		//Utility::drawPolygon(temp, new float[3]{ 0.25, 0.25, 0 });
		//float zAxis[3] = {0, 0, 1};
		//Utility::extrudePolygon(temp, new float[3]{ 0.25, 0.25, 0 }, zAxis, 0.5f, false, false);


		// Draw bezier line + combine coordinateSet
		//glColor3f(1, 0, 0);
		//CoordinateSet temp2(10);
		//temp2.addCoordinate(0, 0, 0);
		//temp2.addCoordinate(0, 0.5, 0);
		//temp2.addCoordinate(0.87, 0.87, 0);
		//temp2 = Utility::bezierCurveCoords(temp2, 10);
		//temp2.addCoordinate(...);
		//CoordinateSet temp3(10);
		//temp3.addCoordinate(...);
		//temp3 = Utility::bezierCurveCoords(temp3, 10);
		//temp2.combineCoords(temp3);
	}
	glPopMatrix();


	// Gridlines
	glColor3f(0.78, 0.78, 0.78);
	Utility::drawGrids();

	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}
//--------------------------------------------------------------------

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