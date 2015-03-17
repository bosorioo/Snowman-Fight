#ifndef _SNOWMAN_H
#define _SNOWMAN_H

#include <string>
#include "SphericalTexture.h"

class Snowman
{
public:

	Snowman();
	~Snowman();
	
	void accel(float delta, float angleDelta = 0.f);
	void setSpeed(float x, float z);
	void getSpeed(float & x, float & z);
	void move(float delta, float angleDelta = 0.f);
	void turn(float ang);

	void draw();
	void update(float interval);
	void setFace(std::string texturePath);
	void setLookDirection(float angle);
	void getPosition(float & x, float & z);
	void setPosition(float x, float z);
	float getLookDirection();

private:

	void drawBodySnowball(float radius, float contourratio, float scaley, float y);

	float m_rgb[3];
	float m_rgb_countour[3];
	float m_pos_x, m_pos_z;
	float m_speed_x, m_speed_z;
	float m_look_direction;

	SphericalTexture m_faceTexture;

};

#endif