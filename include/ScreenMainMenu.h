#ifndef _MAIN_MENU_H
#define _MAIN_MENU_H

#include "ScreenMainMenuBackground.h"
#include "GuiComponent.h"
#include "GuiButtonList.h"

class MainMenu : public GuiComponentAdapter
{
public:

	MainMenu();

	GuiComponentAnswer onMouseClick(int button, int state, int x, int y);
	GuiComponentAnswer onMouseMove(int x, int y);
	void onResize(int w, int h);
	void onUpdate(float interval);
	void onDraw();

private:

	MainMenuBackground m_background;
	GuiButtonList m_mainMenu;
	GuiButtonList m_startNewGame;
	GuiButtonList* m_activeMenu;
};

#endif