#include "SphericalTexture.h"
#include "Texture.h"
#include "Utils.h"
#include <cmath>

SphericalTexture::SphericalTexture(int quadPoints, float radius)
{
	m_QuadPoints = quadPoints;
	m_Radius = radius;
	m_TextureId = 0;
	calculate();
}

void SphericalTexture::setTexture(std::string& path)
{
	Texture tex;
	Texture::getTexture(path, tex);
	m_TextureId = tex.getId();
}

void SphericalTexture::draw()
{
	glColor3f(1.f, 1.f, 1.f);
	Texture::bindId(m_TextureId);

	for (int x = 0; x < m_QuadPoints * 2; x++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int y = 0; y <= m_QuadPoints * 2; y++)
		{
			m_Vertexes[x * (m_QuadPoints * 2 + 1) + y].vertex();
			m_Vertexes[(x + 1) * (m_QuadPoints * 2 + 1) + y].vertex();
		}
		glEnd();
	}
	Texture::bindId();
}

void SphericalTexture::calculate()
{
	m_Vertexes.clear();
	m_Vertexes.resize((m_QuadPoints * m_QuadPoints + m_QuadPoints) * 4 + 1);

	int x = 0, y = 0;
	float px, py;

	for (unsigned i = 0; i < m_Vertexes.size(); i++)
	{
		px = 1.3f * (float)(x - m_QuadPoints) / m_QuadPoints;
		py = 1.1f * (float)(y - m_QuadPoints) / m_QuadPoints;
		
		m_Vertexes[i].setXYZ(std::sin(px) * std::cos(py) * m_Radius, -std::sin(py) * m_Radius, std::cos(px) * std::cos(py) * m_Radius);
		m_Vertexes[i].setUV((float)x / (m_QuadPoints * 2), 1.f - (float)y / (m_QuadPoints * 2));

		x++;

		if (x > (int)m_QuadPoints * 2)
		{
			x = 0;
			y++;
		}
	}
}