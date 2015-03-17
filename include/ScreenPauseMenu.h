#ifndef _SCREENPAUSE_MENU_GAME_SETTINGS
#define _SCREENPAUSE_MENU_GAME_SETTINGS

#include "GuiComponent.h"
#include "GuiPages.h"
#include "ScreenPauseMenuPlayerSettings.h"
#include "ScreenPauseMenuGameSettings.h"
#include "GuiCheckBox.h"

enum PauseMenuStates {PMGS_RESTART, PMGS_MAIN_MENU, PMGS_CLOSED, PMGS_OPEN};

class PauseMenu : public GuiComponentAdapter
{
public:

	PauseMenu();

	void setPlayers(Player* p1, Player* p2);

	GuiComponentAnswer onKeyDown(unsigned char key);
	GuiComponentAnswer onSpecialKeyDown(int key);
	GuiComponentAnswer onMouseClick(int button, int state, int x, int y);
	GuiComponentAnswer onMouseMove(int x, int y);
	void onResize(int w, int h);
	void onUpdate(float interval);
	void onDraw();
	void setState(PauseMenuStates state);
	PauseMenuStates getState();

private:

	bool m_enabled;
	GuiPages m_pages;
	PauseMenuStates m_state;
	PauseMenuPlayerSettings m_player1_settings, m_player2_settings;
	PauseMenuGameSettings m_game_settings;
	GuiCheckBox m_display_snow, m_split_horizotally;

};

#endif