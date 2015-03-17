#include "GuiButtonList.h"
#include "Game.h"
#include "glutInclude.h"

bool GuiButtonList::isPointWithin(int x, int y)
{
	return x >= m_x - m_w && x <= m_x + m_w && y >= m_y - m_h  && y <= m_y + m_h;
}

GuiButton& GuiButtonList::addButton(std::string label)
{
	m_buttons.push_back(GuiButton((char*)label.c_str()));
	m_needsUpdate = true;
	return m_buttons.back();
}

GuiButton& GuiButtonList::operator[](unsigned index)
{
	static GuiButton dummyButton;

	if (index < m_buttons.size())
		return m_buttons[index];

	return dummyButton;
}

void GuiButtonList::setPosition(float x, float y)
{
	m_x = x;
	m_y = y;
	m_relative_x = (float)x / m_parent_w;
	m_relative_y = (float)y / m_parent_h;
	m_needsUpdate = true;
}

void GuiButtonList::setRelativePosition(float x, float y)
{
	m_relative_x = x;
	m_relative_y = y;
	m_x = m_parent_w * x;
	m_y = m_parent_h * y;
	m_needsUpdate = true;
}

void GuiButtonList::resetButtonsState()
{
	std::vector<GuiButton>::iterator it;
	for (it = m_buttons.begin(); it != m_buttons.end(); it++)
		it->resetState();
}

GuiComponentAnswer GuiButtonList::onMouseClick(int button, int state, int x, int y)
{
	if (!isPointWithin(x, y))
		return GUI_ANS_PROPAGATE;

	std::vector<GuiButton>::iterator it;
	for (it = m_buttons.begin(); it != m_buttons.end(); it++)
		it->onMouseClick(button, state, x, y);

	return GUI_ANS_STOP;
}

GuiComponentAnswer GuiButtonList::onMouseMove(int x, int y)
{
	std::vector<GuiButton>::iterator it;
	for (it = m_buttons.begin(); it != m_buttons.end(); it++)
		it->onMouseMove(x, y);

	return isPointWithin(x, y) ? GUI_ANS_STOP : GUI_ANS_PROPAGATE;
}

void GuiButtonList::onResize(int w, int h)
{
	m_parent_w = w;
	m_parent_h = h;
	setPosition(m_relative_x * w, m_relative_y * h);
	m_needsUpdate = true;
}

void GuiButtonList::onUpdate(float interval)
{
	if (m_needsUpdate)
		selfUpdate();
}

void GuiButtonList::onDraw()
{
	glColor4f(0.f, 0.f, 0.f, .25f);
	
	glBegin(GL_QUADS);
		glVertex3f(m_x - m_w, m_y - m_h, 0.f); glVertex3f(m_x + m_w, m_y - m_h, 0.f);
		glVertex3f(m_x + m_w, m_y + m_h, 0.f); glVertex3f(m_x - m_w, m_y + m_h, 0.f);
	glEnd();

	glColor3f(1.f, 1.f, 1.f);
	glBegin(GL_LINE_STRIP);
		glVertex3f(m_x - m_w + GUI_BUTTONLIST_BORDER_DISTANCE, m_y - m_h + GUI_BUTTONLIST_BORDER_DISTANCE, 0.f);
		glVertex3f(m_x + m_w - GUI_BUTTONLIST_BORDER_DISTANCE, m_y - m_h + GUI_BUTTONLIST_BORDER_DISTANCE, 0.f);
		glVertex3f(m_x + m_w - GUI_BUTTONLIST_BORDER_DISTANCE, m_y + m_h - GUI_BUTTONLIST_BORDER_DISTANCE, 0.f);
		glVertex3f(m_x - m_w + GUI_BUTTONLIST_BORDER_DISTANCE, m_y + m_h - GUI_BUTTONLIST_BORDER_DISTANCE, 0.f);
		glVertex3f(m_x - m_w + GUI_BUTTONLIST_BORDER_DISTANCE, m_y - m_h + GUI_BUTTONLIST_BORDER_DISTANCE, 0.f);
	glEnd();
	
	std::vector<GuiButton>::iterator it;
	for (it = m_buttons.begin(); it != m_buttons.end(); it++)
		it->onDraw();
}

void GuiButtonList::selfUpdate()
{
	m_needsUpdate = false;

	std::vector<GuiButton>::iterator it;

	float width, height;
	float widest = 0.f;
	float currentY = GUI_BUTTONLIST_VERTICAL_MARGIN + GUI_BUTTONLIST_BORDER_DISTANCE - GUI_BUTTONLIST_BUTTON_SPACING;

	for (it = m_buttons.begin(); it != m_buttons.end(); it++)
	{
		it->getSize(width, height);

		if (width > widest)
			widest = width;

		currentY += height + GUI_BUTTONLIST_BUTTON_SPACING;
		it->setPosition(m_x, -currentY);
	}

	m_w = widest / 2.f + GUI_BUTTONLIST_HORIZONTAL_MARGIN;
	m_h = (currentY + height + GUI_BUTTONLIST_VERTICAL_MARGIN) / 2.f;

	for (it = m_buttons.begin(); it != m_buttons.end(); it++)
		it->move(0.f, m_y + m_h);
}