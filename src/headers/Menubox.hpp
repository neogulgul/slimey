#pragma once

#include "Text.hpp"

struct Menubox
{
	State destination;
	bool active = false;

	float activeScale = 1.2;

	std::string string;

	sf::RectangleShape shape;
	sf::FloatRect bounds;

	Menubox(State _destination, std::string _string, sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position);

	virtual void action();
	void draw(sf::RenderWindow *window, Text *text);
};
