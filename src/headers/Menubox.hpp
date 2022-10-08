#pragma once

#include "Button.hpp"

struct Menubox : Button
{
	State destination;

	Menubox(State _destination, std::string string, sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position, bool relativeToView = true);

	virtual void action();
};
