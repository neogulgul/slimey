#pragma once

#include "DefaultMaps.hpp"
#include "Global.hpp"
#include "Menubox.hpp"
#include "Level.hpp"
#include "Levelbox.hpp"
#include "Sprites.hpp"
#include "Text.hpp"
#include "Transition.hpp"

struct Game
{
	sf::RenderWindow *window;
	sf::View *view;

	sf::Cursor cursor;

	Level level;
	Sprites sprites;
	Text text;
	Transition transition;

	sf::Vector2f mousePosition;

	// mouse
	bool leftClick   = false;
	bool leftClicked = false;

	std::vector<Menubox *> menu;
	bool hoveringMenubox = false;

	State lastState;
	State     state;

	/*
	these are used when drawing some text in level select
	couldn't come up with any good names for them
	the second one can vary so it isn't assigned here
	*/
	float verticalPositionOfDefaultSection = viewHeight / 4 + 32;
	float verticalPositionOfCustomSection;

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
	void updateLevelSelect();
	void updateLevelPlay();
	void updateLevelClear();
	void updateState();

	void drawSplashScreen();
	void drawExitScreen();
	void drawMainMenu();
	void drawOptions();
	void drawLevelSelect();
	void drawLevelPlay();
	void drawLevelClear();
	void drawState();
	

	void update();
	void draw();
};
