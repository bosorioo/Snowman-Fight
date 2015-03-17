#ifndef _SKY_H
#define _SKY_H

#include <string>

class Sky
{
public:

	Sky();
	~Sky();

	void setPosition(float x, float z);
	void setTexture(std::string path);
	void draw();

private:

	float m_x, m_z;
	int m_textureId;
};


#endif