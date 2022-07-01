#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/MenuBox.hpp"

MenuBox::MenuBox(sf::Font font, int fontSize, std::string string, int x, int y, bool centerX, bool centerY, int width, int height, int padding) {
	this->font     = font;
	this->fontSize = fontSize;
	this->string   = string;
	this->x = x;
	this->y = y;
	this->centerX = centerX;
	this->centerY = centerY;
	this->width   = width;
	this->height  = height;
	this->padding = padding;
	this->backgroundColor       = sf::Color(146, 161, 185);
	this->outlineColor          = sf::Color(101, 115, 146);
	this->activeBackgroundColor = sf::Color(90 , 197, 79 );
	this->activeOutlineColor    = sf::Color(12 , 46 , 68 );
}

void MenuBox::draw(sf::RenderWindow &window, sf::View &view, bool active) {
	sf::Color backgroundColor;
	sf::Color outlineColor;
	if (active) {
		backgroundColor = this->activeBackgroundColor;
		outlineColor    = this->activeOutlineColor;
	} else {
		backgroundColor = this->backgroundColor;
		outlineColor    = this->outlineColor;
	}
	textBox(window, view, this->font, this->fontSize, this->string, this->x, this->y, this->centerX, this->centerY, false, false, this->width, this->height, this->padding, outlineColor, backgroundColor, outlineColor);
}
