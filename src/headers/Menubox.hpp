#pragma once

#include "Text.hpp"

struct Menubox
{
	State destination;
	bool active = false;

	float activeScale = 1.2;

	std::string string;

	sf::Vector2f position;

	sf::RectangleShape shape;
	sf::RectangleShape border;
	sf::FloatRect bounds;

	Menubox(State _destination, std::string _string, sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f _position);

	virtual void action();
	void draw(sf::RenderWindow *window, sf::View *view, Text *text);
};
