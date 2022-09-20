#include <SFML/Graphics.hpp>

#include "headers/Global.hpp"
#include "headers/Menubox.hpp"

Menubox::Menubox(State _destination, std::string _string, sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position)
{
	destination = _destination;
	string      = _string;
	shape.setOutlineThickness(-1);
	shape.setSize(size);
	shape.setPosition(align(size, horizontalAlignment, verticalAlignment, position));
	bounds = shape.getGlobalBounds();
}

void Menubox::action()
{
	/*
	empty by default
	used by derived structs
	*/
}

void Menubox::draw(sf::RenderWindow *window, Text *text)
{
	sf::Vector2f center;
	center.x = bounds.left + bounds.width / 2;
	center.y = bounds.top + bounds.height / 2;

	if (active)
	{
		shape.setFillColor   (activeMenuboxBackground);
		shape.setOutlineColor(activeMenuboxForeground);
	}
	else
	{
		shape.setFillColor   (inactiveMenuboxBackground);
		shape.setOutlineColor(inactiveMenuboxForeground);
	}

	window->draw(shape);

	if (active) {
		text->draw(string, Center, Center, center, activeMenuboxForeground, sf::Vector2f(activeScale, activeScale));
	}
	else {
		text->draw(string, Center, Center, center, inactiveMenuboxForeground);
	}
}
