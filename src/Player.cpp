#include "Player.h"
#include "Utils.h"
#include "Game.h"
#include <cstdlib>
#include "glutInclude.h"

#define PLAYER_SNOWMAN_MOVE_SPEED 4.2f
#define PLAYER_SNOWMAN_TURN_SPEED 165.f
#define PLAYER_CHARGE_TIME 2.5f

Player::Player(bool one)
{
	m_movingForward = m_movingBackward = m_turningLeft = m_turningRight = m_paused = 
		m_movingRightward = m_movingLeftward = m_lookingUp = m_lookingDown = 
		m_throwingBall = false;
	m_prefs.cameraMode = 0;
	m_prefs.aspectRatioMode = 0;
	m_snowman.setFace("SnowTextures/face1.bmp");
	m_cameraHeight = 1.f;
	m_is_one = one;
	m_charge = 0.f;
	m_points = 0;
}

float Player::getAspectRatio()
{
	if (m_prefs.aspectRatioMode == 0)
		return -1.f;
	else if (m_prefs.aspectRatioMode == 1)
		return 2.f;

	return .5f;
}

bool Player::hasPaused()
{
	return m_paused;
}

bool Player::isOne()
{
	return m_is_one;
}

void Player::setPause(bool b)
{
	m_paused = b;
}

void Player::setClipping(bool s)
{
	if (!s)
	{
		glDisable(GL_CLIP_PLANE0);
		return;
	}

	glEnable(GL_CLIP_PLANE0);

	float a, b, c, d, e, f;
	getCameraPosition(a, b, c);
	getCameraLookAt(d, e, f);

	double eq[] = {d - a, e - b, f - c, 1.5};

	if (m_prefs.cameraMode == 2)
		eq[3] = 6.5;

	Utils::normalize(eq, 3);
	m_snowman.getPosition(a, b);

	glTranslatef(a, 0.f, b);
	glClipPlane(GL_CLIP_PLANE0, eq);
	glTranslatef(-a, 0.f, -b);
}

void Player::setPreferences(PlayerPreferences& prefs)
{
	m_movingForward = m_movingBackward = m_turningLeft = m_turningRight = m_paused = 
		m_movingRightward = m_movingLeftward = m_lookingUp = m_lookingDown = 
		m_throwingBall = false;
	m_prefs = prefs;
	m_snowman.setFace(m_prefs.snowmanFaceTexture);
}

Player::PlayerPreferences Player::getPreferences()
{
	return m_prefs;
}

void Player::getSnowmanPosition(float & x, float & z)
{
	m_snowman.getPosition(x, z);
}

void Player::getSnowmanRotation(float & rot)
{
	rot = m_snowman.getLookDirection();
}

void Player::onKeyDown(int key, bool isSpecial)
{
	KeyInput input(key, isSpecial);

	if (isKeyControl(MoveForward, input))
		m_movingForward = true;

	if (isKeyControl(MoveBackward, input))
		m_movingBackward = true;

	if (isKeyControl(MoveLeft, input))
		m_movingLeftward = true;

	if (isKeyControl(MoveRight, input))
		m_movingRightward = true;

	if (isKeyControl(LookUp, input))
		m_lookingUp = true;

	if (isKeyControl(LookDown, input))
		m_lookingDown = true;

	if (isKeyControl(TurnLeft, input))
		m_turningLeft = true;

	if (isKeyControl(TurnRight, input))
		m_turningRight = true;

	if (isKeyControl(Pause, input))
	{
		m_paused = !m_paused;
	}

	if (isKeyControl(ThrowingBall, input))
		m_throwingBall = true;

	if (isKeyControl(ChangeCamera, input))
	{
		m_prefs.cameraMode = (m_prefs.cameraMode + 1) % 5;
		Game::getPlayerPreferences(m_is_one).cameraMode = m_prefs.cameraMode;
	}

	if (isKeyControl(ChangeFace, input))
	{
		int face = m_prefs.snowmanFaceTexture[17] - '1';
		face = (face + 1) % 5;
		m_prefs.snowmanFaceTexture[17] = face + '1';
		Game::getPlayerPreferences(m_is_one).snowmanFaceTexture = m_prefs.snowmanFaceTexture;
		m_snowman.setFace(m_prefs.snowmanFaceTexture);
	}
}

void Player::onKeyUp(int key, bool isSpecial)
{
	KeyInput input(key, isSpecial);

	if (isKeyControl(MoveForward, input))
		m_movingForward = false;

	if (isKeyControl(MoveBackward, input))
		m_movingBackward = false;

	if (isKeyControl(MoveLeft, input))
		m_movingLeftward = false;

	if (isKeyControl(MoveRight, input))
		m_movingRightward = false;

	if (isKeyControl(LookUp, input))
		m_lookingUp = false;

	if (isKeyControl(LookDown, input))
		m_lookingDown = false;

	if (isKeyControl(TurnLeft, input))
		m_turningLeft = false;

	if (isKeyControl(TurnRight, input))
		m_turningRight = false;

	if (isKeyControl(ThrowingBall, input))
		m_throwingBall = false;
}

void Player::getCameraPosition(float & ox, float & oy, float & oz)
{
	float x, z, angle, cos, sin;
	m_snowman.getPosition(x, z);

	switch (m_prefs.cameraMode)
	{
		case 0:
		{
			angle = Utils::toRad(m_snowman.getLookDirection());
			cos = std::cos(angle);
			sin = -std::sin(angle);
			ox = x - 3.f * cos;
			oy = 2.5f;
			oz = z - 3.f * sin; 
			break;
		}

		case 1:
		{
			angle = Utils::toRad(m_snowman.getLookDirection());
			cos = std::cos(angle);
			sin = -std::sin(angle);
			ox = x - 0.3f * cos;
			oy = 1.5f;
			oz = z - 0.3f * sin; 
			break;
		}

		case 2:
		{
			angle = Utils::toRad(m_snowman.getLookDirection());
			cos = std::cos(angle);
			sin = -std::sin(angle);
			ox = x + 7.f * cos;
			oy = 2.5f;
			oz = z + 7.f * sin; 
			break;
		}

		case 3:
		{
			angle = Utils::toRad(m_snowman.getLookDirection() + 90.f);
			cos = std::cos(angle);
			sin = -std::sin(angle);
			ox = x + 5.f * cos;
			oy = 1.5f;
			oz = z + 5.f * sin; 
			break;
		}

		case 4:
		{
			angle = Utils::toRad(m_snowman.getLookDirection() - 90.f);
			cos = std::cos(angle);
			sin = -std::sin(angle);
			ox = x + 5.f * cos;
			oy = 1.5f;
			oz = z + 5.f * sin; 
			break;
		}

		default:
			break;
	}
}

void Player::getCameraLookAt(float & ox, float & oy, float & oz)
{
	float x, z, angle, cos, sin;
	m_snowman.getPosition(x, z);

	switch (m_prefs.cameraMode)
	{

	case 0: case 1:
		{
			angle = Utils::toRad(m_snowman.getLookDirection());
			cos = std::cos(angle);
			sin = -std::sin(angle);
			float cameraDistance = 2.f;
			ox = x + cameraDistance * cos;
			oy = m_cameraHeight;
			oz = z + cameraDistance * sin; 

			if (m_prefs.cameraMode == 1)
				oy = Utils::limit(-2.f, oy, 1.5f);

			break;
		}

	case 2: case 3: case 4:
	{
		ox = x;
		oy = 1.2f;
		oz = z;
		break;
	}

		default:
			break;
	}
}

bool Player::isKeyControl(Player::PlayerControls controlId, Player::KeyInput input)
{
	if (m_prefs.inputs.size() <= (unsigned)controlId || (int)controlId < 0)
		return false;

	KeyInput control = m_prefs.inputs[controlId];
	return control.keyValue == input.keyValue && control.isSpecial == input.isSpecial;
}

void Player::onUpdate(float interval)
{
	if (m_movingForward && !m_movingBackward)
		m_snowman.accel(PLAYER_SNOWMAN_MOVE_SPEED * interval);
	else if (m_movingBackward && !m_movingForward)
		m_snowman.accel(-PLAYER_SNOWMAN_MOVE_SPEED * interval);

	if (m_movingLeftward && !m_movingRightward)
		m_snowman.accel(PLAYER_SNOWMAN_MOVE_SPEED * interval, 90.f);
	else if (m_movingRightward && !m_movingLeftward)
		m_snowman.accel(PLAYER_SNOWMAN_MOVE_SPEED * interval, -90.f);

	if (m_turningLeft && !m_turningRight)
		m_snowman.turn(PLAYER_SNOWMAN_TURN_SPEED * interval);
	else if (m_turningRight && !m_turningLeft)
		m_snowman.turn(-PLAYER_SNOWMAN_TURN_SPEED * interval);

	if (m_lookingUp && !m_lookingDown)
		m_cameraHeight = Utils::limit(-2.f, m_cameraHeight + 5.f * interval, 2.f);
	else if (m_lookingDown && !m_lookingUp)
		m_cameraHeight = Utils::limit(-2.f, m_cameraHeight - 5.f * interval, 2.f);

	if (m_throwingBall)
	{
		m_charge = std::min(1.f, m_charge + 1.f / PLAYER_CHARGE_TIME * interval);
	}
}

void Player::onDraw()
{
	m_snowman.draw();
}

void Player::setCamera()
{
	float x, y, z, ax, ay, az;
	getCameraPosition(x, y, z);
	getCameraLookAt(ax, ay, az);
	glLoadIdentity();

	gluLookAt(x, y, z, ax, ay, az, 0.f, 1.f, 0.f);
}

Snowman& Player::getSnowman()
{
	return m_snowman;
}

float Player::getCharge(bool discharge)
{
	if (m_throwingBall && discharge)
		return 0.f;

	float charge = m_charge;

	if (discharge)
		m_charge = 0.f;

	return charge;
}

void Player::drawCharge()
{
	glBegin(GL_QUAD_STRIP);

	for (float i = 0; i <= 3.8f; i += .05f)
	{
		if (m_charge * 3.8f > i)
		{
			if (i < 1.f)
				glColor3f(0.f, 0.2f, .5f + .5f * i);
			else if (i < 2.f)
				glColor3f(0.f, i - .5f, 2.f - i);
			else
				glColor3f(i - 2.f, 3.f - i, 0.f);
		}
		else
			glColor4f(1.f, 1.f, 1.f, .5f);

		glVertex3f(i * 25.f, 15.f + 5 / 3.f * i, 0.f);
		glVertex3f(i * 25.f, 5.f - 5 / 3.f * i, 0.f);
	}

	glEnd();
}

void Player::addPoint(int delta)
{
	m_points += delta;
}

int Player::getPoints()
{
	return m_points;
}