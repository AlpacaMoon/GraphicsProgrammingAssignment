#include "TextureMap.h"
#include <Windows.h>
#include <gl/GL.h>
#include "Texture.h"

TextureMap::TextureMap() {
	for (int i = 0; i < 3; i++) {
		addedTextures[i] = false;
		textures[i] = 0;
	}
}

/*	Adds texture to this textureMap
	{values of 'face' corresponds to: }
	1) Front
	2) Back
	3) Side
*/
void TextureMap::add(GLuint texture, int face) {
	addedTextures[face - 1] = true;
	textures[face - 1] = texture;
}

bool TextureMap::has(int face) {
	return addedTextures[face - 1];
}

GLuint TextureMap::get(int face) {
	return textures[face - 1];
}

// Static methods that returns a texture map with the same texture on all sides

TextureMap TextureMap::allBlue() {
	TextureMap tmap;
	tmap.add(Texture::_blue, 1);
	tmap.add(Texture::_blue, 2);
	tmap.add(Texture::_blue, 3);
	return tmap;
}

TextureMap TextureMap::gunBlack() {
	TextureMap tmap;
	tmap.add(Texture::_gunBlack, 1);
	tmap.add(Texture::_gunBlack, 2);
	tmap.add(Texture::_gunBlack, 3);
	return tmap;
}

TextureMap TextureMap::gunYellow() {
	TextureMap tmap;
	tmap.add(Texture::_gunYellow, 1);
	tmap.add(Texture::_gunYellow, 2);
	tmap.add(Texture::_gunYellow, 3);
	return tmap;
}

TextureMap TextureMap::gunGrey() {
	TextureMap tmap;
	tmap.add(Texture::_gunGrey, 1);
	tmap.add(Texture::_gunGrey, 2);
	tmap.add(Texture::_gunGrey, 3);
	return tmap;
}