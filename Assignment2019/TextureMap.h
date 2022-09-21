#pragma once
#include <Windows.h>
#include <gl/GL.h>
class TextureMap
{
	TextureMap();

	bool addedTextures[6];
	GLuint textures[6];

	void addTexture(GLuint texture, int face);

};

