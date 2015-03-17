#include "glutInclude.h"
#include "Player.h"
#include "Drawing.h"
#include "Game.h"
#include "ScreenPauseMenuPlayerSettings.h"
#include <cmath>

std::string getKeyName(Player::KeyInput& key)
{
	if (key.keyValue == -1)
		return "N/A";

	if (!key.isSpecial)
	{
		if (key.keyValue == ' ')
			return "Spacebar";
		else if (key.keyValue == 13)
			return "Enter";
		else if (key.keyValue == '\t')
			return "Tab";
	}
	else
	{
		switch (key.keyValue) {
		case GLUT_KEY_UP: return "Up Arrow"; case GLUT_KEY_LEFT: return "Left Arrow";
		case GLUT_KEY_DOWN: return "Down Arrow"; case GLUT_KEY_RIGHT: return "Right Arrow";
		case GLUT_KEY_END: return "End"; case GLUT_KEY_HOME: return "Home";
		case GLUT_KEY_INSERT: return "Insert"; case GLUT_KEY_PAGE_DOWN: return "Page Down";
		case GLUT_KEY_PAGE_UP: return "Page Up"; default: break;
		}
	}

	std::string str;
	str += (unsigned char)key.keyValue;

	return str;
}

std::string getKeyName(std::vector<Player::KeyInput>& keys, unsigned index)
{
	if (keys.size() <= index)
		return "N/A";

	return getKeyName(keys[index]);
}

PauseMenuPlayerSettings::PauseMenuPlayerSettings() :
	m_done_button("Done"), m_control_buttons(Player::NumberOfControls, GuiButton("Change", 0, 0, GLUT_BITMAP_8_BY_13)),
	m_editOkay("Apply", 0.f, 0.f, GLUT_BITMAP_8_BY_13), m_editCancel("Cancel", 0.f, 0.f, GLUT_BITMAP_8_BY_13)
{
	m_player = 0;
	m_done = false;
	m_keyEdit = -1;
}

void PauseMenuPlayerSettings::setPlayer(Player* player)
{
	m_player = player;

	if (player)
	{
		m_prefs = player->getPreferences();
		if (m_prefs.inputs.size() <= Player::NumberOfControls)
			m_prefs.inputs.resize(Player::NumberOfControls);
	}
}

GuiComponentAnswer PauseMenuPlayerSettings::onKeyDown(unsigned char key)
{
	if (m_keyEdit >= 0)
	{
		m_key.keyValue = key;
		m_key.isSpecial = false;
	}

	return GUI_ANS_STOP;
}

GuiComponentAnswer PauseMenuPlayerSettings::onSpecialKeyDown(int key)
{
	if (m_keyEdit >= 0)
	{
		m_key.keyValue = key;
		m_key.isSpecial = true;
	}

	return GUI_ANS_STOP;
}

GuiComponentAnswer PauseMenuPlayerSettings::onMouseClick(int button, int state, int x, int y)
{
	if (m_keyEdit >= 0)
	{
		m_editOkay.onMouseClick(button, state, x, y);
		m_editCancel.onMouseClick(button, state, x, y);
		return GUI_ANS_STOP;
	}

	m_done_button.onMouseClick(button, state, x, y);

	for (unsigned i = 0; i < m_control_buttons.size(); i++)
		m_control_buttons[i].onMouseClick(button, state, x, y);

	return GUI_ANS_STOP;
}

GuiComponentAnswer PauseMenuPlayerSettings::onMouseMove(int x, int y)
{
	m_done_button.onMouseMove(x, y);
	m_editCancel.onMouseMove(x, y);
	m_editOkay.onMouseMove(x, y);

	for (unsigned i = 0; i < m_control_buttons.size(); i++)
		m_control_buttons[i].onMouseMove(x, y);

	return GUI_ANS_STOP;
}

void PauseMenuPlayerSettings::onResize(int w, int h)
{
	m_done_button.setPosition(w * .5f, h * .08f);
	m_editOkay.setPosition(w * .5f, h * .5f + 30.f);
	m_editCancel.setPosition(w * .5f, h * .5f);

	unsigned i, j;

	for (i = 0; i < std::ceil(m_control_buttons.size() / 2); i++)
		m_control_buttons[i].setPosition(.41f * w, h * .92f - i * 52.f);

	for (j = 0; i < m_control_buttons.size(); j++, i++)
		m_control_buttons[i].setPosition(.91f * w, h * .92f - j * 52.f);

}

void PauseMenuPlayerSettings::onUpdate(float interval)
{

	if (m_keyEdit >= 0)
	{
		if (m_editOkay.isClicked())
		{
			m_prefs.inputs[m_keyEdit] = m_key;
			m_keyEdit = -1;
		}

		if (m_editCancel.isClicked())
			m_keyEdit = -1;

		return;
	}

	for (unsigned i = 0; i < m_control_buttons.size(); i++)
	{
		if (m_control_buttons[i].isClicked())
		{
			m_keyEdit = i;
			m_key = m_prefs.inputs[i];
			return;
		}
	}

	if (m_done_button.isClicked())
	{
		m_done = true;

		if (m_player)
		{
			Player::PlayerPreferences prefs = m_player->getPreferences();
			prefs.inputs = m_prefs.inputs;
			m_player->setPreferences(prefs);
			m_prefs = prefs;
			Game::getPlayerPreferences(m_player->isOne()) = prefs;
		}
	}
}

void PauseMenuPlayerSettings::onDraw()
{
	if (m_keyEdit >= 0)
	{
		std::string text = "Press the new key";
		float x, y;
		m_editOkay.getPosition(x, y);
		glColor3f(.855f, .647f, .125f);
		Draw::text(text, x, y + 70.f, true, true, GLUT_BITMAP_8_BY_13);
		text = getKeyName(m_key);
		glColor3f(.655f, .247f, .015f);
		Draw::text(text, x, y + 40.f, true, true, GLUT_BITMAP_HELVETICA_18);
		m_editOkay.onDraw();
		m_editCancel.onDraw();
		return;
	}

	static std::string labels[] = {
		"Move Forward", "Move Backward", "Move Right", "Move Left", "Look Up", "Look Down", "Turn Left", "Turn Right",
		"Pause", "Shoot", "Change Camera", "Change Face"
	};

	m_done_button.onDraw();
	float x, y;
	std::string mappedKey;

	for (unsigned i = 0; i < m_control_buttons.size(); i++)
	{
		m_control_buttons[i].onDraw();
		m_control_buttons[i].getPosition(x, y);
		glColor3f(1.f, 1.f, 1.f);
		Draw::text(labels[i], x - 170.f, y + 12.f, true, false, GLUT_BITMAP_8_BY_13);
		glColor3f(.855f, .647f, .125f);
		mappedKey = "Key: " + getKeyName(m_prefs.inputs, i);
		Draw::text(mappedKey, x - 172.f, y - 5.f, true, false, GLUT_BITMAP_8_BY_13);
	}
}

bool PauseMenuPlayerSettings::isDone()
{
	bool ret = m_done;
	m_done = false;
	return ret;
}
