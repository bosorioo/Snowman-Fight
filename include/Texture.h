#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <map>
#include <string>

class Texture
{
public:

	static bool getTexture(std::string path, Texture & tex);
	static void loadBMPTexture(std::string path, Texture & tex);
	static void loadSGFXTextures(std::string path);
	static void bindId(unsigned id = 0);
	
	Texture();

	void bind();
	void unbind();
	unsigned getId();

private:

	typedef std::map<std::string, Texture> TextureMap;
	typedef std::pair<std::string, Texture> TexturePair;
	static TextureMap static_textures;

	unsigned int m_textureId, m_width, m_height;
	bool m_loaded;

};


#endif