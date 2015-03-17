#include "Snow.h"
#include "Game.h"
#include "Texture.h"
#include "glutInclude.h"
#include <GL/glext.h>
#include "Utils.h"
#include <iostream>

#define SNOW_MODEL_MIN 0
#define SNOW_MODEL_MAX 1

bool Snow::sSnowFramebufferCreated = false;
bool Snow::sHasFramebuffer = false;
GLuint Snow::sSnowFramebuffer = 0;
GLuint Snow::sSnowTexture = 0;

Snow::Snow()
{
	m_snowModel = 0;
	m_r = m_g = m_b = m_a = 1.f;
	m_x = m_y = m_z = 0.f;
	m_rotation_speed = 1.f;
	m_sx = m_sy = m_sz = 1.f;
	m_rx = m_ry = m_rz = 0.f;
}

bool Snow::isRotating()
{
	return m_rx != m_grx || m_ry != m_gry || m_rz != m_grz;
}

void Snow::getPosition(float & x, float & y, float & z)
{
	x = m_x;
	y = m_y;
	z = m_z;
}

void Snow::setScaling(float x, float y, float z)
{
	m_sx = x;
	m_sy = y;
	m_sz = z;
}

void Snow::setRotationSpeed(float s)
{
	m_rotation_speed = s;
}

void Snow::setGoalRotation(float x, float y, float z)
{
	m_grx = x;
	m_gry = y;
	m_grz = z;
}

void Snow::setRotation(float x, float y, float z)
{
	m_rx = x;
	m_ry = y;
	m_rz = z;
}

void Snow::move(float x, float y, float z)
{
	m_x += x;
	m_y += y;
	m_z += z;
}

void Snow::setPosition(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}

void Snow::setColor(float r, float g, float b, float a)
{
	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;
}

void Snow::setModel(int m)
{
	if (m == -1)
		m_snowModel = Utils::getRandomInt(SNOW_MODEL_MIN, SNOW_MODEL_MAX);
	else
		m_snowModel = Utils::limit(SNOW_MODEL_MIN, m, SNOW_MODEL_MAX);
}

void Snow::update(float interval)
{
	if (m_rotation_speed > 0.f)
	{
		float rotationDelta = m_rotation_speed * interval;

		if (m_grx > m_rx)
			m_rx = m_rx + rotationDelta > m_grx ? m_grx : m_rx + rotationDelta;
		else if (m_grx < m_rx)
			m_rx = m_rx - rotationDelta < m_grx ? m_grx : m_rx - rotationDelta;

		if (m_gry > m_ry)
			m_ry = m_ry + rotationDelta > m_gry ? m_gry : m_ry + rotationDelta;
		else if (m_gry < m_ry)
			m_ry = m_ry - rotationDelta < m_gry ? m_gry : m_ry - rotationDelta;

		if (m_grz > m_rz)
			m_rz = m_rz + rotationDelta > m_grz ? m_grz : m_rz + rotationDelta;
		else if (m_grz < m_rz)
			m_rz = m_rz - rotationDelta < m_grz ? m_grz : m_rz - rotationDelta;
	}

	m_y -= (m_sx + m_sy + m_sz) / 20.f * interval;
}

void Snow::draw()
{
	glColor4f(m_r, m_g, m_b, m_a);
	glPushMatrix();

	glTranslatef(m_x, m_y, m_z);
	glScalef(m_sx, m_sy, m_sz);
	
	if (m_snowModel == 0)
	{
		static int glListId = 0;

		if (!glListId)
		{
			glListId = glGenLists(1);

			if (!glListId)
				return;

			glNewList(glListId, GL_COMPILE_AND_EXECUTE);
			glutSolidSphere(0.0045f, 5, 5);
			glEndList();

		}
		else
			glCallList(glListId);
	}
	else
	{
		glRotatef(m_rx, 1.f, 0.f, 0.f);
		glRotatef(m_ry, 0.f, 1.f, 0.f);
		glRotatef(m_rz, 0.f, 0.f, 1.f);
		drawKochSnow();
	}

	glPopMatrix();
}

void Snow::drawKochSnow()
{
	static int glListId = -1;

	if (glListId > 0)
	{
		glCallList(glListId);
		return;
	}

	glListId = glGenLists(1);

	if (!glListId)
		return;

	glNewList(glListId, GL_COMPILE_AND_EXECUTE);

	glClearStencil(0);
	glClear(GL_STENCIL_BUFFER_BIT);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);
	glStencilFunc(GL_NEVER, 1, 1);
	glStencilOp(GL_INVERT, GL_INVERT, GL_INVERT);

	glBegin(GL_POLYGON);
		drawKochSnowFractalSegment(0.f, 1.155f, -1.f, -0.577f, 0, 3);
		drawKochSnowFractalSegment(-1.f, -0.577f, 1.f, -0.577f, 0, 3);
		drawKochSnowFractalSegment(1.f, -0.577f, 0.f, 1.155f, 0, 3);
	glEnd();

	glStencilFunc(GL_EQUAL, 1, 1);
	
	glBegin(GL_QUADS);
	glVertex3f(-2.f, -2.f, 0.f); glVertex3f(-2.f, 2.f, 0.f);
	glVertex3f(2.f, 2.f, 0.f); glVertex3f(2.f, -2.f, 0.f);
	glEnd();
		
	glDisable(GL_STENCIL_TEST);

	glEndList();
}

void Snow::drawKochSnowFractalSegment(float x1, float y1, float x2, float y2, int level, int maxlevel, bool ignorelast)
{
	if (maxlevel == 0)
		glVertex3f(x1, y1, 0.f);

	if (level >= maxlevel)
	{
		glVertex3f(x2, y2, 0.f);
		return;
	}

	float dX = x2 - x1, dY = y2 - y1;
	float dT = Utils::pointAngle(x1, y1, x2, y2) - PI / 2.f;
	float dL = std::sqrt(dX * dX + dY * dY) / 2.f;
	float cos = std::cos(dT), sin = std::sin(dT);
	float dR = 0.5f;

	drawKochSnowFractalSegment(x1, y1, x1 + dX / 3.f, y1 + dY / 3.f, level, maxlevel - 1, true);
	drawKochSnowFractalSegment(x1 + dX / 3.f, y1 + dY / 3.f, (x1 + dX / 2.f) + dL * dR * cos, (y1 + dY / 2.f) + dL * dR * sin, level + 1, maxlevel, true);
	drawKochSnowFractalSegment((x1 + dX / 2.f) + dL * dR * cos, (y1 + dY / 2.f) + dL * dR * sin, x1 + dX / 3.f * 2.f, y1 + dY / 3.f * 2.f, level + 1, maxlevel, true);
	drawKochSnowFractalSegment(x1 + dX / 3.f * 2.f, y1 + dY / 3.f * 2.f, x2, y2, level, maxlevel - 1);

	if (!ignorelast && maxlevel == 0)
		glVertex3f(x2, y2, 0.f);
}