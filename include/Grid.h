#ifndef _GROUND_H
#define _GROUND_H

#include <string>

#define GRID_DEFAULT_WIDTH 150.f
#define GRID_DEFAULT_HEIGHT 150.f
#define GRID_DEFAULT_QUAD_SIZE 1.f

class Grid
{
public:

	Grid(float width, float height, float quadsize = GRID_DEFAULT_QUAD_SIZE);

	void setTexture(std::string path);
	void setColor(float r, float g, float b, float a = 1.f);
	void draw();

private:

	int m_textureId;
	int m_glList;
	float m_width, m_height, m_quadsize;
	float m_r, m_g, m_b, m_a;

};

#endif