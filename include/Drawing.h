#ifndef _DRAWING_H
#define _DRAWING_H

#include <string>
#include "glutInclude.h"

namespace Draw
{
	void rectangle2d(float x, float y, float w, float h, bool centered = false);
	void wirerectangle2d(float x, float y, float w, float h, bool centered = false);
	
	void circle2d(float x, float y, float radius, unsigned points = 12);
	void wirecircle2d(float x, float y, float radius, unsigned points = 12);

	void triangle2d(float base, float height);

	void scissor(float x, float y, float width, float height, bool centered = false);

	void text(std::string& text, float x, float y, bool verticalCenter = false, bool horizontalCenter = false, void * font = GLUT_BITMAP_HELVETICA_18);
	void text3d(std::string& text);
};


#endif