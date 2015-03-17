#include "SnowRain.h"
#include "Utils.h"
#include "glutInclude.h"

SnowRain::SnowRain(float x, float y, float z, float width, float height, unsigned int particlesCount) :
	m_particles(particlesCount, Snow()), m_x(x), m_y(y), m_z(z), m_w(width), m_h(height)
{
	m_scaling = 1.f;
	m_trans_x = m_trans_z = 0.f;
}

void SnowRain::setScaling(float s)
{
	m_scaling = s;
}

void SnowRain::setPosition(float x, float z)
{
	m_x = x;
	m_z = z;
}

void SnowRain::setTranslation(float x, float z)
{
	m_trans_x = x;
	m_trans_z = z;
}

void SnowRain::update(float interval)
{
	float x, y, z;
	int index = 0;

	for (std::vector<Snow>::iterator it = m_particles.begin(); it != m_particles.end(); it++, index++)
	{
		it->update(interval);
		it->getPosition(x, y, z);

		if (y <= 0.f)
		{
			randomize(index);
			it->getPosition(x, y, z);
			it->setPosition(x, m_y, z);
		}

		if (!it->isRotating())
			it->setGoalRotation(Utils::getRandomFloat(0.f, 360.f), Utils::getRandomFloat(0.f, 360.f), Utils::getRandomFloat(0.f, 360.f));
	}
}

void SnowRain::draw()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glDisable(GL_BLEND);
	glTranslatef(m_trans_x, 0.f, m_trans_z);

	for (std::vector<Snow>::iterator it = m_particles.begin(); it != m_particles.end(); it++)
	{
		it->draw();
	}

	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glTranslatef(-m_trans_x, 0.f, -m_trans_z);
}

void SnowRain::randomize(int index)
{
	if (index == -1)
	{
		for (unsigned i = 0; i < m_particles.size(); i++)
			randomize(i);

		return;
	}

	float color = 1.f;//Utils::getRandomFloat(0.75f, 1.f);
	float scale = 1.5f;

	m_particles[index].setModel(0);
	m_particles[index].setColor(color, color, color, 0.8f);
	//m_particles[index].setGoalRotation(Utils::getRandomFloat(0.f, 360.f), Utils::getRandomFloat(0.f, 360.f), Utils::getRandomFloat(0.f, 360.f));
	//m_particles[index].setRotation(Utils::getRandomFloat(0.f, 360.f), Utils::getRandomFloat(0.f, 360.f), Utils::getRandomFloat(0.f, 360.f));
	//m_particles[index].setRotationSpeed(0.f);

	float x = m_x + Utils::getRandomFloat(-m_w / 2.f, m_w / 2.f), z = m_z + Utils::getRandomFloat(-m_h / 2.f, m_h / 2.f);

	m_particles[index].setPosition(x, Utils::getRandomFloat(0.f, m_y), z);
	m_particles[index].setScaling(scale * m_scaling, scale * m_scaling, scale * m_scaling); 
}