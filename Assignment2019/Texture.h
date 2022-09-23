#pragma once
#include <Windows.h>
#include <gl/GL.h>

class Texture
{
public:
	// Pathfinder textures
	static GLuint _blue;
	static GLuint _tvTextures[11];
	static GLuint _yellowRope;
	static GLuint _eye;
	static GLuint _blackVent;
	static GLuint _winder;

	// Weapon textures
	static GLuint _gunYellow;
	static GLuint _gunBlack;
	static GLuint _gunGrey;
	
	// Environment textures
	static GLuint _skybox;

	// Change current using texture to the given texture
	static void use(GLuint texture);

	// Enable / Disable texture
	static void on();
	static void off();
	static bool onTexture;

	// Initialize all textures at once
	static void setupTextures();

	// Initialize one texture
	static void initializeTexture(LPCSTR filename, GLuint* texture);

	// Set the magnification and minification filtering methods
	static void setMagMinFilter(int textureMagFilter, int textureMinFilter);

	// Set the texture wrapping methods
	static void setTextureWrap(int wrapHorizontal, int wrapVertical);

	// Variables for magnification and minification filtering methods
	static int magFilter;
	static int minFilter;

	// Variables for texture wrapping methods
	static int wrapS;
	static int wrapT;
};

