#include <SFML/Graphics.hpp>

#include "headers/Menubox.hpp"

Menubox::Menubox(State _destination, std::string string, sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position, bool relativeToView)
: Button(string, size, horizontalAlignment, verticalAlignment, position, relativeToView)
{
	destination = _destination;
}

void Menubox::action()
{
	/*
	empty by default
	used by derived structs
	*/
}
