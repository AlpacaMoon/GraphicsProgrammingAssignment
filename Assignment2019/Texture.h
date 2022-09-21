#pragma once
#include <Windows.h>
#include <gl/GL.h>

class Texture
{
public:
	// Texture GLuint
	static GLuint _blue1;
	static GLuint _blue2;

	// Change current using texture to the given texture
	static void use(GLuint texture);

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

