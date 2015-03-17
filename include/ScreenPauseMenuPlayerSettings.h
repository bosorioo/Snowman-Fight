#ifndef _SCREENPAUSE_MENU_PLAYER_SETTINGS
#define _SCREENPAUSE_MENU_PLAYER_SETTINGS

#include "GuiComponent.h"
#include "Player.h"
#include "GuiButton.h"
#include <vector>

class PauseMenuPlayerSettings : public GuiComponentAdapter
{
public:

	PauseMenuPlayerSettings();

	void setPlayer(Player* player);

	GuiComponentAnswer onKeyDown(unsigned char key);
	GuiComponentAnswer onSpecialKeyDown(int key);
	GuiComponentAnswer onMouseClick(int button, int state, int x, int y);
	GuiComponentAnswer onMouseMove(int x, int y);
	void onResize(int w, int h);
	void onUpdate(float interval);
	void onDraw();

	bool isDone();

private:

	Player* m_player;
	Player::PlayerPreferences m_prefs;
	GuiButton m_done_button;
	std::vector<GuiButton> m_control_buttons;
	bool m_done;

	GuiButton m_editOkay, m_editCancel;
	Player::KeyInput m_key;
	int m_keyEdit;

};

#endif