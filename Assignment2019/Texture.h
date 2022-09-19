#pragma once
#include <Windows.h>
#include <gl/GL.h>

class Texture
{
public:
	// Texture GLuint
	GLuint _lightBlue;

	// Change current using texture to the given texture
	void use(GLuint texture);

	// Initialize all textures at once
	void setupTextures();

	// Initialize one texture
	void initializeTexture(LPCSTR filename, GLuint* texture);

	// Set the magnification and minification filtering methods
	void setMagMinFilter(int textureMagFilter, int textureMinFilter);

	// Set the texture wrapping methods
	void setTextureWrap(int wrapHorizontal, int wrapVertical);

	// Variables for magnification and minification filtering methods
	int magFilter = GL_LINEAR;
	int minFilter = GL_LINEAR;

	// Variables for texture wrapping methods
	int wrapS = GL_REPEAT;
	int wrapT = GL_REPEAT;
};

