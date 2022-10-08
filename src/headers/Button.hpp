#pragma once

#include "Text.hpp"

struct Button
{
	std::string string;
	sf::Vector2f position;
	sf::RectangleShape shape;
	sf::RectangleShape border;
	sf::FloatRect bounds;

	bool relativeToView;
	bool active = false;

	Button();
	Button(std::string _string, sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f _position, bool _relativeToView = true);
	void draw(sf::RenderWindow *window, sf::View *view, Text *text);
};
