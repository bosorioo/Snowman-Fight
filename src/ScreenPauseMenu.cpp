#include "ScreenPauseMenu.h"
#include "glutInclude.h"
#include "ScreenGame.h"
#include "Game.h"

PauseMenu::PauseMenu() :
	m_display_snow("Display Snow"), m_split_horizotally("Split Screen Horizontally")
{
	m_pages.addPage("Game Settings", &m_game_settings);

	m_state = PMGS_CLOSED;
}

void PauseMenu::setPlayers(Player* p1, Player* p2)
{
	if (p1 && !p2)
	{
		m_pages.addPage("Player Settings", &m_player1_settings);
		m_player1_settings.setPlayer(p1);
	}
	else if (p1 && p2)
	{
		m_pages.addPage("Player 1 Settings", &m_player1_settings);
		m_player1_settings.setPlayer(p1);
		m_pages.addPage("Player 2 Settings", &m_player2_settings);
		m_player2_settings.setPlayer(p2);
	}
}

GuiComponentAnswer PauseMenu::onKeyDown(unsigned char key)
{
	m_pages.onKeyDown(key);

	return GUI_ANS_STOP;
}

GuiComponentAnswer PauseMenu::onSpecialKeyDown(int key)
{
	m_pages.onSpecialKeyDown(key);

	return GUI_ANS_STOP;
}

GuiComponentAnswer PauseMenu::onMouseClick(int button, int state, int x, int y)
{
	m_pages.onMouseClick(button, state, x, y);

	return GUI_ANS_STOP;
}

GuiComponentAnswer PauseMenu::onMouseMove(int x, int y)
{
	m_pages.onMouseMove(x, y);

	return GUI_ANS_STOP;
}

void PauseMenu::onResize(int w, int h)
{
	m_pages.setPosition(w / 2.f, h * .42f);
	m_pages.setSize(w * .9f, h * .81f);
}

void PauseMenu::onUpdate(float interval)
{
	m_pages.onUpdate(interval);
	
	if (m_game_settings.isClicked(0)) // Return to game
		setState(PMGS_CLOSED);
	else if (m_game_settings.isClicked(1)) // Restart game
		setState(PMGS_RESTART);
	else if (m_game_settings.isClicked(2)) // Go to main menu
		setState(PMGS_MAIN_MENU);
	else if (m_game_settings.isClicked(3)) // Exit
		Game::exit();

	if (m_player1_settings.isDone() || m_player2_settings.isDone()) // Return to game
		setState(PMGS_CLOSED);
}

void PauseMenu::onDraw()
{
	m_pages.onDraw();
}

void PauseMenu::setState(PauseMenuStates state)
{
	if (m_state != state && PMGS_OPEN == state)
		m_pages.setActivePage(0);

	m_state = state;
}

PauseMenuStates PauseMenu::getState()
{
	return m_state;
}