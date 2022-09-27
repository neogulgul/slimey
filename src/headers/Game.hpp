#pragma once

#include <random>

#include "DefaultMaps.hpp"
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
	sf::Cursor cursor;

	std::mt19937 rng;

	Level level;
	Sprites sprites;
	Text text;
	Transition transition;

	std::vector<RainbowSlimey> rainbowSlimeys;

	sf::Vector2f mousePosition;

	// mouse
	bool leftClick   = false;
	bool leftClicked = false;

	std::vector<Menubox *> menu;
	bool hoveringMenubox = false;

	State lastState;
	State     state;
	bool changedState = false;

	Game(sf::RenderWindow &_window, sf::View &_view);

	void processKeyboardInput();
	void processMouseInput();

	void createLevelboxes();
	void createMenu();
	void updateMenu();
	void drawMenu();

	void updateCursor();

	void updateSplashScreen();
	void updateExitScreen();
	void updateMainMenu();
	void updateOptions();
	void updateEditor();
	void updateStoryLevels();
	void updateCustomLevels();
	void updateLevelPlay();
	void updateLevelClear();
	void updateState();

	void drawSplashScreen();
	void drawExitScreen();
	void drawMainMenu();
	void drawOptions();
	void drawEditor();
	void drawStoryLevels();
	void drawCustomLevels();
	void drawLevelPlay();
	void drawLevelClear();
	void drawState();
	

	void update();
	void draw();
};
