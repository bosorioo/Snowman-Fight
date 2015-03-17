#ifndef _GUI_CHECK_BOX
#define _GUI_CHECK_BOX

#include "GuiComponent.h"
#include <string>

class GuiCheckBox : public GuiComponentAdapter
{
public:

	GuiCheckBox(std::string label = "");

	bool isPointWithin(int x, int y);
	bool isChecked();
	void setPosition(float x, float y);
	void setChecked(bool b);

	GuiComponentAnswer onMouseClick(int button, int state, int x, int y);
	void onDraw();

private:

	std::string m_label;
	bool m_checked;
	float m_x, m_y;

};


#endif