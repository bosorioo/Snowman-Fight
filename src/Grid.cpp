#include "Grid.h"
#include "Texture.h"
#include "glutInclude.h"

#define GRID_QUAD_TEX_SIZE 0.1f

Grid::Grid(float width, float height, float quadsize)
{
	m_width = width;
	m_height = height;
	m_textureId = 0;
	m_quadsize = quadsize;
	m_r = m_g = m_b = m_a = 1.f;
	m_glList = 0;
}

void Grid::setTexture(std::string path)
{
	Texture temp;
	Texture::getTexture(path, temp);
	m_textureId = temp.getId();
}

void Grid::setColor(float r, float g, float b, float a)
{
	m_r = r;
	m_g = g;
	m_b = b;
	m_a = a;
}

void Grid::draw()
{
	glColor4f(m_r, m_g, m_b, m_a);

	if (m_textureId > 0)
			Texture::bindId(m_textureId);

	if (m_glList)
	{
		glCallList(m_glList);

		if (m_textureId > 0)
			Texture::bindId();
		return;
	}

	int newList = glGenLists(1);
	
	if (newList)
		glNewList(newList, GL_COMPILE_AND_EXECUTE);

	int tilesX = m_width / m_quadsize;
	int tilesY = m_height / m_quadsize;

	float extraX = m_width - tilesX * m_quadsize;
	float extraY = m_height - tilesY * m_quadsize;

	int x, y;
	float px, py;

	glTranslatef(-m_width / 2.f, 0.f, -m_height / 2.f);
	glNormal3f(0.f, 0.f, -1.f);

	for (x = 0; x <= tilesX; x++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (y = 0; y <= tilesY + 1; y++)
		{
			if (x >= tilesX)
			{
				if (extraX <= 0.f)
					continue;

				px = x + extraX;
			}
			else
				px = x + 1;

			if (y > tilesY)
			{
				if (extraY <= 0.f)
					continue;

				py = y - 1 + extraY;
			}
			else
				py = y;

			glTexCoord2f(x * GRID_QUAD_TEX_SIZE, py * GRID_QUAD_TEX_SIZE);
			glVertex3f(x * m_quadsize, 0.f, py * m_quadsize);

			glTexCoord2f(px * GRID_QUAD_TEX_SIZE, py * GRID_QUAD_TEX_SIZE);
			glVertex3f(px * m_quadsize, 0.f, py * m_quadsize);
		}

		glEnd();
	}

	glTranslatef(m_width / 2.f, 0.f, m_height / 2.f);

	if (newList)
	{
		glEndList();
		m_glList = newList;
	}

	if (m_textureId > 0)
			Texture::bindId();
}