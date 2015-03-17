#include "Ground.h"
#include "Texture.h"
#include "glutInclude.h"

#define GROUND_QUAD_SIZE .5f
#define GROUND_QUAD_TEX_SIZE 0.1f

Ground::Ground(float width, float height)
{
	m_width = width;
	m_height = height;
	m_textureId = 0;
}

void Ground::setTexture(std::string path)
{
	Texture temp;
	Texture::getTexture(path, temp);
	m_textureId = temp.getId();
}

void Ground::draw()
{
	static int listId = 0;

	if (listId)
	{
		glCallList(listId);
		return;
	}

	int newList = glGenLists(1);
	
	if (newList)
		glNewList(newList, GL_COMPILE_AND_EXECUTE);

	glColor3f(1.f, 1.f, 1.f);
	Texture::bindId(m_textureId);

	int tilesX = m_width / GROUND_QUAD_SIZE;
	int tilesY = m_height / GROUND_QUAD_SIZE;

	float extraX = m_width - tilesX * GROUND_QUAD_SIZE;
	float extraY = m_height - tilesY * GROUND_QUAD_SIZE;

	int x, y;
	
	glTranslatef(-m_width / 2.f, 0.f, -m_height / 2.f);
	glNormal3f(0.f, 0.f, -1.f);

	for (x = 0; x <= tilesX; x++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (y = 0; y <= tilesY; y++)
		{

			if (x < tilesX)
			{
				glTexCoord2f(x * GROUND_QUAD_TEX_SIZE, y * GROUND_QUAD_TEX_SIZE);
				glVertex3f(x * GROUND_QUAD_SIZE, 0.f, y * GROUND_QUAD_SIZE);
			}
			else
			{
				glTexCoord2f((tilesX - 1 + extraX) * GROUND_QUAD_TEX_SIZE, y * GROUND_QUAD_TEX_SIZE);
				glVertex3f((tilesX - 1 + extraX) * GROUND_QUAD_SIZE, 0.f, y * GROUND_QUAD_SIZE);
			}

			if  (y < tilesY)
			{
				glTexCoord2f((x + 1) * GROUND_QUAD_TEX_SIZE, y * GROUND_QUAD_TEX_SIZE);
				glVertex3f((x + 1) * GROUND_QUAD_SIZE, 0.f, y * GROUND_QUAD_SIZE);
			}
			else
			{
				glTexCoord2f((x + 1) * GROUND_QUAD_TEX_SIZE, (tilesY - 1 + extraY) * GROUND_QUAD_TEX_SIZE);
				glVertex3f((x + 1) * GROUND_QUAD_SIZE, 0.f, (tilesY - 1 + extraY) * GROUND_QUAD_SIZE);
			}
		}

		glEnd();
	}

	Texture::bindId();
	glTranslatef(m_width / 2.f, 0.f, m_height / 2.f);

	if (newList)
	{
		glEndList();
		listId = newList;
	}
}