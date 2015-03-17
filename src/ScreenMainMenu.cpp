#include "ScreenMainMenu.h"
#include "ScreenGame.h"
#include "Game.h"

MainMenu::MainMenu() :
	m_activeMenu(0)
{
	m_mainMenu.addButton("Start new game");
	m_mainMenu.addButton("Exit");
	
	m_startNewGame.addButton("Single Player");
	m_startNewGame.addButton("Two Players");
	m_startNewGame.addButton("Return to Main Menu");

	m_activeMenu = &m_mainMenu;
	
	m_background.init();
}

GuiComponentAnswer MainMenu::onMouseClick(int button, int state, int x, int y)
{
	if (m_activeMenu)
		m_activeMenu->onMouseClick(button, state, x, y);

	return GUI_ANS_STOP;
}

GuiComponentAnswer MainMenu::onMouseMove(int x, int y)
{
	if (m_activeMenu)
		m_activeMenu->onMouseMove(x, y);

	return GUI_ANS_STOP;
}

void MainMenu::onResize(int w, int h)
{
	float menuPositionX = 350/500.f, menuPositionY = .5f;
	m_mainMenu.setPosition(menuPositionX * w, menuPositionY * h);
	m_startNewGame.setPosition(menuPositionX * w, menuPositionY * h);
}

void MainMenu::onUpdate(float interval)
{
	if (m_activeMenu)
		m_activeMenu->onUpdate(interval);

	m_background.update(interval);
	GuiButtonList* nextMenu = 0;

	if (m_mainMenu[0].isClicked()) // Start new game
		nextMenu = &m_startNewGame;
	else if (m_mainMenu[1].isClicked()) // Exit
		Game::exit();
	else if (m_startNewGame[0].isClicked()) // One player
		Game::setActiveComponent(new GameScreen(false), true);
	else if (m_startNewGame[1].isClicked()) // Two players
		Game::setActiveComponent(new GameScreen(true), true);
	else if (m_startNewGame[2].isClicked())
		nextMenu = &m_mainMenu;

	if (nextMenu)
	{
		if (m_activeMenu)
			m_activeMenu->resetButtonsState();

		m_activeMenu = nextMenu;
		m_activeMenu->onUpdate(0.f);
	}
}

void MainMenu::onDraw()
{
	m_background.draw();

	Game::setOrthoProjection();
	glDisable(GL_DEPTH_TEST);
	glLoadIdentity();

	if (m_activeMenu)
		m_activeMenu->onDraw();

	glEnable(GL_DEPTH_TEST);
}