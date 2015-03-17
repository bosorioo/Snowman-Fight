#include "glutInclude.h"
#include "Player.h"
#include "Drawing.h"
#include "Game.h"
#include "ScreenPauseMenuGameSettings.h"

PauseMenuGameSettings::PauseMenuGameSettings() :
	m_display_snow("Render Snow"), m_horizontal_split("Horizontal Split Screen"), m_display_minimap("Display Minimap")
{
	m_menu.addButton("Return to Game");
	m_menu.addButton("Restart Game");
	m_menu.addButton("Return to Main Menu");
	m_menu.addButton("Exit");

	GameScreen::GamePreferences prefs = Game::getGamePreferences();

	m_display_snow.setChecked((prefs.booleans & GameScreen::BIT_DISPLAY_SNOW) > 0);
	m_display_minimap.setChecked((prefs.booleans & GameScreen::BIT_DISPLAY_MINIMAP) > 0);
	m_horizontal_split.setChecked((prefs.booleans & GameScreen::BIT_HORIZONTAL_SPLIT) > 0);
}

GuiComponentAnswer PauseMenuGameSettings::onMouseClick(int button, int state, int x, int y)
{
	m_menu.onMouseClick(button, state, x, y);

	m_display_minimap.onMouseClick(button, state, x, y);
	m_display_snow.onMouseClick(button, state, x, y);
	m_horizontal_split.onMouseClick(button, state, x, y);

	return GUI_ANS_STOP;
}

GuiComponentAnswer PauseMenuGameSettings::onMouseMove(int x, int y)
{
	m_menu.onMouseMove(x, y);
	return GUI_ANS_STOP;
}

void PauseMenuGameSettings::onResize(int w, int h)
{
	m_menu.setPosition(.5f * w, .33f * h);
	m_display_minimap.setPosition(w * .5f - 90.f, h * .69f);
	m_display_snow.setPosition(w * .5f - 90.f, h * .80f);
	m_horizontal_split.setPosition(w * .5f - 90.f, h * .91f);
}

void PauseMenuGameSettings::onUpdate(float interval)
{
	m_menu.onUpdate(interval);

	GameScreen::GamePreferences& prefs = Game::getGamePreferences();

	if (m_display_snow.isChecked())
		prefs.booleans |= GameScreen::BIT_DISPLAY_SNOW;
	else
		prefs.booleans &= ~GameScreen::BIT_DISPLAY_SNOW;

	if (m_display_minimap.isChecked())
		prefs.booleans |= GameScreen::BIT_DISPLAY_MINIMAP;
	else
		prefs.booleans &= ~GameScreen::BIT_DISPLAY_MINIMAP;

	if (m_horizontal_split.isChecked())
		prefs.booleans |= GameScreen::BIT_HORIZONTAL_SPLIT;
	else
		prefs.booleans &= ~GameScreen::BIT_HORIZONTAL_SPLIT;

}

void PauseMenuGameSettings::onDraw()
{
	m_menu.onDraw();
	m_display_minimap.onDraw();
	m_display_snow.onDraw();
	m_horizontal_split.onDraw();
}

bool PauseMenuGameSettings::isClicked(int index)
{
	return m_menu[index].isClicked();
}
