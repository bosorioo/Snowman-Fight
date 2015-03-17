#ifndef _TEXTURE_LOADER_H
#define _TEXTURE_LOADER_H

#include "Texture.h"

namespace TextureLoader
{
	void LoadTextures()
	{
		Texture temp;
		Texture::getTexture("SnowTextures/face1.bmp", temp);
		Texture::getTexture("SnowTextures/face2.bmp", temp);
		Texture::getTexture("SnowTextures/face3.bmp", temp);
		Texture::getTexture("SnowTextures/face4.bmp", temp);
		Texture::getTexture("SnowTextures/face5.bmp", temp);
		Texture::getTexture("SnowTextures/snow.bmp", temp);
	}

};

#endif