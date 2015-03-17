#ifndef _GUI_MANAGER_H
#define _GUI_MANAGER_H

#include <vector>
#include "GuiComponent.h"

class GuiManager : public GuiComponentAdapter
{
public:

	GuiComponentAnswer onKeyDown(unsigned char key);
	GuiComponentAnswer onKeyUp(unsigned char key);
	GuiComponentAnswer onSpecialKeyDown(int key);
	GuiComponentAnswer onSpecialKeyUp(int key);
	GuiComponentAnswer onMouseMove(int x, int y);
	GuiComponentAnswer onMouseClick(int button, int state, int x, int y);
	void onUpdate(float interval);
	void onDraw();

	void addComponent(GuiComponent* comp);

private:

	typedef std::vector<GuiComponent*>::iterator GuiIterator;

	std::vector<GuiComponent*> m_components;
};

#endif