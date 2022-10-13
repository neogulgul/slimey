#pragma once

#include "Text.hpp"

struct Button
{
	std::string string;
	sf::Vector2f position;
	sf::RectangleShape shape;
	sf::RectangleShape border;
	sf::FloatRect bounds;

	sf::Sprite* sprite;
	bool isSprite = false;

	bool relativeToView;
	bool active = false;

	Button();
	Button(std::string _string, sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f _position, bool _relativeToView = true);
	Button(sf::Sprite* _sprite, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position, bool _relativeToView = true);

	void update(sf::Vector2f mousePosition);
	void draw(sf::RenderWindow* window, sf::View* view, Text* text);
};
