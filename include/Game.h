#ifndef _GAME_H
#define _GAME_H

#include "GuiComponent.h"
#include "ScreenGame.h"
#include <string>
#include <cstdio>
#include "Player.h"

class Game
{
public:

	static void startUp();
	static void getPreferredWindowSize(int & width, int & height);
	static void setMousePosition(int x, int y);
	static void getMousePosition(int & x, int & y);
	static void getWindowSize(int & w, int & h);
	static void exit();
	static void applyViewport();
	static void setActiveComponent(GuiComponent* gui, bool deleteAfterUse = false);
	static void setOrthoProjection();
	static void setPerspectiveProjection(float aspectRatio = 0.f);
	static void setFog(bool b);
	static void clearScreen(float r = 1.f, float g = 1.f, float b = 1.f);
	static void paintScreen(float r, float g, float b, float a = 1.f);
	static Player::PlayerPreferences& getPlayerPreferences(bool playerOne = true);
	static GameScreen::GamePreferences& getGamePreferences();
	static void infoBox(std::string info);

	static void onKeyDown(unsigned char key);
	static void onKeyUp(unsigned char key);
	static void onSpecialKeyDown(int key);
	static void onSpecialKeyUp(int key);
	static void onMouseMove(int x, int y);
	static void onMouseClick(int button, int state, int x, int y);
	static void onResize(int width, int height);
	static void onUpdate(float interval);
	static void onDraw();

private:

	void setDefaultPlayerPreferences(Player::PlayerPreferences& prefs);
	void saveGameSettings();
	bool loadGameSettings();
	void savePlayerPreferences(std::vector<unsigned char>& data, Player::PlayerPreferences& prefs);
	Player::PlayerPreferences loadPlayerPreferences(FILE* file);

	static Game globalGame;

	Game();
	Game(Game const &);
	Game& operator=(Game const &);

	GuiComponent* m_activeComponent;
	Player::PlayerPreferences m_player1, m_player2;
	GameScreen::GamePreferences m_game_prefs;
	int m_width, m_height, m_preferredWidth, m_preferredHeight;
	int m_mouse_x, m_mouse_y;
	float m_aspectRatio;
	bool m_deleteComponent, m_keepAspectRatio;
};

#endif