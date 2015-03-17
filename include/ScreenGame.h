#ifndef _SCREEN_GAME_H
#define _SCREEN_GAME_H

#include "GuiComponent.h"
#include "Player.h"
#include "Grid.h"
#include "SnowRain.h"
#include "Sky.h"
#include "GuiPages.h"
#include "ScreenPauseMenu.h"
#include "glutInclude.h"

#define MAP_SIZE 20.f
#define BARRICADE_SIZE .38f
#define BARRICADE_HEIGHT 1.2f
#define MAX_BUBBLES 9
#define BUBBLE_RADIUS .58f

class GameScreen : public GuiComponentAdapter
{
public:

	enum GamePrefsBits {
		BIT_HORIZONTAL_SPLIT = 1 << 0,
		BIT_DISPLAY_MINIMAP = 1 << 1,
		BIT_DISPLAY_SNOW = 1 << 2
	};

	struct GamePreferences {
		unsigned char booleans,
			player1_minimap_color[3],
			player2_minimap_color[3];
	};

	GameScreen(bool isMultiplayer);
	~GameScreen();

	GuiComponentAnswer onKeyDown(unsigned char key);
	GuiComponentAnswer onKeyUp(unsigned char key);
	GuiComponentAnswer onSpecialKeyDown(int key);
	GuiComponentAnswer onSpecialKeyUp(int key);
	GuiComponentAnswer onMouseMove(int x, int y);
	GuiComponentAnswer onMouseClick(int button, int state, int x, int y);
	void onResize(int w, int h);
	void onUpdate(float interval);
	void onDraw();

	void setPreferences(GamePreferences prefs);
	GamePreferences getPreferences();

private:

	void updateSnowman(Snowman& man, float interval);
	bool checkPreferenceFlag(unsigned char bit);
	void setHalfScreen(bool isPlayerOne);
	void drawInfoSingleplayer();
	void drawInfoMultiplayer();
	void drawMinimap();
	void drawBarricades();
	void drawPause();
	void drawSnowballs();
	void drawBubbles();
	void createBubble(int level = 0);

	float m_bubbleTimer;
	float m_timer;

	GamePreferences& m_prefs;

	Sky m_sky;
	Grid m_ground;
	std::vector<Grid> m_barricade;
	SnowRain m_snowrain;
	Player *m_player1, *m_player2;

	PauseMenu m_pause_menu;

	unsigned char m_pause;
	bool m_isMultiplayer;

	struct Bubble {
		float x, y, z;
		float gx, gy, gz;
	};

	std::vector<Bubble> m_bubbles;

	struct SnowCharge {
		bool fromPlayerOne;
		float speedX, speedY, speedZ, x, y, z, size;
	};

	std::vector<SnowCharge> m_snowballs;

};


#endif