#include "GuiCheckBox.h"
#include "glutInclude.h"
#include "Drawing.h"

#define GUI_BOX_SIZE 12

GuiCheckBox::GuiCheckBox(std::string label)
{
	m_label = label;
	m_checked = false;
}

bool GuiCheckBox::isPointWithin(int x, int y)
{
	return x >= m_x - GUI_BOX_SIZE/2 && x <= m_x + GUI_BOX_SIZE/2 && y >= m_y - GUI_BOX_SIZE/2 && y <= m_y + GUI_BOX_SIZE/2; 
}

bool GuiCheckBox::isChecked()
{
	return m_checked;
}

void GuiCheckBox::setPosition(float x, float y)
{
	m_x = x;
	m_y = y;
}

void GuiCheckBox::setChecked(bool b)
{
	m_checked = b;
}

GuiComponentAnswer GuiCheckBox::onMouseClick(int button, int state, int x, int y)
{
	if (!isPointWithin(x, y) || button == GLUT_RIGHT_BUTTON || state == GLUT_UP)
		return GUI_ANS_PROPAGATE;

	m_checked = !m_checked;

	return GUI_ANS_STOP;
}

void GuiCheckBox::onDraw()
{
	if (m_checked)
	{
		glColor3f(0., 66.f, 0.12f);
		glLineWidth(3.0f);
		glBegin(GL_LINES);
			glVertex3f(m_x - GUI_BOX_SIZE/2 + 2, m_y - GUI_BOX_SIZE/2 + 2, 0);
			glVertex3f(m_x + GUI_BOX_SIZE/2 - 2, m_y + GUI_BOX_SIZE/2 - 2, 0);
			glVertex3f(m_x + GUI_BOX_SIZE/2 - 2, m_y - GUI_BOX_SIZE/2 + 2, 0);
			glVertex3f(m_x - GUI_BOX_SIZE/2 + 2, m_y + GUI_BOX_SIZE/2 - 2, 0);
		glEnd();
		glLineWidth(1.0f);
	}

	glColor3f(1.f, 1.f, 1.f);
	Draw::wirerectangle2d(m_x, m_y, GUI_BOX_SIZE, GUI_BOX_SIZE, true);
	Draw::text(m_label, m_x + 4 + GUI_BOX_SIZE, m_y - 5.f, false, false);
}