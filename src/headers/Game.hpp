#pragma once

#include <random>
#include <sstream>

#include "Audio.hpp"
#include "DefaultMaps.hpp"
#include "Editor.hpp"
#include "Global.hpp"
#include "Menubox.hpp"
#include "Level.hpp"
#include "Levelbox.hpp"
#include "Sprites.hpp"
#include "Text.hpp"
#include "Transition.hpp"

struct RainbowSlimey // used on the main menu
{
	sf::Color color;
	sf::Vector2f position;
	float scale;
	int rotation;
	int speed;

	bool passed = false;
	
	RainbowSlimey(sf::Color _color, sf::Vector2f _position, float _scale, int _rotation, int _speed);

	void update();
};

struct Game
{
	sf::RenderWindow *window;
	sf::View *view;
	sf::FloatRect viewport;

	sf::Cursor cursor;
	bool handyCursor = false;

	float frameUpdateDelta = 0.1; // the time between FPS counter update, in seconds
	sf::Clock frameUpdateClock;   // used to only update the FPS counter after a certain amount of time has passed, to make it more readable
	sf::Clock frameClock;         // used to calculate FPS
	sf::Clock displayClock;       // the time it takes the window to display everything drawn which is then subtracted when calculating FPS
	std::stringstream FPS;

	std::mt19937 rng;

	Audio audio;
	Editor editor;
	Level level;
	Sprites sprites;
	Text text;
	Transition transition;

	std::vector<RainbowSlimey> rainbowSlimeys;

	sf::Vector2f mousePosition;

	// mouse
	bool leftClick     = false;
	bool leftClicked   = false;
	bool middleClick   = false;
	bool middleClicked = false;
	bool rightClick    = false;
	bool rightClicked  = false;

	std::vector<Menubox *> menu;

	State lastState;
	State     state;
	bool changedState = false;

	// pause
	sf::RectangleShape pauseShape;
	sf::Keyboard::Key pause = sf::Keyboard::Escape;
	bool paused       = false;
	bool pausePress   = false;
	bool pausePressed = false;

	Game(sf::RenderWindow *_window, sf::View *_view);

	void resetView();
	void updateViewport();

	void processKeyboardInput();
	void processMouseInput();

	void createStoryLevelboxes();
	void createCustomLevelboxes();
	void createMenu();
	void updateMenu();
	void drawMenu();

	void updateCursor();

	void updateFPS();
	void drawFPS();

	void updateSplashScreen();
	void updateExitScreen();
	void updateMainMenu();
	void updateOptions();
	void updateLevelEditor();
	void updateStoryLevels();
	void updateCustomLevels();
	void updateLevelPlay();
	void updateLevelClear();
	void updateState();

	void drawSplashScreen();
	void drawExitScreen();
	void drawMainMenu();
	void drawOptions();
	void drawLevelEditor();
	void drawStoryLevels();
	void drawCustomLevels();
	void drawLevelPlay();
	void drawLevelClear();
	void drawState();

	void update();
	void draw();
};
