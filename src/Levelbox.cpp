#include "headers/Levelbox.hpp"

#define         destination LevelPlay
#define           storySize {24, 24}
#define          customSize {114, 24}
#define horizontalAlignment Center
#define   verticalAlignment Center

Levelbox::Levelbox(Level* _level, unsigned int index, sf::Vector2f position, bool _locked)
: Menubox(destination, std::to_string(index), storySize, horizontalAlignment, verticalAlignment, position)
{
	level = _level;
	locked = _locked;
}

Levelbox::Levelbox(Level* _level, std::string string, sf::Vector2f position)
: Menubox(destination, string, customSize, horizontalAlignment, verticalAlignment, position)
{
	level = _level;
	relativeToView = false;
	customLevelName = string;
	custom = true;
}

void Levelbox::action()
{
	if (custom)
	{
		level->loadLevel(getCustomLevelVector(customLevelName), CustomLevels);
	}
	else
	{
		int index = std::atoi(string.c_str()) - 1;
		level->loadLevel(getStoryLevelVector(index), index);
	}
}
