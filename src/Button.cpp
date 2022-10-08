#include <SFML/Graphics.hpp>

#include "headers/Button.hpp"

#define activeScale 1.2
#define borderSize 2

Button::Button() {}

Button::Button(std::string _string, sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f _position, bool _relativeToView)
{
	string   = _string;
	position = align(size, horizontalAlignment, verticalAlignment, {_position.x + borderSize, _position.y + borderSize});
	relativeToView = _relativeToView;
	shape.setOutlineThickness(borderSize);
	shape.setOutlineColor(sf::Color::Transparent);
	shape.setSize({size.x - borderSize * 2, size.y - borderSize * 2});
}

void Button::draw(sf::RenderWindow *window, sf::View *view, Text *text)
{
	if (relativeToView)
	{
		shape.setPosition(relativeViewPosition(*view, position));
	}
	else
	{
		shape.setPosition(position);
	}
	bounds = shape.getGlobalBounds();

	sf::Vector2f shapeCenter;
	shapeCenter.x = bounds.left + bounds.width  / 2;
	shapeCenter.y = bounds.top  + bounds.height / 2;

	if (active)
	{
		shape .setFillColor(activeMenuboxBackground);
		border.setFillColor(activeMenuboxForeground);
	}
	else
	{
		shape .setFillColor(inactiveMenuboxBackground);
		border.setFillColor(inactiveMenuboxForeground);
	}

	window->draw(shape);

	if (active)
	{
		text->draw(string, Center, Center, shapeCenter, activeMenuboxForeground, {activeScale, activeScale});
	}
	else
	{
		text->draw(string, Center, Center, shapeCenter, inactiveMenuboxForeground);
	}

	// drawing border sides
	// top
	border.setSize({bounds.width - borderSize * 2, borderSize});
	border.setPosition(bounds.left + borderSize, bounds.top);
	window->draw(border);
	// bottom
	border.setSize({bounds.width - borderSize * 2, borderSize});
	border.setPosition(bounds.left + borderSize, bounds.top + bounds.height - borderSize);
	window->draw(border);
	// left
	border.setSize({borderSize, bounds.height - borderSize * 2});
	border.setPosition(bounds.left, bounds.top + borderSize);
	window->draw(border);
	// right
	border.setSize({borderSize, bounds.height - borderSize * 2});
	border.setPosition(bounds.left + bounds.width - borderSize, bounds.top + borderSize);
	window->draw(border);

	// drawing border corners
	border.setSize({borderSize, borderSize});
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
