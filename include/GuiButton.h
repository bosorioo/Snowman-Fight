#ifndef _GUI_BUTTON_H
#define _GUI_BUTTON_H

#include <string>
#include "GuiComponent.h"
#include "glutInclude.h"

#define GUI_BUTTON_MAX_LABEL_LENGTH 40
#define GUI_BUTTON_GLUT_FONT GLUT_BITMAP_HELVETICA_18
#define GUI_BUTTON_FONT_CORRECTION_X -1
#define GUI_BUTTON_FONT_CORRECTION_Y 2
#define GUI_BUTTON_CHAR_HEIGHT 18
#define GUI_BUTTON_HORIZONTAL_MARGIN 10
#define GUI_BUTTON_VERTICAL_MARGIN 6

class GuiButton : public GuiComponentAdapter
{
public:

	GuiButton(char* label = 0, int x = 0, int y = 0, void* font = GUI_BUTTON_GLUT_FONT);

	void setLabel(char* label);
	void setPosition(int x, int y);
	void getPosition(float & x, float & y);
	void move(int x, int y);
	void getSize(float & width, float & height);
	void resetState();
	bool isClicked();

	GuiComponentAnswer onMouseClick(int button, int state, int x, int y);
	GuiComponentAnswer onMouseMove(int x, int y);
	void onDraw();

private:

	bool isPointWithin(int x, int y);
	void calculateDimensions();

	void* m_font;
	std::string m_label;
	float m_x, m_y, m_label_w, m_label_h;
	bool m_isMouseHovering;
	bool m_clicked;

};

#endif