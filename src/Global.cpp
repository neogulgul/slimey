#include <fstream>

#include "headers/Global.hpp"

sf::Color                background( 66,  76, 110);
sf::Color                foreground(255, 255, 255);
sf::Color inactiveMenuboxBackground(146, 161, 185);
sf::Color   activeMenuboxBackground( 90, 197,  79);
sf::Color inactiveMenuboxForeground(101, 115, 146);
sf::Color   activeMenuboxForeground( 12,  46,  68);
sf::Color                pauseColor(  0,   0,   0, 191);
// level
sf::Color collisionColor(0x4f37b3ee);
// editor
sf::Color          mapOutlineColor(0x306bbdff);
sf::Color      mapCheckerEvenColor(0x53a5d9ff);
sf::Color       mapCheckerOddColor(0x73b6dfff);
sf::Color        mapCrosshairColor(0x306bbdaa);
sf::Color       mapRestrictedColor(255,  23,  68, 191);
sf::Color               placeColor(  0, 255,   0,  63);
sf::Color               eraseColor(255,   0,   0,  63);
sf::Color       selectionTilesetBackgroundColor(0xd9c797ff);
sf::Color          selectionTilesetOutlineColor(0xdaba87ff);
sf::Color selectionTilesetSelectionOutlineColor(0x5ac54faa);



void toggle(bool &boolean)
{
	if (boolean)
	{
		boolean = false;
	}
	else
	{
		boolean = true;
	}
}

std::string boolString(bool boolean)
{
	if (boolean)
	{
		return "true";
	}
	return "false";
}

bool pressing(sf::Keyboard::Key key)
{
	return sf::Keyboard::isKeyPressed(key);
}

bool pressing(sf::Mouse::Button button)
{
	return sf::Mouse::isButtonPressed(button);
}

void handlePress(bool pressing, bool &press, bool &pressed)
{
	press = false;

	if (pressing)
	{
		if (!pressed)
		{
			pressed = true;
			press = true;
		}
	}
	else
	{
		pressed = false;
	}
}


sf::Vector2f relativeViewPosition(sf::View* view, sf::Vector2f position)
{
	return {view->getCenter().x - view->getSize().x / 2 + position.x, view->getCenter().y - view->getSize().y / 2 + position.y};
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

mapVector getCustomMapVector(std::string mapName)
{
	mapVector customMap;

	std::ifstream mapStream;
	mapStream.open("custom_maps/" + mapName + ".txt");
	std::stringstream stream;
	int currentRow = 0;
	char character;
	/*
		i wrote this code a long time ago but had to revisit it and change some stuff
		added some additional comments to try and make sense of the code i had previously written
	*/
	while (mapStream.get(character))
	{
		// reseting the string stream whenever we hit a left-facing curly bracket
		if (character == '{')
		{
			stream.str("");
		}
		stream << character;
		// pushing a new vector whenever we come across a left-facing curly bracket plus a blank space right after
		if (stream.str() == "{ ")
		{
			customMap.push_back({});
		}
		// keeping track of which row we are on
		else if (stream.str() == " }")
		{
			currentRow++;
		}
		// seeing if we have come across a pattern like {x,x,x} where x can be any digit (0-9)
		// this represents one tile on the map and so we push that as a sf::Vector3i
		// to the vector at the position of the row we are currently on
		else if (stream.str().length() > 6 && stream.str()[0] == '{' && stream.str()[6] == '}')
		{
			customMap.at(currentRow).push_back(sf::Vector3i(stream.str()[1] - '0', stream.str()[3] - '0', stream.str()[5] - '0'));
			stream.str("");
		}
	}

	return customMap;
}
