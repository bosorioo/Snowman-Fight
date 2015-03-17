#include "GuiPages.h"
#include "Drawing.h"
#include "Game.h"
#include "glutInclude.h"

#include <cmath>

#define PAGE_MARGIN 4.f
#define PAGE_TITLE_HEIGHT 24.f
#define PAGE_FONT GLUT_BITMAP_HELVETICA_12
#define PAGE_TITLE_SPACING 25.f

GuiPages::GuiPages(float width, float height)
{
	m_width = width;
	m_height = height;
	m_active_page = 0;
	m_x = m_y = 0.f;
}

bool GuiPages::isPointWithin(int x, int y)
{
	return x >= m_x - m_width / 2.f && x <= m_x + m_width / 2.f &&
		y >= m_y - m_height / 2.f && y <= m_y + m_height / 2.f;
}

void GuiPages::setActivePage(unsigned id)
{
	if (id >= m_pages.size())
		return;

	m_active_page = id;
}

void GuiPages::addPage(std::string name, GuiComponent* page)
{
	m_pages_name.push_back(name);
	m_pages_name_width.push_back(glutBitmapLength(PAGE_FONT, (unsigned char*)name.c_str()));
	m_pages.push_back(page);

	if (page)
		page->onResize(std::floor(m_width - 2 * PAGE_MARGIN) - 1.f, m_height - 2 * PAGE_MARGIN - PAGE_TITLE_HEIGHT - 1.f);
}

void GuiPages::setPosition(float x, float y)
{
	m_x = std::floor(x);
	m_y = std::floor(y);

	m_tx = m_x - m_width / 2.f + PAGE_MARGIN;
	m_ty = m_y - m_height / 2.f + PAGE_MARGIN;
}

void GuiPages::setSize(float width, float height)
{
	m_width = width;
	m_height = height;

	m_tx = m_x - m_width / 2.f + PAGE_MARGIN;
	m_ty = m_y - m_height / 2.f + PAGE_MARGIN;

	for (unsigned i = 0; i < m_pages.size(); i++)
	{
		if (m_pages[i])
			m_pages[i]->onResize(std::floor(m_width - 2 * PAGE_MARGIN) - 1.f, m_height - 2 * PAGE_MARGIN - PAGE_TITLE_HEIGHT - 1.f);
	}
}

GuiComponentAnswer GuiPages::onSpecialKeyDown(int key)
{
	if (m_pages.size() > m_active_page && m_pages[m_active_page])
		m_pages[m_active_page]->onSpecialKeyDown(key);

	return GUI_ANS_STOP;
}

GuiComponentAnswer GuiPages::onKeyDown(unsigned char key)
{
	if (m_pages.size() > m_active_page && m_pages[m_active_page])
		m_pages[m_active_page]->onKeyDown(key);

	return GUI_ANS_STOP;
}

GuiComponentAnswer GuiPages::onMouseClick(int button, int state, int x, int y)
{
	if (!isPointWithin(x, y))
		return GUI_ANS_PROPAGATE;

	float xBegin = m_x - m_width / 2.f + PAGE_TITLE_SPACING / 2.f;

	if (x >= xBegin && y >= m_y + m_height / 2.f - PAGE_TITLE_HEIGHT - PAGE_MARGIN)
	{
		for (unsigned i = 0; i < m_pages.size(); i++)
		{
			xBegin += m_pages_name_width[i] + PAGE_TITLE_SPACING / 2.f;
			if (x <= xBegin)
			{
				m_active_page = i;
				break;
			}
		}
		return GUI_ANS_STOP;
	}

	if (m_pages.size() > m_active_page && m_pages[m_active_page])
		m_pages[m_active_page]->onMouseClick(button, state, x - m_tx, y - m_ty);

	return GUI_ANS_STOP;
}

GuiComponentAnswer GuiPages::onMouseMove(int x, int y)
{
	if (!isPointWithin(x, y))
		return GUI_ANS_PROPAGATE;

	if (m_pages.size() > m_active_page && m_pages[m_active_page])
		m_pages[m_active_page]->onMouseMove(x - m_tx, y - m_ty);

	return GUI_ANS_STOP;
}

void GuiPages::onResize(int w, int h)
{

}

void GuiPages::onUpdate(float interval)
{
	for (unsigned i = 0; i < m_pages.size(); i++)
	{
		if (m_pages[i])
			m_pages[i]->onUpdate(interval);
	}
}

void GuiPages::onDraw()
{
	Game::setOrthoProjection();
	glDisable(GL_DEPTH_TEST);

	glColor3f(0.f, 0.f, 0.f);
	Draw::rectangle2d(m_x, m_y, m_width, m_height, true);

	glColor3f(1.f, 1.f, 1.f);

	std::vector<float> xPoints, yPoints;
	float x = m_x - m_width / 2.f + PAGE_TITLE_SPACING / 2.f;
	float nextX;

	for (unsigned i = 0; i < m_pages_name.size(); i++)
	{
		Draw::text(m_pages_name[i], 2 * PAGE_MARGIN + x + m_pages_name_width[i] / 2.f,
			m_y + m_height / 2.f - PAGE_TITLE_HEIGHT * .8f, false, true, PAGE_FONT);

		nextX = m_pages_name_width[i] + PAGE_TITLE_SPACING / 2.f;

		if (!i)
		{
			if (m_active_page == i)
			{
				xPoints.push_back(std::floor(x - PAGE_TITLE_SPACING / 2.f + PAGE_MARGIN));
				xPoints.push_back(x + nextX + PAGE_TITLE_SPACING / 2.f);
				yPoints.push_back(m_y + m_height / 2.f - PAGE_MARGIN);
				yPoints.push_back(m_y + m_height / 2.f - PAGE_MARGIN);
			}
			else
			{
				xPoints.push_back(x);
				yPoints.push_back(m_y + m_height / 2.f - PAGE_MARGIN - PAGE_TITLE_HEIGHT);
			}
		}
		else
		{
			xPoints.push_back(x);
			yPoints.push_back(m_y + m_height / 2.f - PAGE_MARGIN - PAGE_TITLE_HEIGHT);

			if (m_active_page == i)
			{
				xPoints.push_back(x);
				yPoints.push_back(m_y + m_height / 2.f - PAGE_MARGIN);
				xPoints.push_back(x + nextX + PAGE_TITLE_SPACING / 2.f);
				yPoints.push_back(m_y + m_height / 2.f - PAGE_MARGIN);
			}
		}

		if (i + 1 == m_pages.size() && m_active_page == i)
		{
			xPoints.push_back(x + nextX + PAGE_TITLE_SPACING / 2.f);
			yPoints.push_back(m_y + m_height / 2.f - PAGE_MARGIN - PAGE_TITLE_HEIGHT);
		}

		x += nextX + PAGE_TITLE_SPACING / 2.f;
	}

	if (!xPoints.empty())
	{

		glBegin(GL_LINE_STRIP);

		for (unsigned i = 0; i < xPoints.size(); i++)
			glVertex3f(xPoints[i], yPoints[i], 0.f);

		glVertex3f(std::floor(m_x + m_width / 2.f - PAGE_MARGIN), m_y + m_height / 2.f - PAGE_MARGIN - PAGE_TITLE_HEIGHT, 0.f);
		glVertex3f(std::floor(m_x + m_width / 2.f - PAGE_MARGIN), m_y - m_height / 2.f + PAGE_MARGIN, 0.f);
		glVertex3f(std::floor(m_x - m_width / 2.f + PAGE_MARGIN), m_y - m_height / 2.f + PAGE_MARGIN, 0.f);
		glVertex3f(std::floor(m_x - m_width / 2.f + PAGE_MARGIN), m_y + m_height / 2.f - PAGE_MARGIN - PAGE_TITLE_HEIGHT, 0.f);
		glVertex3f(xPoints[0], yPoints[0], 0.f);
		glEnd();
	}

	Draw::scissor(m_x - 1.f, m_y - PAGE_TITLE_HEIGHT / 2.f,
		std::floor(m_width - 2 * PAGE_MARGIN) - 1.f, m_height - 2 * PAGE_MARGIN - PAGE_TITLE_HEIGHT - 1.f, true);

	glTranslatef(m_tx, m_ty, 0.f);

	if (m_active_page < m_pages.size() && m_pages[m_active_page])
		m_pages[m_active_page]->onDraw();

	glTranslatef(-m_tx, -m_ty, 0.f);

	Draw::scissor(0.f, 0.f, 0.f, 0.f);

	glEnable(GL_DEPTH_TEST);
}
