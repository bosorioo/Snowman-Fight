#include "ScreenMainMenuBackground.h"
#include "Game.h"
#include "Texture.h"
#include "glutInclude.h"

MainMenuBackground::MainMenuBackground() :
	m_snowrain(0.f, 2.5f, 0.f, 1.f, 1.f, 220),
	m_ground(150.f, 150.f)
{
}

void MainMenuBackground::init()
{
	m_snowrain.randomize();
	m_snowman.setFace("SnowTextures/face1.bmp");
	m_snowman.setLookDirection(292);
	m_ground.setTexture("SnowTextures/snow.bmp");
	m_sky.setTexture("SnowTextures/skyline.bmp");
}

void MainMenuBackground::update(float interval)
{
	m_snowrain.update(interval);
}

void MainMenuBackground::draw()
{
	Game::setPerspectiveProjection();
	gluLookAt(.85f, 1.2f, 2.8f, 0.45f, .7f, 0.f, 0.f, 1.f, 0.f);
	m_sky.draw();
	Game::setFog(true);
	m_ground.draw();
	m_snowman.draw();
	m_snowrain.draw();
	Game::setFog(false);
}