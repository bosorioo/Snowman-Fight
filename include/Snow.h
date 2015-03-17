#ifndef _SNOW_H
#define _SNOW_H

#include <vector>

class Snow
{
public:
	Snow();
	
	bool isRotating();
	void getPosition(float & x, float & y, float & z);
	void setScaling(float x, float y, float z);
	void setRotationSpeed(float s);
	void setGoalRotation(float x, float y, float z);
	void setRotation(float x, float y, float z);
	void move(float x, float y, float z);
	void setPosition(float x, float y, float z);
	void setColor(float r, float g, float b, float a = 1.f);
	void setModel(int m);
	void update(float interval);
	void draw();

private:

	static bool sSnowFramebufferCreated;
	static bool sHasFramebuffer;
	static unsigned sSnowFramebuffer;
	static unsigned sSnowTexture;
	void createSnowFramebuffer();

	void drawKochSnow();
	void drawKochSnowVariation();
	void drawKochSnowFractalSegment(float x1, float y1, float x2, float y2, int level, int maxlevel, bool ignorelast = false);

	int m_snowModel;
	float m_r, m_g, m_b, m_a;
	float m_x, m_y, m_z;
	float m_rx, m_ry, m_rz; // rotation
	float m_grx, m_gry, m_grz; // goal rotation
	float m_rotation_speed;
	float m_sx, m_sy, m_sz; // scaling
};

#endif

