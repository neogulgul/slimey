#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/MenuBox.hpp"

MenuBox::MenuBox(sf::Font font, int fontSize, std::string string, int x, int y, bool centerX, bool centerY, int width, int height, int padding) {
	this->font = font;
	this->fontSize = fontSize;
	this->string = string;
	this->x = x;
	this->y = y;
	this->centerX = centerX;
	this->centerY = centerY;
	this->width = width;
	this->height = height;
	this->padding = padding;
}

void MenuBox::draw(sf::RenderWindow &window, sf::View &view, bool active) {
	sf::Color outlineColor;
	if (active) {
		outlineColor = sf::Color(sf::Color::Green);
	} else {
		outlineColor = sf::Color(sf::Color(0, 0, 0));
	}
	textBox(window, view, this->font, this->fontSize, this->string, this->x, this->y, this->centerX, this->centerY, this->width, this->height, this->padding, sf::Color(101, 115, 146), outlineColor);
}
