#include "Texture.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>

// Change current using texture to the given texture
void Texture::use(GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
}

// Initialize all textures at once
void Texture::setupTextures() {
	//initializeTexture("brick.bmp", &brickTexture);
}

// Initialize one texture
void Texture::initializeTexture(LPCSTR filename, GLuint* texture) {
	BITMAP BMP;
	HBITMAP hBMP = NULL;

	// -This function is used to change how pixel data is stored and retrieved.
		// - GL_UNPACK_ALIGNMENT specifies how OpenGL unpacks image data from data buffers.
		// - A bitmap file format uses 4 - byte alignment for its pixel data.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	// - This function is going to load a bitmap file to a WINAPI BITMAP handle.
	// LOAD BITMAP
	hBMP = (HBITMAP)LoadImage(GetModuleHandle(NULL),
		filename, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION |
		LR_LOADFROMFILE);

	// - GetObject will load bitmap data from the handle into the BITMAP structure.
	GetObject(hBMP, sizeof(BMP), &BMP);

	// - This function will return 1 currently unused names for the given texture object.
	// Because there are no textures now, boxTexture will be assigned to slot 1 (Theoretically)
	glGenTextures(1, texture);

	// - When the texture name is used for the first time, this function will create a new texture
	//   object and assigned with the given name.
	// - This function is also called when we want to bind a previously created texture to an object.
	// IN SHORT, change current texture to boxTexture, or GL_TEXTURE_2D = boxTexture
	glBindTexture(GL_TEXTURE_2D, *texture);

	// - This function is used to specify the magnification and minification filtering methods.
	// GL_LINEAR or GL_NEAREST for the last param
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Texture::magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Texture::magFilter);

	// GL_TEXTURE_WRAP_S = Horizontal
	// GL_TEXTURE_WRAP_T = Vertical
	// GL_CLAMP or GL_REPEAT for the last param
	//		GL_CLAMP = Only draw one unit of texture
	//		GL_REPEAT = Draw units of texture infinitely
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Texture::wrapS);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Texture::wrapT);

	// - This function is used to define a two dimensional texture.
	//			target,		level, internalFormat, width, height,	border, format, type,			*pixels
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);

	// - Delete the bitmap handle once you create the texture object.
	DeleteObject(hBMP);
}

void Texture::setMagMinFilter(int textureMagFilter, int textureMinFilter) {
	if (textureMagFilter == GL_NEAREST || textureMagFilter == GL_LINEAR) {
		Texture::magFilter = textureMagFilter;
	}
	if (textureMinFilter == GL_NEAREST || textureMinFilter == GL_LINEAR) {
		Texture::minFilter = textureMinFilter;
	}
}

void Texture::setTextureWrap(int wrapHorizontal, int wrapVertical) {
	if (wrapHorizontal == GL_CLAMP || wrapHorizontal == GL_REPEAT) {
		Texture::wrapS = wrapHorizontal;
	}
	if (wrapVertical == GL_CLAMP || wrapVertical == GL_REPEAT) {
		Texture::wrapT = wrapVertical;
	}
}