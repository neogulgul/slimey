#include <SFML/Graphics.hpp>

#include "headers/Text.hpp"

void loadPixelatedFont(sf::Font &font, int fontSize, std::string fontPath)
{
	font.loadFromFile(fontPath);
	sf::Texture& fontTexture = const_cast<sf::Texture&>(font.getTexture(fontSize));
	fontTexture.setSmooth(false);
}

Text::Text() {}

Text::Text(sf::RenderWindow *_window)
{
	window = _window;

	loadPixelatedFont(font, fontSize, "assets/fonts/FiveByFive.ttf");
}

void Text::draw(std::string string, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position, sf::Color color, sf::Vector2f scale)
{
	text.setString(string);
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setFillColor(color);
	text.setScale(scale);
	text.setOrigin(0, text.getCharacterSize() / 2 - 1);

	sf::FloatRect textBounds = text.getGlobalBounds();
	sf::Vector2f size(textBounds.width, textBounds.height);

	position = align(size, horizontalAlignment, verticalAlignment, position);

	text.setPosition(position);

	window->draw(text);
}

void Text::draw(std::string string, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position, sf::Vector2f scale)
{
	draw(string, horizontalAlignment, verticalAlignment, position, foreground, scale);
}
