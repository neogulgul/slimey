#pragma once

#include "Global.hpp"
#include "State.hpp"

struct Transition
{
	int transitionSpeed       = 2;
	int transitionTimer       = 0;
	int transitionFrames      = 128;
	int transitionDelayTimer  = 0;
	int transitionDelayFrames = 32;

	sf::RectangleShape square;

	Direction direction;
	enum Type { fade, spiral };
	Type type;
	enum Way { inward, outward };
	Way way = inward;

	State destination;

	bool ongoing = false;
	bool inwardComplete = false;
	bool outwardComplete = false;

	Transition(Type type);
	void changeType(Type type);
	void to(State destination);
	void reset();
	void fadeAnimation(sf::RenderWindow &window, sf::View view);
	void spiralAnimation(sf::RenderWindow &window, sf::View view);
	void draw(sf::RenderWindow &window, sf::View view);
};
