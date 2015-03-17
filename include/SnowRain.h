#ifndef _SNOWRAIN_H
#define _SNOWRAIN_H

#include "Snow.h"
#include <vector>

class SnowRain
{
public:

	SnowRain(float x, float y, float z, float width, float height, unsigned particlesCount = 250);

	void setScaling(float s);
	void setPosition(float x, float z);
	void setTranslation(float x, float z);
	void update(float interval);
	void draw();

	void randomize(int index = -1);

private:

	std::vector <Snow> m_particles;

	float m_x, m_y, m_z, m_w, m_h, m_scaling;
	float m_trans_x, m_trans_z;

};


#endif