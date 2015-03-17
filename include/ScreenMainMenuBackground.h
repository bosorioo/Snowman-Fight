#ifndef _MAINMENU_BACKGROUND_H
#define _MAINMENU_BACKGROUND_H

#include "SnowRain.h"
#include "Snowman.h"
#include "Grid.h"
#include "Sky.h"

class MainMenuBackground
{
public:

	MainMenuBackground();

	void init();
	void update(float interval);
	void draw();

private:

	SnowRain m_snowrain;
	Snowman m_snowman;
	Grid m_ground;
	Sky m_sky;
};

#endif