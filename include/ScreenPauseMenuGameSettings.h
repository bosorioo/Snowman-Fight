#ifndef _SCREENPAUSE_MENU_GAME_SETTINGS_H
#define _SCREENPAUSE_MENU_GAME_SETTINGS_H

#include "GuiComponent.h"
#include "Player.h"
#include "GuiButtonList.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include <vector>

class PauseMenuGameSettings : public GuiComponentAdapter
{
public:

	PauseMenuGameSettings();

	GuiComponentAnswer onMouseClick(int button, int state, int x, int y);
	GuiComponentAnswer onMouseMove(int x, int y);
	void onResize(int w, int h);
	void onUpdate(float interval);
	void onDraw();

	bool isClicked(int index);

private:

	GuiButtonList m_menu;
	GuiCheckBox m_display_snow, m_horizontal_split, m_display_minimap;

};

#endif