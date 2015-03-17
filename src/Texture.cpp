#include "Texture.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include "glutInclude.h"

Texture::TextureMap Texture::static_textures;

bool Texture::getTexture(std::string path, Texture & tex)
{
	TextureMap::iterator it;

	if (!static_textures.empty())
	{
		it = static_textures.find(path);
		if (it != static_textures.end())
		{
			tex = it->second;
			return true;
		}
	}
	
	if (path.substr(std::max(0, (int)path.size() - 4), 4) == ".bmp")
	{
		loadBMPTexture(path, static_textures[path]);
		tex = static_textures[path];
		return true;
	}

	return false;
}

// Taken from example "sphereMapping.cpp"
void Texture::loadBMPTexture(std::string path, Texture & tex)
{
	unsigned width = 0, height = 0;
	unsigned char* data;
	unsigned int size, offset, headerSize;
  
	tex.m_textureId = 0;

	// Read input file name.
	std::ifstream infile(path.c_str(), std::ios::binary);
 
	if (!infile.is_open())
	{
		std::cout << "Error loading file from path " << path << ". Texture couldn't be loaded.\n";
		return;
	}

	// Get the starting point of the image data.
	infile.seekg(10);
	infile.read((char *) &offset, 4); 
   
	// Get the header size of the bitmap.
	infile.read((char *) &headerSize,4);

	// Get width and height values in the bitmap header.
	infile.seekg(18);
	infile.read( (char *) &width, 4);
	infile.read( (char *) &height, 4);

	// Allocate buffer for the image.
	size = width * height * 3;
	try {
		data = new unsigned char[size];
	} catch (std::exception & except) {
		std::cout << "Exception thrown on attempt to allocate memory to load bmp texture from path " << path << ": " << except.what() << '\n';
		return;
	}

	// Read bitmap data.
	infile.seekg(offset);
	infile.read((char *) data , size);
   
	// Reverse color from bgr to rgb.
	int temp;
	for (unsigned i = 0; i < size; i += 3)
	{ 
		temp = data[i];
		data[i] = data[i+2];
		data[i+2] = temp;
	}

	tex.m_height = height;
	tex.m_width = width;

	glGenTextures(1, &tex.m_textureId);
	glBindTexture(GL_TEXTURE_2D, tex.m_textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.m_width, tex.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	delete [] data;
}

void Texture::loadSGFXTextures(std::string path)
{
	FILE* file = fopen(path.c_str(), "rb");

	if (!file)
		return;

	unsigned char byte[3];
	unsigned char* data;
	unsigned textureCount, width, height;

	fscanf(file, "%c%c", byte, byte + 1);
	textureCount = byte[0] * 256 + byte[1];

	for (unsigned int textureIndex = 0; textureIndex < textureCount; textureIndex++)
	{
		fscanf(file, "%c%c", byte, byte + 1);
		width = byte[0] * 256 + byte[1];

		fscanf(file, "%c%c", byte, byte + 1);
		height = byte[0] * 256 + byte[1];

		fscanf(file, "%c", byte);

		data = new unsigned char [std::max(width * height * 3, (unsigned)byte[0])];

		for (unsigned char textureNameCharIndex = 0; textureNameCharIndex < byte[0]; textureNameCharIndex++)
			fscanf(file, "%c", &data[textureNameCharIndex]);

		data[byte[0]] = '\0';

		Texture& tex = static_textures[path + "@" + (char*)data];
		tex.m_height = height;
		tex.m_width = width;

		for (unsigned rgbIndex = tex.m_width * tex.m_height; rgbIndex > 0; rgbIndex--)
		{
			fscanf(file, "%c%c%c",
				&data[rgbIndex * 3], 
				&data[rgbIndex * 3 + 1],
				&data[rgbIndex * 3 + 2]);
		}

		glGenTextures(1, &tex.m_textureId);
		glBindTexture(GL_TEXTURE_2D, tex.m_textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex.m_width, tex.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		delete [] data;
	}
}

void Texture::bindId(unsigned id)
{
	if (id)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, id);
	}
	else
		glDisable(GL_TEXTURE_2D);
}

Texture::Texture()
{
	m_textureId = 0;
}

void Texture::bind()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_textureId);
}

void Texture::unbind()
{
	glDisable(GL_TEXTURE_2D);
}

unsigned Texture::getId()
{
	return m_textureId;
}