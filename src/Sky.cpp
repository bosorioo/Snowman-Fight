#include "Sky.h"
#include "Game.h"
#include "Texture.h"
#include "Drawing.h"
#include "glutInclude.h"

#define SKY_PLANE_DISTANCE 32.5f
#define SKY_GROUND_HEIGHT 0.f
#define SKY_HEIGHT 16.f
#define SKY_GLUE_DISTANCE 0.05f

Sky::Sky()
{
	m_x = m_z = 0;
	m_textureId = 0;
}

Sky::~Sky()
{
}

void Sky::setPosition(float x, float z)
{
	m_x = x;
	m_z = z;
}

void Sky::setTexture(std::string path)
{
	Texture tex;
	Texture::getTexture(path, tex);
	m_textureId = tex.getId();
}

void Sky::draw()
{
	glColor3f(72.f/255.f, 79.f/255.f, 125.f/255.f);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f, 1.0f, 1.0f);

	Texture::bindId(m_textureId);
	glTranslatef(m_x, 0.f, m_z);
	glBegin(GL_QUADS);

	glTexCoord2f(0.f, 0.f); glVertex3f(-SKY_PLANE_DISTANCE, SKY_GROUND_HEIGHT, SKY_PLANE_DISTANCE - SKY_GLUE_DISTANCE);
	glTexCoord2f(1.f, 0.f); glVertex3f(SKY_PLANE_DISTANCE, SKY_GROUND_HEIGHT, SKY_PLANE_DISTANCE - SKY_GLUE_DISTANCE);
	glTexCoord2f(1.f, 1.f); glVertex3f(SKY_PLANE_DISTANCE, SKY_HEIGHT, SKY_PLANE_DISTANCE - SKY_GLUE_DISTANCE);
	glTexCoord2f(0.f, 1.f); glVertex3f(-SKY_PLANE_DISTANCE, SKY_HEIGHT, SKY_PLANE_DISTANCE - SKY_GLUE_DISTANCE);

	glTexCoord2f(1.f, 0.f); glVertex3f(-SKY_PLANE_DISTANCE + SKY_GLUE_DISTANCE, SKY_GROUND_HEIGHT, -SKY_PLANE_DISTANCE);
	glTexCoord2f(0.f, 0.f); glVertex3f(-SKY_PLANE_DISTANCE + SKY_GLUE_DISTANCE, SKY_GROUND_HEIGHT, SKY_PLANE_DISTANCE);
	glTexCoord2f(0.f, 1.f); glVertex3f(-SKY_PLANE_DISTANCE + SKY_GLUE_DISTANCE, SKY_HEIGHT, SKY_PLANE_DISTANCE);
	glTexCoord2f(1.f, 1.f); glVertex3f(-SKY_PLANE_DISTANCE + SKY_GLUE_DISTANCE, SKY_HEIGHT, -SKY_PLANE_DISTANCE);

	glTexCoord2f(1.f, 0.f); glVertex3f(-SKY_PLANE_DISTANCE, SKY_GROUND_HEIGHT, -SKY_PLANE_DISTANCE + SKY_GLUE_DISTANCE);
	glTexCoord2f(1.f, 1.f); glVertex3f(-SKY_PLANE_DISTANCE, SKY_HEIGHT, -SKY_PLANE_DISTANCE + SKY_GLUE_DISTANCE);
	glTexCoord2f(0.f, 1.f); glVertex3f(SKY_PLANE_DISTANCE, SKY_HEIGHT, -SKY_PLANE_DISTANCE + SKY_GLUE_DISTANCE);
	glTexCoord2f(0.f, 0.f); glVertex3f(SKY_PLANE_DISTANCE, SKY_GROUND_HEIGHT, -SKY_PLANE_DISTANCE + SKY_GLUE_DISTANCE);

	glTexCoord2f(0.f, 0.f); glVertex3f(SKY_PLANE_DISTANCE - SKY_GLUE_DISTANCE, SKY_GROUND_HEIGHT, -SKY_PLANE_DISTANCE);
	glTexCoord2f(0.f, 1.f); glVertex3f(SKY_PLANE_DISTANCE - SKY_GLUE_DISTANCE, SKY_HEIGHT, -SKY_PLANE_DISTANCE);
	glTexCoord2f(1.f, 1.f); glVertex3f(SKY_PLANE_DISTANCE - SKY_GLUE_DISTANCE, SKY_HEIGHT, SKY_PLANE_DISTANCE);
	glTexCoord2f(1.f, 0.f); glVertex3f(SKY_PLANE_DISTANCE - SKY_GLUE_DISTANCE, SKY_GROUND_HEIGHT, SKY_PLANE_DISTANCE);

	glEnd();
	glTranslatef(-m_x, 0.f, -m_z);
	Texture::bindId();
	glEnable(GL_DEPTH_TEST);
}

