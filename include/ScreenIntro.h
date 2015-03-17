#ifndef _SCREEN_INTRO_H
#define _SCREEN_INTRO_H

#include "GuiComponent.h"
#include "Snow.h"

class IntroScreen : public GuiComponentAdapter
{
public:

	IntroScreen();

	void onDraw();
	void onUpdate(float interval);

private:

	GuiComponentAdapter* next;
	float timer;
	Snow snow;

};

#endif