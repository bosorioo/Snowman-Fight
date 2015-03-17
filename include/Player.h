#ifndef _PLAYER_H
#define _PLAYER_H

#include "Snowman.h"
#include <string>
#include <vector>

class Player
{
public:

	struct KeyInput {
		KeyInput(int _keyValue = -1, bool _isSpecial = false) : isSpecial(_isSpecial ? '\1' : '\0'), keyValue(_keyValue) {} 
		char isSpecial;
		int keyValue;
	};

	struct PlayerPreferences {
		std::string snowmanFaceTexture;
		std::vector<KeyInput> inputs;
		unsigned char snowmanColor[3];
		unsigned char aspectRatioMode;
		unsigned char cameraMode;
	};

	enum PlayerControls {
		MoveForward = 0, MoveBackward, MoveRight, MoveLeft, LookUp, LookDown, TurnLeft, TurnRight, Pause,
		ThrowingBall, ChangeCamera, ChangeFace,

		NumberOfControls
	};

	Player(bool one);

	float getAspectRatio();
	bool hasPaused();
	bool isOne();
	void setPause(bool b);
	void setClipping(bool s);

	void setPreferences(PlayerPreferences& prefs);
	PlayerPreferences getPreferences();

	void getSnowmanPosition(float & x, float & z);
	void getSnowmanRotation(float & rot);

	void onKeyDown(int key, bool isSpecial);
	void onKeyUp(int key, bool isSpecial);

	void onUpdate(float interval);
	void onDraw();
	void setCamera();

	Snowman& getSnowman();
	float getCharge(bool discharge = true);
	void drawCharge();
	void addPoint(int delta);
	int getPoints();

private:

	void getCameraPosition(float & x, float & y, float & z);
	void getCameraLookAt(float & x, float & y, float & z);

	float m_charge;
	int m_points;

	bool isKeyControl(PlayerControls controlId, KeyInput input);
	bool m_movingForward, m_movingBackward, m_movingLeftward, m_movingRightward, m_turningLeft, m_turningRight,
		m_lookingUp, m_lookingDown, m_paused, m_throwingBall;
	bool m_is_one;
	float m_cameraHeight;
	PlayerPreferences m_prefs;
	Snowman m_snowman;

};

#endif