#pragma once

#include "StoryLevels.hpp"
#include "Level.hpp"
#include "Menubox.hpp"

#define levelboxSpacing 36

struct Levelbox : Menubox
{
	Level* level;

	std::string customLevelName; // only used if custom
	bool custom = false;

	Levelbox(Level* _level, unsigned int index, sf::Vector2f position);
	Levelbox(Level* _level, std::string string, sf::Vector2f position);

	virtual void action();
};
