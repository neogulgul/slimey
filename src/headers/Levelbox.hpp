#pragma once

#include "DefaultMaps.hpp"
#include "Level.hpp"
#include "Menubox.hpp"

struct Levelbox : Menubox
{
	Level* level;

	std::string customMapName; // only used if custom
	bool custom = false;

	Levelbox(Level &_level, unsigned int index, sf::Vector2f position);
	Levelbox(Level &_level, std::string string, sf::Vector2f position);

	virtual void action();
};
