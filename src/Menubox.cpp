#include <SFML/Graphics.hpp>

#include "headers/Menubox.hpp"

Menubox::Menubox(State _destination, std::string _string, sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f _position)
: Button(_string, size, horizontalAlignment, verticalAlignment, _position)
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
