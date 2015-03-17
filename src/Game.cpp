#include "Game.h"
#include "Utils.h"
#include "Drawing.h"
#include <ctime>
#include "glutInclude.h"

Game Game::globalGame;

const int GAME_VERSION = 2;

Game::Game()
{
	m_keepAspectRatio = true;
	m_activeComponent = 0;
	m_width = m_height = -99;
	m_aspectRatio = 1.f;
	m_preferredHeight = m_preferredWidth = 500;

	if (!globalGame.loadGameSettings())
	{
		setDefaultPlayerPreferences(globalGame.m_player1);
		setDefaultPlayerPreferences(globalGame.m_player2);
		globalGame.m_player2.inputs.clear();
		m_game_prefs.booleans = GameScreen::BIT_DISPLAY_MINIMAP |
			GameScreen::BIT_DISPLAY_SNOW | GameScreen::BIT_HORIZONTAL_SPLIT;
	}

	m_game_prefs.player1_minimap_color[0] = 47;
	m_game_prefs.player1_minimap_color[1] = 75;
	m_game_prefs.player1_minimap_color[2] = 189;

	m_game_prefs.player2_minimap_color[0] = 240;
	m_game_prefs.player2_minimap_color[1] = 10;
	m_game_prefs.player2_minimap_color[2] = 50;
}

void Game::startUp()
{
	glClearColor(1.f, 1.f, 0.f, 1.0f);
	glClearDepth(1.f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	srand(time(NULL));
	rand(); rand(); rand();
}

void Game::getPreferredWindowSize(int & width, int & height)
{
	width = globalGame.m_preferredWidth;
	height = globalGame.m_preferredHeight;
}

void Game::setMousePosition(int x, int y)
{
	globalGame.m_mouse_x = x;
	globalGame.m_mouse_y = y;
}

void Game::getMousePosition(int & x, int & y)
{
	x = globalGame.m_mouse_x;
	y = globalGame.m_mouse_y;
}

void Game::getWindowSize(int & w, int & h)
{
	w = globalGame.m_width;
	h = globalGame.m_height;
}

void Game::exit()
{
	globalGame.saveGameSettings();
	::exit(0);
}

void Game::applyViewport()
{
	glViewport(0, 0, globalGame.m_width, globalGame.m_height);
}

void Game::setActiveComponent(GuiComponent* gui, bool deleteAfterUse)
{
	if (globalGame.m_deleteComponent && globalGame.m_activeComponent)
		delete globalGame.m_activeComponent;

	globalGame.m_activeComponent = gui;
	globalGame.m_deleteComponent = deleteAfterUse;

	if (gui)
		gui->onResize(globalGame.m_width, globalGame.m_height);
}

void Game::setOrthoProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.f, globalGame.m_width, 0.f, globalGame.m_height, 0.f, 1.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Game::setPerspectiveProjection(float aspectRatio)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35.f, aspectRatio <= 0.f ? (globalGame.m_keepAspectRatio ? globalGame.m_aspectRatio :
		(float)globalGame.m_width / globalGame.m_height) : aspectRatio, 1.f, 1000.f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Game::setFog(bool b)
{
	float fogColor[] = {.95f, .95f, 1.f, 0.f};

	glHint(GL_FOG_HINT, GL_NICEST);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 20.f);
	glFogf(GL_FOG_END, 28.f);
	glFogf(GL_FOG_DENSITY, 0.002f);

	if (b)
		glEnable(GL_FOG);
	else
		glDisable(GL_FOG);
}

void Game::clearScreen(float r, float g, float b)
{
	glClearColor(r, g, b, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::paintScreen(float r, float g, float b, float a)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	Game::setOrthoProjection();
	glColor4f(r, g, b, a);
	Draw::rectangle2d(globalGame.m_width / 2.f, globalGame.m_height / 2.f, globalGame.m_width + 2, globalGame.m_height + 2, true);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

Player::PlayerPreferences& Game::getPlayerPreferences(bool playerOne)
{
	return playerOne ? globalGame.m_player1 : globalGame.m_player2;
}

GameScreen::GamePreferences& Game::getGamePreferences()
{
	return globalGame.m_game_prefs;
}

void Game::infoBox(std::string info)
{
	setOrthoProjection();
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);

	glColor4f(0.f, 0.f, 0.f, .75f);
	Draw::rectangle2d(globalGame.m_width * .5f, globalGame.m_height * .9f, globalGame.m_width * .6f, 60, true);

	glColor3f(26.f/255, 147.f/255, 217.f/255);
	Draw::wirerectangle2d(globalGame.m_width * .5f, globalGame.m_height * .9f, globalGame.m_width * .6f - 8, 52, true);

	glColor3f(1.f, 1.f, 1.f);
	Draw::text(info, globalGame.m_width * .5f, globalGame.m_height * .9f, true, true);

	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}

void Game::onKeyDown(unsigned char key)
{
	if (globalGame.m_activeComponent)
		globalGame.m_activeComponent->onKeyDown(key);
}

void Game::onKeyUp(unsigned char key)
{
	if (globalGame.m_activeComponent)
		globalGame.m_activeComponent->onKeyUp(key);
}

void Game::onSpecialKeyDown(int key)
{
	if (globalGame.m_activeComponent)
		globalGame.m_activeComponent->onSpecialKeyDown(key);
}

void Game::onSpecialKeyUp(int key)
{
	if (globalGame.m_activeComponent)
		globalGame.m_activeComponent->onSpecialKeyUp(key);
}

void Game::onMouseMove(int x, int y)
{
	if (globalGame.m_activeComponent)
		globalGame.m_activeComponent->onMouseMove(x, y);
}

void Game::onMouseClick(int button, int state, int x, int y)
{
	if (globalGame.m_activeComponent)
		globalGame.m_activeComponent->onMouseClick(button, state, x, y);
}

void Game::onResize(int width, int height)
{
	if (globalGame.m_activeComponent)
		globalGame.m_activeComponent->onResize(width, height);

	if (globalGame.m_width == -99)
		globalGame.m_aspectRatio = (float)width/height;

	globalGame.m_width = width;
	globalGame.m_height = height;
	glViewport (0, 0, width, height);
}

void Game::onUpdate(float interval)
{
	if (globalGame.m_activeComponent)
		globalGame.m_activeComponent->onUpdate(interval);
}

void Game::onDraw()
{
	if (globalGame.m_activeComponent)
		globalGame.m_activeComponent->onDraw();
}

void Game::setDefaultPlayerPreferences(Player::PlayerPreferences& prefs)
{
	prefs.inputs.push_back(Player::KeyInput('w', false));
	prefs.inputs.push_back(Player::KeyInput('s', false));
	prefs.inputs.push_back(Player::KeyInput('d', false));
	prefs.inputs.push_back(Player::KeyInput('a', false));
	prefs.inputs.push_back(Player::KeyInput(GLUT_KEY_UP, true));
	prefs.inputs.push_back(Player::KeyInput(GLUT_KEY_DOWN, true));
	prefs.inputs.push_back(Player::KeyInput(GLUT_KEY_LEFT, true));
	prefs.inputs.push_back(Player::KeyInput(GLUT_KEY_RIGHT, true));
	prefs.inputs.push_back(Player::KeyInput(13, false)); // enter - pause game
	prefs.inputs.push_back(Player::KeyInput(' ', false)); // shoot
	prefs.inputs.push_back(Player::KeyInput('\t', false)); // change camera
	prefs.inputs.push_back(Player::KeyInput('1', false)); // change face

	prefs.cameraMode = 1;
	prefs.aspectRatioMode = 0;
	prefs.snowmanColor[0] = prefs.snowmanColor[1] = prefs.snowmanColor[2] = 1.f;
	prefs.snowmanFaceTexture = "SnowTextures/face1.bmp";
}

#define SAVE_VAR(var) Utils::VectorWriteBytes(bytes, (void*)&var, sizeof(var))
#define LOAD_VAR(var) Utils::FileReadBytes(fileOut, &var, sizeof(var))

void Game::saveGameSettings()
{
	std::vector<unsigned char> bytes;

	SAVE_VAR(GAME_VERSION);
	SAVE_VAR(m_preferredWidth);
	SAVE_VAR(m_preferredHeight);
	SAVE_VAR(m_game_prefs.booleans);
	savePlayerPreferences(bytes, m_player1);
	savePlayerPreferences(bytes, m_player2);

	FILE* fileOut = fopen("game.data", "wb");

	if (!fileOut)
	{
		std::cout << "Error savings game settings. Unable to open game.data file.\n";
		return;
	}

	Utils::FileWriteBytes(fileOut, bytes.data(), sizeof(unsigned char) * bytes.size(), true);
	fclose(fileOut);
}

bool Game::loadGameSettings()
{
	FILE* fileOut = fopen("game.data", "rb");

	if (!fileOut)
		return false;

	int c = fgetc(fileOut);

	if (c == EOF)
	{
		fclose(fileOut);
		return false;
	}
	else
		ungetc(c, fileOut);

	int fileVersion;
	bool success = false;

	try {
		LOAD_VAR(fileVersion);

		if (fileVersion != GAME_VERSION)
		{
			std::cout << "[Warning] Failed to load game settings: file is outdated.\n";
			fclose(fileOut);
			return false;
		}

		LOAD_VAR(m_preferredWidth);
		LOAD_VAR(m_preferredHeight);
		LOAD_VAR(m_game_prefs.booleans);

		m_player1 = loadPlayerPreferences(fileOut);
		m_player2 = loadPlayerPreferences(fileOut);
		success = true;
	} catch (std::string& errorString) {
		std::cout << "[Warning] Failed to load game settings: " << errorString << '\n';
	}

	fclose(fileOut);
	return success;
}

void Game::savePlayerPreferences(std::vector<unsigned char>& data, Player::PlayerPreferences& inputs)
{
	char temp;

	Utils::VectorWriteBytes(data, &inputs.cameraMode, sizeof(inputs.cameraMode));
	Utils::VectorWriteBytes(data, &inputs.aspectRatioMode, sizeof(inputs.aspectRatioMode));
	Utils::VectorWriteBytes(data, inputs.snowmanColor, sizeof(inputs.snowmanColor));

	temp = inputs.snowmanFaceTexture.size();
	Utils::VectorWriteBytes(data, &temp, sizeof(temp));

	if (temp > 0)
		Utils::VectorWriteBytes(data, (void*)inputs.snowmanFaceTexture.c_str(), sizeof(char) * inputs.snowmanFaceTexture.size());

	temp = inputs.inputs.size();
	Utils::VectorWriteBytes(data, &temp, sizeof(temp));

	for (unsigned i = 0; i < inputs.inputs.size(); i++)
	{
		Utils::VectorWriteBytes(data, &inputs.inputs[i].keyValue, sizeof(inputs.inputs[i].keyValue));
		Utils::VectorWriteBytes(data, &inputs.inputs[i].isSpecial, sizeof(inputs.inputs[i].isSpecial));
	}
}

Player::PlayerPreferences Game::loadPlayerPreferences(FILE* file)
{
	Player::PlayerPreferences result;
	unsigned char temp;

	Utils::FileReadBytes(file, &result.cameraMode, sizeof(result.cameraMode));
	Utils::FileReadBytes(file, &result.aspectRatioMode, sizeof(result.aspectRatioMode));
	Utils::FileReadBytes(file, result.snowmanColor, sizeof(result.snowmanColor) / sizeof(*result.snowmanColor));

	Utils::FileReadBytes(file, &temp, sizeof(temp));
	result.snowmanFaceTexture.resize(temp);
	Utils::FileReadBytes(file, (void*)result.snowmanFaceTexture.c_str(), sizeof(unsigned char) * temp);

	Utils::FileReadBytes(file, &temp, sizeof(temp));

	Player::KeyInput input;

	for (int i = 0; i < temp; i++)
	{
		Utils::FileReadBytes(file, &input.keyValue, sizeof(input.keyValue));
		Utils::FileReadBytes(file, &input.isSpecial, sizeof(input.isSpecial));
		result.inputs.push_back(input);
	}

	return result;
}
