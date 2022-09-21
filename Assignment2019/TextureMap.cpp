#include "TextureMap.h"
#include <Windows.h>
#include <gl/GL.h>

TextureMap::TextureMap() {
	for (int i = 0; i < 6; i++) {
		addedTextures[i] = false;
	}
}

/*	Adds texture to this textureMap
	{values of 'face' corresponds to: }
	1) Front
	2) Back
	3) Top
	4) Bottom
	5) Left
	6) Right
*/
void TextureMap::addTexture(GLuint texture, int face) {
	addedTextures[face - 1] = true;
	textures[face - 1] = texture;
}