#pragma once

#include "Global.hpp"

struct Transition
{
	sf::RenderWindow *window;
	sf::View *view;

	sf::RectangleShape shape;

	State destination;
	State *gameState;

	bool transitioning = false;
	bool atDestination = false;

	int alpha = 0;

	float opacity          = 0;
	float opacityIncrement = 0.03;

	// used in between transitions
	int sleepTimer  = 0;
	int sleepFrames = 15;
	bool sleeping = false;

	Transition();
	Transition(sf::RenderWindow *_window, sf::View *_view, State &_gameState);
	void to(State state);
	void changeState();
	void reset();
	void update();
	void draw();
};
