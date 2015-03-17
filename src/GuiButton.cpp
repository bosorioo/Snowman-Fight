#include "GuiButton.h"
#include "Game.h"
#include "Snowman.h"
#include <iostream>

GuiButton::GuiButton(char* label, int x, int y, void* font) :
	m_isMouseHovering(false), m_clicked(false)
{
	m_font = font;
	setLabel(label);
	setPosition(x, y);	
}

void GuiButton::setLabel(char* label)
{
	if (label == 0)
		m_label.clear();
	else
		m_label = std::string(label).substr(0, GUI_BUTTON_MAX_LABEL_LENGTH - 1);

	calculateDimensions();
}

void GuiButton::setPosition(int x, int y)
{
	m_x = (float)x;
	m_y = (float)y;
}

void GuiButton::getPosition(float & x, float & y)
{
	x = m_x;
	y = m_y;
}

void GuiButton::move(int x, int y)
{
	m_x += (float)x;
	m_y += (float)y;
}

void GuiButton::getSize(float & width, float & height)
{
	width = (m_label_w + GUI_BUTTON_HORIZONTAL_MARGIN) * 2.f;
	height = (m_label_h + GUI_BUTTON_VERTICAL_MARGIN) * 2.f;
}

void GuiButton::resetState()
{
	m_isMouseHovering = false;
	m_clicked = false;
}

bool GuiButton::isClicked()
{
	bool wasClicked = m_clicked;
	m_clicked = false;
	return wasClicked;
}

GuiComponentAnswer GuiButton::onMouseClick(int button, int state, int x, int y)
{
	if (!isPointWithin(x, y))
		return GUI_ANS_PROPAGATE;

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		m_clicked = true;

	return GUI_ANS_STOP;
}

GuiComponentAnswer GuiButton::onMouseMove(int x, int y)
{
	if (!isPointWithin(x, y))
	{
		m_isMouseHovering = false;
		return GUI_ANS_PROPAGATE;
	}

	m_isMouseHovering = true;
	return GUI_ANS_STOP;
}


void GuiButton::onDraw()
{
	glDisable(GL_DEPTH_TEST);

	if (m_isMouseHovering)
		glColor4f(0.75f, 0.75f, 0.95f, 0.65f);
	else
		glColor4f(0.f, 0.f, 0.f, 0.65f);

	glBegin(GL_QUADS);
	glVertex3f(m_x - m_label_w - GUI_BUTTON_HORIZONTAL_MARGIN, m_y - m_label_h - GUI_BUTTON_VERTICAL_MARGIN, 0.f);
	glVertex3f(m_x + m_label_w + GUI_BUTTON_HORIZONTAL_MARGIN, m_y - m_label_h - GUI_BUTTON_VERTICAL_MARGIN, 0.f);
	glVertex3f(m_x + m_label_w + GUI_BUTTON_HORIZONTAL_MARGIN, m_y + m_label_h + GUI_BUTTON_VERTICAL_MARGIN, 0.f);
	glVertex3f(m_x - m_label_w - GUI_BUTTON_HORIZONTAL_MARGIN, m_y + m_label_h + GUI_BUTTON_VERTICAL_MARGIN, 0.f);
	glEnd();

	glColor3f(0.2f, 0.2f, 0.2f);
	glBegin(GL_LINE_STRIP);
	glVertex3f(m_x - m_label_w - GUI_BUTTON_HORIZONTAL_MARGIN, m_y - m_label_h - GUI_BUTTON_VERTICAL_MARGIN, 0.f);
	glVertex3f(m_x + m_label_w + GUI_BUTTON_HORIZONTAL_MARGIN, m_y - m_label_h - GUI_BUTTON_VERTICAL_MARGIN, 0.f);
	glVertex3f(m_x + m_label_w + GUI_BUTTON_HORIZONTAL_MARGIN, m_y + m_label_h + GUI_BUTTON_VERTICAL_MARGIN, 0.f);
	glVertex3f(m_x - m_label_w - GUI_BUTTON_HORIZONTAL_MARGIN, m_y + m_label_h + GUI_BUTTON_VERTICAL_MARGIN, 0.f);
	glVertex3f(m_x - m_label_w - GUI_BUTTON_HORIZONTAL_MARGIN, m_y - m_label_h - GUI_BUTTON_VERTICAL_MARGIN, 0.f);
	glEnd();

	glColor3f(1.f, 1.f, 1.f);
	glRasterPos3f(m_x - m_label_w + GUI_BUTTON_FONT_CORRECTION_X, m_y - m_label_h + GUI_BUTTON_FONT_CORRECTION_Y, 0.f);

	for (std::string::iterator it = m_label.begin(); it != m_label.end(); it++)
		glutBitmapCharacter(m_font, *it);

	glEnable(GL_DEPTH_TEST);
}


bool GuiButton::isPointWithin(int x, int y)
{
	return x >= m_x - m_label_w - GUI_BUTTON_HORIZONTAL_MARGIN && x <= m_x + m_label_w + GUI_BUTTON_HORIZONTAL_MARGIN &&
		y >= m_y - m_label_h - GUI_BUTTON_VERTICAL_MARGIN && y <= m_y + m_label_h + GUI_BUTTON_VERTICAL_MARGIN;
}

void GuiButton::calculateDimensions()
{
	m_label_w = glutBitmapLength(m_font, (unsigned char*)m_label.c_str()) / 2.f;

	if (m_font == GLUT_BITMAP_8_BY_13)
		m_label_h = 5.f;
	else
		m_label_h = GUI_BUTTON_CHAR_HEIGHT / 2.f;
}