#pragma once

#include "Global.hpp"

struct Text
{
	sf::RenderWindow *window;

	sf::Text text;

	sf::Font font;
	int fontSize = 8;

	Text();
	Text(sf::RenderWindow *_window);

	void draw(std::string string, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position, sf::Color color = foreground, sf::Vector2f scale = {1, 1});
	void draw(std::string string, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position, sf::Vector2f scale);
};
