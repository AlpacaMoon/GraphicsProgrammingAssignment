#include "Texture.h"
#include <Windows.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Model.h"
#include "Animation.h"

// Static variables
int Texture::magFilter = GL_LINEAR;
int Texture::minFilter = GL_LINEAR;
int Texture::wrapS = GL_REPEAT;
int Texture::wrapT = GL_REPEAT;

//texture control
bool Texture::onTexture = true;

// Pathfinder textures
GLuint Texture::_bodyTexture;
GLuint Texture::_yellow;
GLuint Texture::_yellowRope;
GLuint Texture::_eye;
GLuint Texture::_blackVent;
GLuint Texture::_winder;
GLuint Texture::_tvTextures[11];

//weapon texture
GLuint Texture::_gunYellow;
GLuint Texture::_gunBlack;
GLuint Texture::_gunGrey;
GLuint Texture::_wood;
GLuint Texture::_darkerWood;
GLuint Texture::_metal;

// Environment Textures
GLuint Texture::_skybox;

// Current skin texture
/*	1 = blue skin
*	2 = green skin
*	3 = red skin
*/
int Texture::currentSkin = 1;
GLuint Texture::_blueSkin;
GLuint Texture::_greenSkin;
GLuint Texture::_redSkin;


// Change current using texture to the given texture
void Texture::use(GLuint texture) {
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::on() {
	if (onTexture) {
		glEnable(GL_TEXTURE_2D);
	}
}

void Texture::off() {
	glDisable(GL_TEXTURE_2D);
}

// Initialize all textures at once
void Texture::setupTextures() {
	// General Pathfinder Textures
	initializeTexture("Textures/yellow.bmp", &_yellow);
	initializeTexture("Textures/yellowRope.bmp", &_yellowRope);
	initializeTexture("Textures/eye.bmp", &_eye);
	initializeTexture("Textures/blackVent.bmp", &_blackVent);
	initializeTexture("Textures/winder.bmp", &_winder);

	// Skins
	initializeTexture("Textures/blueSkin.bmp", &_blueSkin);
	initializeTexture("Textures/greenSkin.bmp", &_greenSkin);
	initializeTexture("Textures/redSkin.bmp", &_redSkin);
	_bodyTexture = _blueSkin;

	// TV Textures
	LPCSTR tvTextureFiles[11] = {
		"Textures/tvAngry.bmp",
		"Textures/tvBlank.bmp",
		"Textures/tvDead.bmp",
		"Textures/tvExclamation.bmp",
		"Textures/tvGlitched.bmp",
		"Textures/tvHappy.bmp",
		"Textures/tvKO.bmp",
		"Textures/tvLove.bmp",
		"Textures/tvQuestion-mark.bmp",
		"Textures/tvSad.bmp",
		"Textures/tvThumbs-Up.bmp",
	};
	for (int i = 0; i < 11; i++) {
		initializeTexture(tvTextureFiles[i], &_tvTextures[i]);
	}
	Animation::currentTVscreen = 5;
	
	// Skybox
	initializeTexture("Textures/skybox.bmp", &_skybox);

	//weapon texture
	initializeTexture("Textures/r99_1.bmp", &_gunYellow);
	initializeTexture("Textures/r99_2.bmp", &_gunBlack);
	initializeTexture("Textures/r99_3.bmp", &_gunGrey);
	initializeTexture("Textures/wood.bmp", &_wood);
	initializeTexture("Textures/darkerWood.bmp", &_darkerWood);
	initializeTexture("Textures/metal.bmp", &_metal);
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

void Texture::changeSkin(int n) {
	currentSkin = n;
	switch (n) {
	case 1:
		_bodyTexture = _blueSkin;
		break;
	case 2:
		_bodyTexture = _greenSkin;
		break;
	case 3:
		_bodyTexture = _redSkin;
		break;
	}
}