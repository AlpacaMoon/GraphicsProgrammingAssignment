#pragma once
#include <Windows.h>
#include <gl/GL.h>
class TextureMap
{
public:
	TextureMap();

	bool addedTextures[3];
	GLuint textures[3];

	void add(GLuint texture, int face);
	bool has(int face);
	GLuint get(int face);

	// Static
	static TextureMap allBlue();
};

