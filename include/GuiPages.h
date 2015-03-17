#ifndef _GUI_PAGES_H
#define _GUI_PAGES_H

#include "GuiComponent.h"

#include <vector>
#include <string>

class GuiPages : public GuiComponentAdapter
{
public:

	GuiPages(float width = 0.f, float height = 0.f);

	bool isPointWithin(int x, int y);

	void setActivePage(unsigned id);
	void addPage(std::string name, GuiComponent* component);
	void setPosition(float x, float y);
	void setSize(float x, float y);

	GuiComponentAnswer onSpecialKeyDown(int key);
	GuiComponentAnswer onKeyDown(unsigned char key);
	GuiComponentAnswer onMouseClick(int button, int state, int x, int y);
	GuiComponentAnswer onMouseMove(int x, int y);

	void onResize(int w, int h);
	void onUpdate(float interval);
	void onDraw();

private:

	float m_tx, m_ty;
	float m_x, m_y;
	float m_width, m_height;
	unsigned m_active_page;
	std::vector<float> m_pages_name_width;
	std::vector<std::string> m_pages_name;
	std::vector<GuiComponent*> m_pages;

};

#endif