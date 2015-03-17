#include "glutInclude.h"
#include "ScreenIntro.h"
#include "Drawing.h"
#include "Game.h"
#include "ScreenMainMenu.h"

IntroScreen::IntroScreen()
{
	timer = 0.f;
	snow.setModel(1);
	snow.setColor(1.f, 1.f, 1.f);
	snow.setPosition(0.f, 0.f, 0.f);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	next = 0;
}

void IntroScreen::onDraw()
{
	if (next)
	{
		next->onDraw();
		Game::paintScreen(0.f, 0.f, 0.f, 1.f - timer);
		return;
	}

	int w, h;
	Game::getWindowSize(w, h);
	Game::setPerspectiveProjection();
	Game::paintScreen(0.f, 0.f, 0.f);

	glLoadIdentity();
	gluLookAt(0.f, 0.f, 6.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f);
	glColor3f(1.f, 1.f, 1.f);

	glPushMatrix();
	glScalef(.75f, .75f, 1.f);
	snow.draw();
	glPopMatrix();
	
	std::string txt("Snowman Fight");
	Game::setOrthoProjection();
	glColor3f(0.f, 0.f, 0.f);
	Draw::text(txt, w / 2, h / 2, false, true, GLUT_BITMAP_HELVETICA_18);

	if (timer <= 1.f)
		Game::paintScreen(0.f, 0.f, 0.f, 1.f);
	else if (timer <= 2.5f)
		Game::paintScreen(0.f, 0.f, 0.f, 1.f - (timer - 1.f) / 1.5f);
	else if (timer >= 4.5f)
		Game::paintScreen(0.f, 0.f, 0.f, timer - 4.5f);
}

void IntroScreen::onUpdate(float interval)
{
	timer += interval;

	if (timer >= 6.f && !next)
	{
		int w, h;
		Game::getWindowSize(w, h);
		next = new MainMenu;
		next->onResize(w, h);
		timer = 0.f;
	}

	if (next)
		next->onUpdate(interval);

	if (next && timer >= 1.f)
		Game::setActiveComponent(next, true);
}