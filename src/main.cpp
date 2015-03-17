#include <cstdlib>
#include <ctime>

#include "Game.h"
#include "ScreenMainMenu.h"
#include "ScreenIntro.h"
#include "TextureLoader.h"
#include "glutInclude.h"

#define UPDATE_INTERVAL_MS 20

float gAmbientLightIntensity = .075f;
int gHeight, gWidth;
bool gIsSelecting;
bool gPauseSoftware = false;
char gPauseSoftwareKey = '`';

void setup();
void drawScene();
void resize(int w, int h);
void specialKeyInputUp(int key, int x, int y);
void specialKeyInput(int key, int x, int y);
void keyInputUp(unsigned char key, int x, int y);
void keyInput(unsigned char key, int x, int y);
void mouseInput(int button, int state, int x, int y);
void mouseMove(int x, int y);
void timerFunction(int arg);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);

	int winWidth, winHeight;
	Game::getPreferredWindowSize(winWidth, winHeight);

	glutInitWindowSize (winWidth, winHeight);

	glutInitWindowPosition (500, 200);
	glutCreateWindow ("Snowman Fight");
	setup();
	glutDisplayFunc(drawScene);
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyInput);
	glutKeyboardUpFunc(keyInputUp);
	glutSpecialFunc(specialKeyInput);
	glutSpecialUpFunc(specialKeyInputUp);
	glutPassiveMotionFunc(mouseMove);
	glutMotionFunc(mouseMove);
	glutMouseFunc(mouseInput);
	glutTimerFunc(UPDATE_INTERVAL_MS, timerFunction, UPDATE_INTERVAL_MS);
	glutMainLoop();

	return 0;
}

void timerFunction(int arg)
{
	if (!gPauseSoftware)
		Game::onUpdate(arg / 1000.f);

	glutTimerFunc(arg, timerFunction, arg);
	glutPostRedisplay();
}

void drawScene()
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	Game::onDraw();

	if (gPauseSoftware)
		Game::infoBox("Game is frozen.");

	glutSwapBuffers();
}

void resize (int w, int h)
{
	Game::onResize(w, h);
	gWidth = w; gHeight = h;
}

void setup()
{
	std::cout << "Snowman Fight Interaction:\n";
	std::cout << "Use the mouse to navigate through the menu until the game starts.\n";
	std::cout << "Inside game, it is possible to press 'Enter' to access the game menu.\n";
	std::cout << "The game menu contains tabs that allows the user to customize the inputs.\n";
	std::cout << "Change the controls to whatever pleases you and move your snowman.\n";
	std::cout << "The goal is to hit balloons, and it can be played with two players.\n";
	std::cout << "Just keep in mind that the inputs for player 2 are not set by default.\n";
	std::cout << "After starting a 2-player game, set the inputs for player 2 in the menu.\n";
	std::cout << "Settings are saved to a file and automatically loaded.\n";

	Game::startUp();
	Game::setActiveComponent(new IntroScreen, true);
	TextureLoader::LoadTextures();
}

void specialKeyInputUp(int key, int x, int y)
{
	Game::onSpecialKeyUp(key);
}

void specialKeyInput(int key, int x, int y)
{
	if (gPauseSoftware)
		return;

	Game::onSpecialKeyDown(key);
}

void keyInputUp(unsigned char key, int x, int y)
{
	Game::onKeyUp(key);
}

void keyInput(unsigned char key, int x, int y)
{
	if (key == 27) // ESC
		Game::exit();

	if (key == gPauseSoftwareKey)
		gPauseSoftware = !gPauseSoftware;

	if (gPauseSoftware)
		return;
	else
	{
		int x, y;
		Game::getMousePosition(x, y);
		Game::onMouseMove(x, y);
	}

	Game::onKeyDown(key);
}

void mouseInput(int button, int state, int x, int y)
{
	if (gPauseSoftware)
		return;

	Game::onMouseClick(button, state, x, gHeight - y);
}

void mouseMove(int x, int y)
{
	Game::setMousePosition(x, gHeight - y);

	if (gPauseSoftware)
		return;

	Game::onMouseMove(x, gHeight - y);
}
