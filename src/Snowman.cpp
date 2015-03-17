#include "Snowman.h"
#include "Texture.h"
#include "Utils.h"
#include <iostream>
#include "glutInclude.h"

#define SNOWMAN_BASE_BALL_RADIUS 0.38f
#define SNOWMAN_BASE_BALL_HEIGHT 0.33f
#define SNOWMAN_MID_BALL_RADIUS 0.25f
#define SNOWMAN_MID_BALL_HEIGHT 0.22f
#define SNOWMAN_HEAD_BALL_RADIUS 0.18f
#define SNOWMAN_NOSE_BASE_RADIUS 0.04f
#define SNOWMAN_NOSE_BASE_SIZE 0.18f

#define SNOWMAN_FRICTION 1.0f
#define SNOWMAN_MAX_SPEED 6.f

Snowman::Snowman() :
	m_faceTexture(6, SNOWMAN_HEAD_BALL_RADIUS + 0.002f)
{
	m_rgb[0] = m_rgb[1] = 0.95f; m_rgb[2] = 1.f; 
	m_rgb_countour[0] = m_rgb_countour[1] = m_rgb_countour[2] = 0.f;
	m_look_direction = 0.f;
	m_pos_x = m_pos_z = 0.f;
	m_speed_x = m_speed_z = 0.f;
}

Snowman::~Snowman()
{

}

void Snowman::accel(float delta, float angleDelta)
{
	float cos = std::cos(Utils::toRad(m_look_direction + angleDelta));
	float sin = -std::sin(Utils::toRad(m_look_direction + angleDelta));

	m_speed_x += delta * cos;
	m_speed_z += delta * sin;

	float len = Utils::vectorLength(m_speed_x, m_speed_z);

	if (len > SNOWMAN_MAX_SPEED)
	{
		m_speed_x = SNOWMAN_MAX_SPEED * m_speed_x / len;
		m_speed_z = SNOWMAN_MAX_SPEED * m_speed_z / len;
	}
}

void Snowman::setSpeed(float x, float z)
{
	m_speed_x = x;
	m_speed_z = z;

	float len = Utils::vectorLength(m_speed_x, m_speed_z);

	if (len > SNOWMAN_MAX_SPEED)
	{
		m_speed_x = SNOWMAN_MAX_SPEED * m_speed_x / len;
		m_speed_z = SNOWMAN_MAX_SPEED * m_speed_z / len;
	}
}

void Snowman::getSpeed(float & x, float & z)
{
	x = m_speed_x;
	z = m_speed_z;
}

void Snowman::move(float delta, float angleDelta)
{
	float x = std::cos(Utils::toRad(m_look_direction + angleDelta)) * delta;
	float z = -std::sin(Utils::toRad(m_look_direction + angleDelta)) * delta;
	m_speed_x += x;
	m_speed_z += z;
}

void Snowman::turn(float ang)
{
	m_look_direction += ang;
}

void Snowman::setFace(std::string texturePath)
{
	m_faceTexture.setTexture(texturePath);
}

void Snowman::setLookDirection(float angle)
{
	m_look_direction = angle;
}

void Snowman::draw()
{
	glPushMatrix();

	glTranslatef(m_pos_x, 0.f, m_pos_z);
	glRotatef(m_look_direction, 0.f, 1.f, 0.f);

	float currentY = -0.08f;

	drawBodySnowball(SNOWMAN_BASE_BALL_RADIUS, 1.04f, SNOWMAN_BASE_BALL_HEIGHT / SNOWMAN_BASE_BALL_RADIUS,
		SNOWMAN_BASE_BALL_HEIGHT + currentY);
	currentY += SNOWMAN_BASE_BALL_HEIGHT * 2.f;

	drawBodySnowball(SNOWMAN_MID_BALL_RADIUS, 1.055f, SNOWMAN_MID_BALL_HEIGHT / SNOWMAN_MID_BALL_RADIUS,
		SNOWMAN_MID_BALL_HEIGHT + currentY - 0.08f);
	currentY += SNOWMAN_MID_BALL_HEIGHT * 2.f - 0.08f;

	drawBodySnowball(SNOWMAN_HEAD_BALL_RADIUS, 1.07f, 1.f, SNOWMAN_HEAD_BALL_RADIUS + currentY - 0.02f);

	glPushMatrix();
	glTranslatef(0.f, SNOWMAN_HEAD_BALL_RADIUS + currentY - 0.02f, 0.f);
	glRotatef(90.f, 0.f, 1.f, 0.f);
	m_faceTexture.draw();
	glPopMatrix();

	currentY += SNOWMAN_HEAD_BALL_RADIUS - 0.02f;

	glColor3f(1.f, 0.4f, 0.f);
	glPushMatrix();
	glRotatef(90.f, 0.f, 1.f, 0.f);
	glTranslatef(0.f, currentY, SNOWMAN_HEAD_BALL_RADIUS * .9f);
	glutSolidCone(SNOWMAN_NOSE_BASE_RADIUS, SNOWMAN_NOSE_BASE_SIZE, 12, 12);
	glPopMatrix();

	glPopMatrix();
}

void Snowman::update(float interval)
{
	float vecLen = Utils::vectorLength(m_speed_x, m_speed_z);

	if (vecLen <= 0.f)
		return;

	float dX = SNOWMAN_FRICTION * interval * m_speed_x / vecLen;
	float dY = SNOWMAN_FRICTION * interval * m_speed_z / vecLen;

	if (std::abs(m_speed_x) - std::abs(dX) < 0.f)
		m_speed_x = 0.f;
	else
		m_speed_x -= dX;

	if (std::abs(m_speed_z) - std::abs(dY) < 0.f)
		m_speed_z = 0.f;
	else
		m_speed_z -= dY;
}

void Snowman::getPosition(float & x, float & z)
{
	x = m_pos_x;
	z = m_pos_z;
}

void Snowman::setPosition(float x, float z)
{
	m_pos_x = x;
	m_pos_z = z;
}

float Snowman::getLookDirection()
{
	return m_look_direction;
}

void Snowman::drawBodySnowball(float radius, float contourratio, float scaley, float y)
{
	glPushMatrix();
	
		glTranslatef(0.f, y, 0.f);
		glScalef(1.f, scaley, 1.f);
		glRotatef(90.f, 1.f, 0.f, 0.f);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT);
		glColor3f(m_rgb_countour[0], m_rgb_countour[1], m_rgb_countour[2]);
		glutSolidSphere(radius * contourratio, 36, 36);
		glDisable(GL_CULL_FACE);
		glColor3f(m_rgb[0], m_rgb[1], m_rgb[2]);
		glutSolidSphere(radius, 36, 36);

	glPopMatrix();
}