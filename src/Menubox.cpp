#include <SFML/Graphics.hpp>

#include "headers/Menubox.hpp"

#define borderSize 2

Menubox::Menubox(State _destination, std::string _string, sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f _position)
{
	destination = _destination;
	string      = _string;
	position    = align(size, horizontalAlignment, verticalAlignment, sf::Vector2f(_position.x + borderSize, _position.y + borderSize));
	shape.setOutlineThickness(borderSize);
	shape.setOutlineColor(sf::Color::Transparent);
	shape.setSize(sf::Vector2f(size.x - borderSize * 2, size.y - borderSize * 2));
}

void Menubox::action()
{
	/*
	empty by default
	used by derived structs
	*/
}

void Menubox::draw(sf::RenderWindow *window, sf::View *view, Text *text)
{
	shape.setPosition(relativeViewPosition(*view, position));
	bounds = shape.getGlobalBounds();

	sf::Vector2f shapeCenter;
	shapeCenter.x = bounds.left + bounds.width / 2;
	shapeCenter.y = bounds.top + bounds.height / 2;

	if (active)
	{
		shape.setFillColor (activeMenuboxBackground);
		border.setFillColor(activeMenuboxForeground);
	}
	else
	{
		shape.setFillColor (inactiveMenuboxBackground);
		border.setFillColor(inactiveMenuboxForeground);
	}

	window->draw(shape);

	if (active)
	{
		text->draw(string, Center, Center, shapeCenter, activeMenuboxForeground, sf::Vector2f(activeScale, activeScale));
	}
	else
	{
		text->draw(string, Center, Center, shapeCenter, inactiveMenuboxForeground);
	}

	// drawing border sides
	// top
	border.setSize(sf::Vector2f(bounds.width - borderSize * 2, borderSize));
	border.setPosition(bounds.left + borderSize, bounds.top);
	window->draw(border);
	// bottom
	border.setSize(sf::Vector2f(bounds.width - borderSize * 2, borderSize));
	border.setPosition(bounds.left + borderSize, bounds.top + bounds.height - borderSize);
	window->draw(border);
	// left
	border.setSize(sf::Vector2f(borderSize, bounds.height - borderSize * 2));
	border.setPosition(bounds.left, bounds.top + borderSize);
	window->draw(border);
	// right
	border.setSize(sf::Vector2f(borderSize, bounds.height - borderSize * 2));
	border.setPosition(bounds.left + bounds.width - borderSize, bounds.top + borderSize);
	window->draw(border);

	// drawing border corners
	border.setSize(sf::Vector2f(borderSize, borderSize));
	// top left
	border.setPosition(bounds.left + borderSize                      - borderSize / 2.f, bounds.top + borderSize                       - borderSize / 2.f);
	window->draw(border);
	// top right
	border.setPosition(bounds.left + bounds.width - borderSize * 2.f + borderSize / 2.f, bounds.top + borderSize                       - borderSize / 2.f);
	window->draw(border);
	// bottom left
	border.setPosition(bounds.left + borderSize                      - borderSize / 2.f, bounds.top + bounds.height - borderSize * 2.f + borderSize / 2.f);
	window->draw(border);
	// bottom right
	border.setPosition(bounds.left + bounds.width - borderSize * 2.f + borderSize / 2.f, bounds.top + bounds.height - borderSize * 2.f + borderSize / 2.f);
	window->draw(border);
}
