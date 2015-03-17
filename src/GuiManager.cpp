#include "GuiManager.h"

GuiComponentAnswer GuiManager::onKeyDown(unsigned char key)
{
	GuiComponentAnswer gAnswer;

	for (GuiIterator git = m_components.begin(); git != m_components.end(); git++)
	{
		gAnswer = (*git)->onKeyDown(key);

		if (gAnswer == GUI_ANS_STOP)
			break;
	}

	return GUI_ANS_PROPAGATE;
}

GuiComponentAnswer GuiManager::onKeyUp(unsigned char key)
{
	GuiComponentAnswer gAnswer;

	for (GuiIterator git = m_components.begin(); git != m_components.end(); git++)
	{
		gAnswer = (*git)->onKeyUp(key);

		if (gAnswer == GUI_ANS_STOP)
			break;
	}

	return GUI_ANS_PROPAGATE;
}

GuiComponentAnswer GuiManager::onSpecialKeyDown(int key)
{
	GuiComponentAnswer gAnswer;

	for (GuiIterator git = m_components.begin(); git != m_components.end(); git++)
	{
		gAnswer = (*git)->onSpecialKeyDown(key);

		if (gAnswer == GUI_ANS_STOP)
			break;
	}

	return GUI_ANS_PROPAGATE;
}

GuiComponentAnswer GuiManager::onSpecialKeyUp(int key)
{
	GuiComponentAnswer gAnswer;

	for (GuiIterator git = m_components.begin(); git != m_components.end(); git++)
	{
		gAnswer = (*git)->onKeyUp(key);

		if (gAnswer == GUI_ANS_STOP)
			break;
	}

	return GUI_ANS_PROPAGATE;
}

GuiComponentAnswer GuiManager::onMouseMove(int x, int y)
{
	GuiComponentAnswer gAnswer;

	for (GuiIterator git = m_components.begin(); git != m_components.end(); git++)
	{
		gAnswer = (*git)->onMouseMove(x, y);

		if (gAnswer == GUI_ANS_STOP)
			break;
	}

	return GUI_ANS_PROPAGATE;
}

GuiComponentAnswer GuiManager::onMouseClick(int button, int state, int x, int y)
{
	GuiComponentAnswer gAnswer;

	for (GuiIterator git = m_components.begin(); git != m_components.end(); git++)
	{
		gAnswer = (*git)->onMouseMove(x, y);

		if (gAnswer == GUI_ANS_STOP)
			break;
	}

	return GUI_ANS_PROPAGATE;
}

void GuiManager::onUpdate(float interval)
{
	for (GuiIterator git = m_components.begin(); git != m_components.end(); git++)\
		(*git)->onUpdate(interval);
}

void GuiManager::onDraw()
{
	for (GuiIterator git = m_components.begin(); git != m_components.end(); git++)\
		(*git)->onDraw();
}

void GuiManager::addComponent(GuiComponent* comp)
{
	m_components.push_back(comp);
}