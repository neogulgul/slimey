#include <SFML/Graphics.hpp>

#include "headers/Levelbox.hpp"

#define         destination LevelPlay
#define                size sf::Vector2f(24, 24)
#define horizontalAlignment Center
#define   verticalAlignment Center

Levelbox::Levelbox(Level &_level, unsigned int index, sf::Vector2f position)
: Menubox(destination, std::to_string(index), size, horizontalAlignment, verticalAlignment, position)
{
	level = &_level;
}

Levelbox::Levelbox(Level &_level, std::string string, sf::Vector2f position)
: Menubox(destination, string, size, horizontalAlignment, verticalAlignment, position)
{
	level = &_level;
	custom = true;
}

void Levelbox::action()
{
	if (custom)
	{
		mapVector customMap;

		// todo: load custom map

		level->loadMap(customMap);
	}
	else
	{
		int index = std::atoi(string.c_str()) - 1;
		mapVector defaultMap = *defaultMaps[index];

		level->loadMap(defaultMap);
	}
}
