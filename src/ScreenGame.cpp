#include "ScreenGame.h"
#include "Game.h"
#include "Drawing.h"
#include "Utils.h"
#include "glutInclude.h"
#include "ScreenMainMenu.h"

#include <cmath>

#define PLAYER_MIN_SHOOTING_CHARGE 0.09f

GameScreen::GameScreen(bool isMultiplayer) :
    m_prefs(Game::getGamePreferences()),
    m_ground(80.f, 80.f),
	m_snowrain(0.f, 12.f, 0.f, 25.f, 25.f, 480),
	m_player1(0), m_player2(0)
{
    m_isMultiplayer = isMultiplayer;
	m_pause = 0;

	Game::clearScreen(1.f, 1.f, 1.f);

	m_player1 = new Player(true);
	m_player1->setPreferences(Game::getPlayerPreferences(true));

	if (isMultiplayer)
	{
		m_player2 = new Player(false);
		m_player2->setPreferences(Game::getPlayerPreferences(false));

		m_player1->getSnowman().setPosition(- MAP_SIZE * .4f, - MAP_SIZE * .4f);
		m_player1->getSnowman().setLookDirection(315.f);
		m_player2->getSnowman().setPosition(MAP_SIZE * .4f, MAP_SIZE * .4f);
		m_player2->getSnowman().setLookDirection(135.f);
	}

	m_ground.setTexture("SnowTextures/snow.bmp");
	m_snowrain.setScaling(6.5f);
	m_snowrain.randomize();
	m_sky.setTexture("SnowTextures/skyline.bmp");

	m_barricade.push_back(Grid(MAP_SIZE + BARRICADE_SIZE * 2.f, BARRICADE_HEIGHT));
	m_barricade[0].setColor(0.f, 0.f, 0.f, .92f);

	m_pause_menu.setPlayers(m_player1, m_player2);

	m_bubbleTimer = Utils::getRandomFloat(0.5f, 2.f);
	m_timer = 120.f;
}

GameScreen::~GameScreen()
{

}

GuiComponentAnswer GameScreen::onKeyDown(unsigned char key)
{
	if (m_pause)
	{
		m_pause_menu.onKeyDown(key);
	}

	if (m_player1)
		m_player1->onKeyDown((int)key, false);

	if (m_player2)
		m_player2->onKeyDown((int)key, false);

	return GUI_ANS_STOP;
}

GuiComponentAnswer GameScreen::onKeyUp(unsigned char key)
{
	if (m_player1)
		m_player1->onKeyUp((int)key, false);

	if (m_player2)
		m_player2->onKeyUp((int)key, false);

	return GUI_ANS_STOP;
}

GuiComponentAnswer GameScreen::onSpecialKeyDown(int key)
{
	if (m_pause)
	{
		m_pause_menu.onSpecialKeyDown(key);
	}

	if (m_player1)
		m_player1->onKeyDown(key, true);

	if (m_player2)
		m_player2->onKeyDown(key, true);

	return GUI_ANS_STOP;
}

GuiComponentAnswer GameScreen::onSpecialKeyUp(int key)
{
	if (m_player1)
		m_player1->onKeyUp(key, true);

	if (m_player2)
		m_player2->onKeyUp(key, true);

	return GUI_ANS_STOP;
}

GuiComponentAnswer GameScreen::onMouseMove(int x, int y)
{

	m_pause_menu.onMouseMove(x, y);
	return GUI_ANS_STOP;
}

GuiComponentAnswer GameScreen::onMouseClick(int button, int state, int x, int y)
{
	if (m_pause)
	{
		m_pause_menu.onMouseClick(button, state, x, y);
	}

	return GUI_ANS_STOP;
}

void GameScreen::onResize(int w, int h)
{
	m_pause_menu.onResize(w, h);
}

void GameScreen::onUpdate(float interval)
{
	if (m_pause == 1 && !m_player1->hasPaused())
		m_pause = 0;
	else if (!m_pause && m_player1->hasPaused())
		m_pause = 1;

	if (m_pause == 2 && !m_player2->hasPaused())
		m_pause = 0;
	else if (!m_pause && m_player2 && m_player2->hasPaused())
		m_pause = 2;

	if (m_pause)
	{
		m_pause_menu.setState(PMGS_OPEN);
		m_pause_menu.onUpdate(interval);
		PauseMenuStates state = m_pause_menu.getState();

		if (state == PMGS_CLOSED)
		{
			(m_pause == 1 ? m_player1 : m_player2)->setPause(false);
			m_pause = 0;
		}
		else if (state == PMGS_MAIN_MENU)
		{
			Game::setActiveComponent(new MainMenu, true);
			return;
		}
		else if (state == PMGS_RESTART)
		{
			Game::setActiveComponent(new GameScreen(m_isMultiplayer), true);
			return;
		}

		return;
	}

	m_bubbleTimer -= interval;
	m_timer -= interval;

	if (m_bubbleTimer <= 0.f)
	{
		m_bubbleTimer = Utils::getRandomFloat(3.5f, 12.5f);
		createBubble();
	}

	if (checkPreferenceFlag(BIT_DISPLAY_SNOW))
		m_snowrain.update(interval);

	m_player1->onUpdate(interval);
	updateSnowman(m_player1->getSnowman(), interval);

	if (m_player2)
	{
		m_player2->onUpdate(interval);
		updateSnowman(m_player2->getSnowman(), interval);

		float x[2], y[2];
		m_player1->getSnowmanPosition(x[0], y[0]);
		m_player2->getSnowmanPosition(x[1], y[1]);

		if (Utils::pointDistance(x[0], y[0], x[1], y[1]) <= 0.76f)
		{
			float normal[2] = {x[1] - x[0], y[1] - y[0]};
			Utils::normalize(normal, 2);

			m_player1->getSnowman().getSpeed(x[0], x[1]);
			m_player2->getSnowman().getSpeed(y[0], y[1]);

			float speed = (Utils::vectorLength(x, 2) + Utils::vectorLength(y, 2)) * .75f;

			m_player1->getSnowman().setSpeed(speed * -normal[0], speed * -normal[1]);
			m_player2->getSnowman().setSpeed(speed * normal[0], speed * normal[1]);

			m_player1->getSnowmanPosition(x[0], y[0]);
			m_player2->getSnowmanPosition(x[1], y[1]);

			float delta = .39f - Utils::pointDistance(x[0], y[0], x[1], y[1]) / 2.f;

			m_player1->getSnowman().setPosition(x[0] - normal[0] * delta, y[0] - normal[1] * delta);
			m_player2->getSnowman().setPosition(x[1] + normal[0] * delta, y[1] + normal[1] * delta);
		}
	}

	float charge = m_player1->getCharge();
	SnowCharge snowball;

	if (charge > PLAYER_MIN_SHOOTING_CHARGE)
	{
		snowball.fromPlayerOne = true;
		m_player1->getSnowmanPosition(snowball.x, snowball.z);
		snowball.y = 1.2f;
		m_player1->getSnowman().getSpeed(snowball.speedX, snowball.speedZ);
		snowball.speedY = 1.8f + 3.2f * charge;
		snowball.speedX += (2.f + 7.f * charge) * std::cos(Utils::toRad(m_player1->getSnowman().getLookDirection()));
		snowball.speedZ *= -1;
		snowball.speedZ += (2.f + 7.f * charge) * std::sin(Utils::toRad(m_player1->getSnowman().getLookDirection()));
		snowball.size = charge * .07f + 0.03f;
		m_snowballs.push_back(snowball);
	}

	if (m_player2 && (charge = m_player2->getCharge()) > PLAYER_MIN_SHOOTING_CHARGE)
	{
		snowball.fromPlayerOne = false;
		m_player2->getSnowmanPosition(snowball.x, snowball.z);
		snowball.y = 1.2f;
		m_player2->getSnowman().getSpeed(snowball.speedX, snowball.speedZ);
		snowball.speedY = 1.8f + 3.2f * charge;
		snowball.speedX += (2.f + 7.f * charge) * std::cos(Utils::toRad(m_player2->getSnowman().getLookDirection()));
		snowball.speedZ *= -1;
		snowball.speedZ += (2.f + 7.f * charge) * std::sin(Utils::toRad(m_player2->getSnowman().getLookDirection()));
		snowball.size = charge * .07f + 0.03f;
		m_snowballs.push_back(snowball);
	}

	for (unsigned i = 0; i < m_snowballs.size(); i++)
	{
		m_snowballs[i].x += m_snowballs[i].speedX * interval;
		m_snowballs[i].y += m_snowballs[i].speedY * interval;
		m_snowballs[i].z -= m_snowballs[i].speedZ * interval;

		if (m_snowballs[i].y <= 0)
		{
			m_snowballs[i].y = 0.f;
			m_snowballs[i].speedX = m_snowballs[i].speedZ = 0.f;
			m_snowballs[i].size -= 0.02f * interval;

			if (m_snowballs[i].size <= 0.f)
			{
				m_snowballs.erase(m_snowballs.begin() + i);
				i--;
			}
			continue;
		}

		m_snowballs[i].speedY -= 8.9f * interval;

		for (unsigned j = 0; j < m_bubbles.size(); j++)
		{
			if (Utils::pointDistance(m_bubbles[j].x, m_bubbles[j].y, m_bubbles[j].z,
				m_snowballs[i].x, m_snowballs[i].y, m_snowballs[i].z) <=
				m_snowballs[i].size + BUBBLE_RADIUS)
			{
				m_bubbles.erase(m_bubbles.begin() + j);
				j--;

				if (m_snowballs[i].fromPlayerOne)
					m_player1->addPoint(1);
				else
					m_player2->addPoint(1);

				continue;
			}
		}
	}

	float bubbleSpeed = .5f * interval;

	for (unsigned i = 0; i < m_bubbles.size(); i++)
	{
		if (Utils::pointDistance(m_bubbles[i].x, m_bubbles[i].gx) <= bubbleSpeed)
		{
			m_bubbles[i].x = m_bubbles[i].gx;
			m_bubbles[i].gx += Utils::getRandomFloat(-4.f, 4.f);
			if (m_bubbles[i].gx >= MAP_SIZE / 2.f)
				m_bubbles[i].gx = MAP_SIZE / 2.f;
			else if (m_bubbles[i].gx <= -MAP_SIZE / 2.f)
				m_bubbles[i].gx = -MAP_SIZE / 2.f;
		}
		else
			m_bubbles[i].x += bubbleSpeed * Utils::signalFromBool(Utils::isPositive(m_bubbles[i].gx - m_bubbles[i].x));

		if (Utils::pointDistance(m_bubbles[i].y, m_bubbles[i].gy) <= bubbleSpeed)
		{
			m_bubbles[i].y = m_bubbles[i].gy;
			m_bubbles[i].gy = Utils::getRandomFloat(0.55f, 2.2f);
		}
		else
		{
			m_bubbles[i].y += .2f * bubbleSpeed * Utils::signalFromBool(Utils::isPositive(m_bubbles[i].gy - m_bubbles[i].y));
		}

		if (Utils::pointDistance(m_bubbles[i].z, m_bubbles[i].gz) <= bubbleSpeed)
		{
			m_bubbles[i].z = m_bubbles[i].gz;
			m_bubbles[i].gz += Utils::getRandomFloat(-4.f, 4.f);
			if (m_bubbles[i].gz >= MAP_SIZE / 2.f)
				m_bubbles[i].gz = MAP_SIZE / 2.f;
			else if (m_bubbles[i].gz <= -MAP_SIZE / 2.f)
				m_bubbles[i].gz = -MAP_SIZE / 2.f;
		}
		else
			m_bubbles[i].z += bubbleSpeed * Utils::signalFromBool(Utils::isPositive(m_bubbles[i].gz - m_bubbles[i].z));
	}
}

void GameScreen::onDraw()
{
	if (!m_player1)
		return;

	Game::setFog(true);

	float x, y;
	std::string pname;

	if (m_isMultiplayer)
		setHalfScreen(true);
	else
		Game::setPerspectiveProjection();

	m_player1->getSnowmanPosition(x, y);
	m_player1->setCamera();
	m_player1->setClipping(true);
	m_ground.draw();
	Game::setFog(false);
	m_sky.setPosition(x, y);
	m_sky.draw();
	Game::setFog(true);
	m_player1->onDraw();

	if (checkPreferenceFlag(BIT_DISPLAY_SNOW))
	{
		m_snowrain.draw();
	}

	if (m_player2)
		m_player2->onDraw();

	drawBarricades();
	drawSnowballs();
	drawBubbles();

	m_player1->setClipping(false);

	if (m_isMultiplayer && m_player2)
	{
		setHalfScreen(false);
		m_player2->getSnowmanPosition(x, y);
		m_player2->setCamera();
		m_player2->setClipping(true);
		m_ground.draw();
		Game::setFog(false);
		m_sky.setPosition(x, y);
		m_sky.draw();
		Game::setFog(true);
		m_player2->onDraw();

		if (checkPreferenceFlag(BIT_DISPLAY_SNOW))
		{
			m_snowrain.draw();
		}

		m_player1->onDraw();
		drawBarricades();
		drawSnowballs();
		drawBubbles();

		m_player2->setClipping(false);
	}

	Game::applyViewport();
	Game::setFog(false);

	if (m_isMultiplayer)
		drawInfoMultiplayer();
	else
		drawInfoSingleplayer();

	if (checkPreferenceFlag(BIT_DISPLAY_MINIMAP))
		drawMinimap();

	if (m_pause)
		drawPause();
}

void GameScreen::setPreferences(GamePreferences prefs)
{
	m_prefs = prefs;
}

GameScreen::GamePreferences GameScreen::getPreferences()
{
	return m_prefs;
}

void GameScreen::updateSnowman(Snowman& man, float interval)
{
	man.update(interval);
	float x, y, sx, sy;
	man.getPosition(x, y);
	man.getSpeed(sx, sy);

	if (x + sx * interval < -MAP_SIZE / 2.f)
	{
		x = -MAP_SIZE - (x + sx * interval);
		sx *= -1.f;
	}
	else if (x + sx * interval > MAP_SIZE / 2.f)
	{
		x = MAP_SIZE - (x + sx * interval);
		sx *= -1.f;
	}
	else
		x += sx * interval;

	if (y + sy * interval < -MAP_SIZE / 2.f)
	{
		y = -MAP_SIZE - (y + sy * interval);
		sy *= -1.f;
	}
	else if (y + sy * interval > MAP_SIZE / 2.f)
	{
		y = MAP_SIZE - (y + sy * interval);
		sy *= -1.f;
	}
	else
		y += sy * interval;

	man.setPosition(x, y);
	man.setSpeed(sx, sy);
}

bool GameScreen::checkPreferenceFlag(unsigned char bit)
{
	return (m_prefs.booleans & bit) > 0;
}

void GameScreen::setHalfScreen(bool isPlayerOne)
{
	Player* player = isPlayerOne ? m_player1 : m_player2;

	if (!player)
		return;

	int w, h;
	Game::getWindowSize(w, h);

	float aspec = (isPlayerOne ? m_player1 : m_player2)->getAspectRatio();

	if (aspec < 0.f)
		aspec = (float)w / h;

	if (m_isMultiplayer)
		aspec *= (checkPreferenceFlag(BIT_HORIZONTAL_SPLIT) ? 2.f : 0.5f);

	Game::setPerspectiveProjection(aspec);

	if (checkPreferenceFlag(BIT_HORIZONTAL_SPLIT))
		glViewport(0, isPlayerOne ? h / 2 : 0, w, h / 2);
	else
		glViewport(isPlayerOne ? 0 : w / 2, 0, w / 2, h);
}

void GameScreen::drawInfoSingleplayer()
{
	glDisable(GL_DEPTH_TEST);

	int w, h;
	Game::getWindowSize(w, h);
	Game::setOrthoProjection();
	glLoadIdentity();

	glColor3f(0.f, 0.f, 0.f);
	std::string str;
	char buffer[256];

	glColor3f(1.f, 0.f, 0.f);
	sprintf(buffer, "Shoot Strength: %s%.0f%%", m_player1->getCharge(false) * 100.f >= 10.f ? "" : "0", m_player1->getCharge(false) * 100.f);
	str = buffer;
	Draw::text(str, 10.f, 68.f, false, false, GLUT_BITMAP_8_BY_13);
	glTranslatef(12.f, 35.f, 0.f);
	m_player1->drawCharge();
	glTranslatef(-12.f, -35.f, 0.f);

	sprintf(buffer, "Points: %d", m_player1->getPoints());
	str = buffer;
	glColor3f(1.f, 0.f, 0.f);
	Draw::text(str, 10.f, 89.f, false, false, GLUT_BITMAP_HELVETICA_18);


	glEnable(GL_DEPTH_TEST);
}

void GameScreen::drawInfoMultiplayer()
{
	glDisable(GL_DEPTH_TEST);

	int w, h;
	Game::getWindowSize(w, h);
	Game::setOrthoProjection();
	glLoadIdentity();

	glColor3f(0.f, 0.f, 0.f);
	std::string str;
	char buffer[256];

	if (checkPreferenceFlag(BIT_HORIZONTAL_SPLIT))
	{
		Draw::rectangle2d(0.f, h / 2.f + 1, w, 1, false);

		glColor3f(1.f, 0.f, 0.f);
		sprintf(buffer, "Shoot Strength: %s%.0f%%", m_player1->getCharge(false) * 100.f >= 10.f ? "" : "0", m_player1->getCharge(false) * 100.f);
		str = buffer;
		Draw::text(str, w * .7f - 18.f, h * .61f, false, false, GLUT_BITMAP_8_BY_13);
		glTranslatef(w * .7f, h * .54f, 0.f);
		m_player1->drawCharge();
		glTranslatef(-w * .7f, -h * .54f, 0.f);
		sprintf(buffer, "Points: %d", m_player1->getPoints());
		str = buffer;
		glColor3f(1.f, 0.f, 0.f);
		Draw::text(str, w * .7f + 18.f, h * .61f + 18.f, false, false, GLUT_BITMAP_HELVETICA_18);

		if (m_player2)
		{
			glColor3f(1.f, 0.f, 0.f);
			sprintf(buffer, "Shoot Strength: %s%.0f%%", m_player2->getCharge(false) * 100.f >= 10.f ? "" : "0", m_player2->getCharge(false) * 100.f);
			str = buffer;
			Draw::text(str, w * .7f - 18.f, h * .11f, false, false, GLUT_BITMAP_8_BY_13);
			glTranslatef(w * .7f, h * .04f, 0.f);
			m_player2->drawCharge();
			glTranslatef(-w * .7f, -h * .04f, 0.f);
			sprintf(buffer, "Points: %d", m_player2->getPoints());
			str = buffer;
			glColor3f(1.f, 0.f, 0.f);
			Draw::text(str, w * .7f + 18.f, h * .11f + 18.f, false, false, GLUT_BITMAP_HELVETICA_18);
		}

	}
	else
	{
		Draw::rectangle2d(w / 2.f - 1, h, 1, h, false);

		glColor3f(1.f, 0.f, 0.f);
		sprintf(buffer, "Shoot Strength: %s%.0f%%", m_player1->getCharge(false) * 100.f >= 10.f ? "" : "0", m_player1->getCharge(false) * 100.f);
		str = buffer;
		Draw::text(str, 12.f, 50.f, false, false, GLUT_BITMAP_8_BY_13);
		glTranslatef(15.f, 20.f, 0.f);
		m_player1->drawCharge();
		glTranslatef(-15.f, -20.f, 0.f);

		sprintf(buffer, "Points: %d", m_player1->getPoints());
		str = buffer;
		glColor3f(1.f, 0.f, 0.f);
		Draw::text(str, 31.f, 70.f, false, false, GLUT_BITMAP_HELVETICA_18);

		if (m_player2)
		{
			glColor3f(1.f, 0.f, 0.f);
			sprintf(buffer, "Shoot Strength: %s%.0f%%", m_player2->getCharge(false) * 100.f >= 10.f ? "" : "0", m_player2->getCharge(false) * 100.f);
			str = buffer;
			Draw::text(str, w - 163.f, 50.f, false, false, GLUT_BITMAP_8_BY_13);
			glTranslatef(w - 110.f, 20.f, 0.f);
			m_player2->drawCharge();
			glTranslatef(110.f - w, -20.f, 0.f);

			sprintf(buffer, "Points: %d", m_player2->getPoints());
			str = buffer;
			glColor3f(1.f, 0.f, 0.f);
			Draw::text(str, w - 100, 70.f, false, false, GLUT_BITMAP_HELVETICA_18);
		}
	}

	glEnable(GL_DEPTH_TEST);
}

void GameScreen::drawMinimap()
{
	glDisable(GL_DEPTH_TEST);

	int w, h;
	Game::getWindowSize(w, h);
	Game::setOrthoProjection();
	glLoadIdentity();

	float minimapSize = std::floor(std::min(w, h) * .2f);
	float minimapX = minimapSize * .75f;
	float minimapY = h / 2.f;

	if (!checkPreferenceFlag(BIT_HORIZONTAL_SPLIT))
	{
		minimapX = w / 2.f;
		minimapY = minimapSize * .75f;
	}

	glColor4f(0.f, 0.f, 0.f, .8f);
	Draw::rectangle2d(minimapX, minimapY, minimapSize, minimapSize, true);
	glColor3f(1.f, 1.f, 1.f);
	Draw::wirerectangle2d(minimapX, minimapY, minimapSize - 4, minimapSize - 4, true);

	float x, y, rot;

	m_player1->getSnowmanPosition(x, y);
	m_player1->getSnowmanRotation(rot);
	x = (x / MAP_SIZE) * (minimapSize - 4) + minimapX;
	y = (- y / MAP_SIZE) * (minimapSize - 4) + minimapY;

	glColor3f(m_prefs.player1_minimap_color[0]/255.f,m_prefs.player1_minimap_color[1]/255.f,m_prefs.player1_minimap_color[2]/255.f);

	glPushMatrix();
	glTranslatef(x, y, 0.f);
	glRotatef(rot - 90.f, 0.f, 0.f, 1.f);
	Draw::triangle2d(8.f, 14.f);
	glPopMatrix();
	Draw::wirecircle2d(x, y, 7.f);
	Draw::wirecircle2d(x, y, 9.f);

	if (m_isMultiplayer && m_player2)
	{
		m_player2->getSnowmanPosition(x, y);
		m_player2->getSnowmanRotation(rot);

		x = (x / MAP_SIZE) * minimapSize + minimapX;
		y = (- y / MAP_SIZE) * minimapSize + minimapY;

		glColor3f(m_prefs.player2_minimap_color[0]/255.f,m_prefs.player2_minimap_color[1]/255.f,m_prefs.player2_minimap_color[2]/255.f);

		glPushMatrix();
		glTranslatef(x, y, 0.f);
		glRotatef(rot - 90.f, 0.f, 0.f, 1.f);
		Draw::triangle2d(8.f, 14.f);
		glPopMatrix();
		Draw::wirecircle2d(x, y, 7.f);
		Draw::wirecircle2d(x, y, 9.f);
	}

	glColor4f(.4f, 0.6f, 1.f, .8f);
	for (unsigned i = 0; i < m_bubbles.size(); i++)
	{
		Draw::circle2d((m_bubbles[i].x / MAP_SIZE) * (minimapSize - 4) + minimapX,
			(-m_bubbles[i].z / MAP_SIZE) * (minimapSize - 4) + minimapY, 2.f, 5);
	}

	glEnable(GL_DEPTH_TEST);
}

void GameScreen::drawBarricades()
{
	for (unsigned i = 0; i < 4; i++)
	{
		glPushMatrix();
		glTranslatef(std::cos(i * 1.5708f) * (MAP_SIZE + 2 * BARRICADE_SIZE) / 2.f,
			BARRICADE_HEIGHT / 2.f, -std::sin(i * 1.5708f) * (MAP_SIZE + 2 * BARRICADE_SIZE) / 2.f);
		glRotatef((i + 1) * 90.f, 0.f, 1.f, 0.f);
		glRotatef(90.f, 1.f, 0.f, 0.f);
		m_barricade[0].draw();
		glPopMatrix();
	}
}

void GameScreen::drawPause()
{
	Game::setOrthoProjection();
	glDisable(GL_DEPTH_TEST);

	Game::paintScreen(0.f, 0.f, 0.f, 0.4f);
	Game::infoBox(m_pause == 1 ? "Player 1 Paused" : "Player 2 Paused");
	m_pause_menu.onDraw();

	glEnable(GL_DEPTH_TEST);
}

void GameScreen::drawSnowballs()
{
	glColor3f(1.f, 1.f, 1.f);
	for (unsigned i = 0; i < m_snowballs.size(); i++)
	{
		glTranslatef(m_snowballs[i].x, m_snowballs[i].y, m_snowballs[i].z);
		glutSolidSphere(m_snowballs[i].size, 10, 10);
		glTranslatef(-m_snowballs[i].x, -m_snowballs[i].y, -m_snowballs[i].z);
	}
}

void GameScreen::drawBubbles()
{
	glColor4f(.4f, 0.6f, 1.f, .8f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	for (unsigned i = 0; i < m_bubbles.size(); i++)
	{
		glTranslatef(m_bubbles[i].x, m_bubbles[i].y, m_bubbles[i].z);
		glutSolidSphere(BUBBLE_RADIUS, 20, 20);
		glTranslatef(-m_bubbles[i].x, -m_bubbles[i].y, -m_bubbles[i].z);
	}
	glDisable(GL_CULL_FACE);
}

void GameScreen::createBubble(int level)
{

	if (Utils::getRandomInt(1, 5) == 1 && level < 1)
		createBubble(level + 1);

	if (m_bubbles.size() >= MAX_BUBBLES)
		return;

	Bubble b;
	b.x = Utils::getRandomFloat(-MAP_SIZE / 2.f, MAP_SIZE / 2.f);
	b.y = Utils::getRandomFloat(0.55f, 2.2f);
	b.z = Utils::getRandomFloat(-MAP_SIZE / 2.f, MAP_SIZE / 2.f);
	b.gx = b.x + Utils::getRandomFloat(-4.f, 4.f);
	b.gy = Utils::getRandomFloat(0.55f, 2.2f);
	b.gz = b.z + Utils::getRandomFloat(-4.f, 4.f);

	if (b.gx < -MAP_SIZE / 2.f)
		b.gx = -MAP_SIZE / 2.f;
	else if (b.gx > MAP_SIZE / 2.f)
		b.gx = MAP_SIZE / 2.f;

	if (b.gz < -MAP_SIZE / 2.f)
		b.gz = -MAP_SIZE / 2.f;
	else if (b.gz > MAP_SIZE / 2.f)
		b.gz = MAP_SIZE / 2.f;

	m_bubbles.push_back(b);
}
