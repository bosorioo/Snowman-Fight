#ifndef _GUI_BUTTON_LIST_H
#define _GUI_BUTTON_LIST_H

#include "GuiComponent.h"
#include "GuiButton.h"
#include <string>
#include <vector>

#define GUI_BUTTONLIST_BORDER_DISTANCE 3.f
#define GUI_BUTTONLIST_VERTICAL_MARGIN 0.f
#define GUI_BUTTONLIST_HORIZONTAL_MARGIN 20.f
#define GUI_BUTTONLIST_BUTTON_SPACING 18.f

class GuiButtonList : public GuiComponentAdapter
{
public:

	bool isPointWithin(int x, int y);

	GuiButton& addButton(std::string label);
	GuiButton& operator[](unsigned index);
	void setPosition(float x, float y);
	void setRelativePosition(float x, float y);
	void resetButtonsState();

	GuiComponentAnswer onMouseClick(int button, int state, int x, int y);
	GuiComponentAnswer onMouseMove(int x, int y);
	void onResize(int w, int h);
	void onUpdate(float interval);
	void onDraw();

private:

	void selfUpdate();

	bool m_needsUpdate;
	float m_x, m_y, m_w, m_h;
	float m_relative_x, m_relative_y;
	float m_parent_w, m_parent_h;
	std::vector<GuiButton> m_buttons;
};

#endif