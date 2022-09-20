#include <SFML/Graphics.hpp>

#include "headers/Global.hpp"

sf::Color                background( 66,  76, 110);
sf::Color                foreground(255, 255, 255);
sf::Color inactiveMenuboxBackground(146, 161, 185);
sf::Color   activeMenuboxBackground( 90, 197,  79);
sf::Color inactiveMenuboxForeground(101, 115, 146);
sf::Color   activeMenuboxForeground( 12,  46,  68);
sf::Color                pauseColor(  0,   0,   0, 191);

void toggle(bool &boolean)
{
	if (boolean)
	{ boolean = false; }
	else
	{ boolean = true; }
}

bool pressing(sf::Keyboard::Key key)
{
	if (sf::Keyboard::isKeyPressed(key))
	{ return true; }

	return false;
}

bool pressing(sf::Mouse::Button button)
{
	if (sf::Mouse::isButtonPressed(button))
	{ return true; }

	return false;
}

sf::Vector2f align(sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position)
{
	switch (horizontalAlignment)
	{
		case Start:
			position.x -= 0;
			break;

		case Center:
			position.x -= size.x / 2;
			break;

		case End:
			position.x -= size.x;
			break;
	}

	switch (verticalAlignment)
	{
		case Start:
			position.y -= 0;
			break;

		case Center:
			position.y -= size.y / 2;
			break;

		case End:
			position.y -= size.y;
			break;
	}

	return position;
}
