#include <Windowsx.h>
#include <Windows.h>
#include <gl/GL.h>
#include <math.h>
#include <ctime>

#include "Utility.h"
#include "CoordinateSet.h"
#include "Model.h"
#include "Animation.h"
#include "Color.h"
#include "Controls.h"
#include "Lightning.h"
#include "Time.h"
#include "Texture.h"
#include "Environment.h"

#pragma comment (lib, "OpenGL32.lib")

#define WINDOW_TITLE "OpenGL Window"
const float PI = 3.1415926f;
const int X_AXIS[3] = { 1, 0, 0 };
const int Y_AXIS[3] = { 0, 1, 0 };
const int Z_AXIS[3] = { 0, 0, 1 };

float camRotation[3] = { 0,0,0 };
float camRotateSpeed = 5.0f;

//lighting 
GLfloat ambientLight[4] = { 1,1,1,1 }; //RGBA
GLfloat diffuseLight[4] = { 1,1,1,1 }; //RGBA
GLfloat positionLight[4] = { 0,5,0,0 }; //x,y,z,0

//lookAt
float eye[3] = { 0,0,5 };
float tempEye[3] = { 0,0,0 };
float lookAt[3] = { 0,0,0 };
float up[3] = { 0,1,0 };

bool isOrtho = false;

// mouse movement
float lastX = 0.0f, lastY = 0.0f;

LRESULT WINAPI WindowProcedure(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	bool inputting = false;
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_MOUSEMOVE:
		switch (wParam) {
		case MK_RBUTTON:
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			camRotation[1] += (xPos - lastX) / 2;
			camRotation[0] += (yPos - lastY) / 2;
			lastX = xPos;
			lastY = yPos;
			break;
		}
		break;

	case WM_RBUTTONDOWN:
		lastX = GET_X_LPARAM(lParam);
		lastY = GET_Y_LPARAM(lParam);
		break;

	case WM_LBUTTONDOWN:
		if (Model::currentWeapon == 1)
			Model::isFired = true;
		break;

	case WM_LBUTTONUP:
		if (Model::currentWeapon == 1)
			Model::isFired = false;
		break;

	case WM_MOUSEWHEEL:
		if (eye[2] <= 20 && eye[2] >= 2.0) {
			eye[2] -= GET_WHEEL_DELTA_WPARAM(wParam) / 240.0f;
		}
		else if (eye[2] > 20) {
			eye[2] = 20.0f;
		}
		else if (eye[2] < 2.0) {
			eye[2] = 2.0f;
		}
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

			case VK_NUMPAD0:
				isOrtho = !isOrtho;
				break;

			case VK_BACK:
				camRotation[0] = 0;
				camRotation[1] = 0;
				camRotation[2] = 0;
				break;


			case VK_F1:
				Lightning::onLightning = !Lightning::onLightning;
				break;

			case VK_F2:
				Environment::showGridlines = !Environment::showGridlines;
				break;

			case VK_F3:
				Controls::isIndependentControls = !Controls::isIndependentControls;
				Animation::cancelAllAnimations();
				Animation::startReset();
				break;

			case VK_F4:
				Environment::showSkybox = !Environment::showSkybox;
				break;

			case VK_F5:
				Texture::onTexture = !Texture::onTexture;
				break;

			}

			if (Controls::isIndependentControls) {
				Controls::independentControls(wParam);
			}
			else {
				Controls::presetAnimationKeyDown(wParam);
			}
		}
		break;

	case WM_KEYUP:
		if (!Controls::isIndependentControls) {
			Controls::presetAnimationKeyUp(wParam);
		}
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
	//glClearColor(1, 1, 1, 1);
	glClearColor(0.8, 0.8, 0.8, 1);

	// Lighting switch
	Lightning::lightningSwitch();

	// Real thing
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2], up[0], up[1], up[2]);


	glRotatef(camRotation[0], 1, 0, 0);
	glRotatef(camRotation[1], 0, 1, 0);
	glRotatef(camRotation[2], 0, 0, 1);

	Environment::skybox();
	Environment::gridLines();
	Animation::runAnimations();

	glScalef(-0.8, 0.8, 0.8);
	glPushMatrix();
	{
		//Model::Pathfinder();
		//Model::r99();
		Model::kukriKnife();

	}
	glPopMatrix();

	//--------------------------------
	//	End of OpenGL drawing
	//--------------------------------
}

//--------------------------------------------------------------------
void setupEnvironmentLightning() {
	Lightning::onLightning = false;

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_POSITION, positionLight);
}

void setupCamera()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (isOrtho) {
		glOrtho(-1.5f, 1.5f, -1.5f, 1.5f, -2, 100);
	}
	else {
		gluPerspective(35, 1, 1, 100);
	}
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
	
	// Texture setup
	Texture::setupTextures();

	// Models setup
	Model::initializeBulletPositions();

	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (true)
	{
		//camera setup
		setupCamera();

		Time::currentTicks = clock();

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		display();

		SwapBuffers(hdc);

		Time::elapsedTicks = clock() - Time::currentTicks;
		Time::elapsedSeconds = Time::toSeconds(Time::elapsedTicks);
	}

	UnregisterClass(WINDOW_TITLE, wc.hInstance);

	return true;
}
//--------------------------------------------------------------------