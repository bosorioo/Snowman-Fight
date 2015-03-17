#ifndef _GROUND_H
#define _GROUND_H

#include <string>

#define GROUND_WIDTH 150.f
#define GROUND_HEIGHT 150.f

class Ground
{
public:

	Ground(float width = GROUND_WIDTH, float height = GROUND_HEIGHT);

	void setTexture(std::string path);
	void draw();

private:

	int m_textureId;
	float m_width, m_height;

};

#endif