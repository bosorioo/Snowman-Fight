#include "Drawing.h"
#include "glutInclude.h"
#include <vector>
#include <cmath>

#define PI 3.14159f

void Draw::rectangle2d(float x, float y, float w, float h, bool centered)
{
	if (centered)
	{
		x -= w / 2.f;
		y += h / 2.f;
	}

	glBegin(GL_QUADS);
	glVertex3f(x, y, 0); glVertex3f(x, y - h, 0);
	glVertex3f(x + w, y - h, 0.f); glVertex3f(x + w, y, 0.f);
	glEnd();
}

void Draw::wirerectangle2d(float x, float y, float w, float h, bool centered)
{
	if (centered)
	{
		x -= std::ceil(w / 2.f);
		y += std::ceil(h / 2.f);
	}

	glBegin(GL_LINE_STRIP);
	glVertex3f(x + 1, y - 1, 0); glVertex3f(x + 1, y - h, 0);
	glVertex3f(x + w, y - h, 0.f); glVertex3f(x + w, y - 1, 0.f);
	glVertex3f(x, y - 1, 0);
	glEnd();
}

void Draw::circle2d(float x, float y, float radius, unsigned points)
{
	float angle = 0.f, delta = 2 * PI / (points - 1);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, 0.f); glVertex3f(x + radius, y, 0.f);
	while (points-- >= 2)
	{
		angle += delta;
		glVertex3f(x + radius * std::cos(angle), y - radius * std::sin(angle), 0.f);
	}
	glEnd();
}

void Draw::wirecircle2d(float x, float y, float radius, unsigned points)
{
	float angle = 0.f, delta = 2 * PI / (points - 1);
	glBegin(GL_LINE_STRIP);
	glVertex3f(x + radius, y, 0.f);
	while (points-- >= 2)
	{
		angle += delta;
		glVertex3f(x + radius * std::cos(angle), y - radius * std::sin(angle), 0.f);
	}
	glVertex3f(x + radius, y, 0.f);
	glEnd();
}

void Draw::triangle2d(float base, float height)
{
	glBegin(GL_TRIANGLES);
	glVertex3f(-base / 2.f, -.2887f * height, 0.f);
	glVertex3f(0.f, .5774f * height, 0.f);
	glVertex3f(base / 2.f, -.2887f * height, 0.f);
	glEnd();
}

struct ScissorCut {int id; char centered; float x, y, w, h;};

void Draw::scissor(float x, float y, float width, float height, bool centered)
{
	static std::vector<ScissorCut> cuts;
	ScissorCut cut;

	if (width <= 0.f && height <= 0.f)
	{
		if (cuts.empty())
			return;

		cut = cuts.back();
		cuts.pop_back();

		if (cuts.empty())
		{
			glDisable(GL_STENCIL_TEST);
			return;
		}

		glStencilFunc(GL_NEVER, 0, 0xFF);
		glStencilOp(GL_DECR, GL_DECR, GL_DECR);

		rectangle2d(cut.x, cut.y, cut.w, cut.h, cut.centered != 0);

		glStencilFunc(GL_ALWAYS, cut.id, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}
	else
	{

		if (cuts.empty())
		{
			glClear(GL_STENCIL_BUFFER_BIT);
			glEnable(GL_STENCIL_TEST);
		}

		cut.id = cuts.size() + 1;
		cut.x = x;
		cut.y = y;
		cut.w = width;
		cut.h = height;
		cut.centered = centered ? '1' : '0';

		cuts.push_back(cut);

		glStencilFunc(GL_NEVER, 0, 0xFF);
		glStencilOp(GL_INCR, GL_INCR, GL_INCR);

		rectangle2d(x, y, width, height, centered);

		glStencilFunc(GL_EQUAL, cut.id, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	}
}

void Draw::text(std::string& text, float x, float y, bool verticalCenter, bool horizontalCenter, void* font)
{
	std::vector<std::string> lines;

	unsigned currentPos = 0, nextPos;

	do
	{
		nextPos = text.find_first_of("\n\0", currentPos);

		if (nextPos == std::string::npos)
		{
			if (currentPos < text.size())
				lines.push_back(text.substr(currentPos));
			break;
		}

		lines.push_back(text.substr(currentPos, nextPos - currentPos));
		currentPos = nextPos + 1;

	} while (true);

	float textWidth;
	float xBegin;
	float yBegin = verticalCenter ? y - 9.f + (lines.size() - 1) * 10.f : y;

	for (unsigned i = 0; i < lines.size(); i++)
	{
		textWidth = glutBitmapLength(font, (unsigned char*)lines[i].c_str());
		xBegin = horizontalCenter ? x - textWidth/2.f : x;
		glRasterPos3f(xBegin, yBegin, 0.f);

		for (unsigned j = 0; j < lines[i].size(); j++)
			glutBitmapCharacter(font, lines[i][j]);

		yBegin -= 20.f;
	}
}

void Draw::text3d(std::string& text)
{
	glPushMatrix();

	glScalef(0.002, 0.003, 1.f);

	for (unsigned i = 0; i < text.size(); i++)
		glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, text[i]);

	glPopMatrix();
}
