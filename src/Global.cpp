#include <fstream>

#include "headers/Global.hpp"
#include "headers/StoryLevels.hpp"

sf::Color               background( 66,  76, 110);
sf::Color               foreground(255, 255, 255);
sf::Color inactiveButtonBackground(146, 161, 185);
sf::Color   activeButtonBackground( 90, 197,  79);
sf::Color   lockedButtonBackground( 48,  48,  64);
sf::Color inactiveButtonForeground(101, 115, 146);
sf::Color   activeButtonForeground( 12,  46,  68);
sf::Color   lockedButtonForeground( 32,  32,  48);
sf::Color               pauseColor(  0,   0,   0, 191);
// level
sf::Color collisionColor(0x4f37b3ee);
// editor
sf::Color          levelOutlineColor(0x306bbdff);
sf::Color      levelCheckerEvenColor(0x53a5d9ff);
sf::Color       levelCheckerOddColor(0x73b6dfff);
sf::Color        levelCrosshairColor(0x306bbdaa);
sf::Color       levelRestrictedColor(255,  23,  68, 191);
sf::Color                 placeColor(  0, 255,   0,  63);
sf::Color                 eraseColor(255,   0,   0,  63);
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

void createSavedata()
{
	if (fs::is_regular_file("savedata"))
	{
		fs::remove("savedata");
	}

	if (!fs::exists("savedata"))
	{
		fs::create_directory("savedata");
	}
}

void createSavedataFile(std::string path)
{
	if (!fs::is_directory("savedata")) { createSavedata(); }

	if (!fs::is_regular_file("savedata/" + path))
	{
		if (fs::is_directory("savedata/" + path))
		{
			fs::remove_all("savedata/" + path);
		}

		std::ofstream optionsFile("savedata/" + path);
		optionsFile.close();
	}
}

void createSavedataFolder(std::string path)
{
	if (!fs::is_directory("savedata")) { createSavedata(); }

	if (!fs::is_directory("savedata/" + path))
	{
		if (fs::is_regular_file("savedata/" + path))
		{
			fs::remove("savedata/" + path);
		}

		fs::create_directory("savedata/" + path);
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

LevelVector getStoryLevelVector(int index)
{
	return *storyLevels[index];
}

LevelVector getCustomLevelVector(std::string levelName)
{
	LevelVector customLevel;

	std::ifstream levelStream;
	levelStream.open("savedata/custom_levels/" + levelName + ".txt");
	std::stringstream stream;
	int currentRow = 0;
	char character;
	/*
		i wrote this code a long time ago but had to revisit it and change some stuff
		added some additional comments to try and make sense of the code i had previously written
	*/
	while (levelStream.get(character))
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
			customLevel.push_back({});
		}
		// keeping track of which row we are on
		else if (stream.str() == " }")
		{
			currentRow++;
		}
		// seeing if we have come across a pattern like {x,x,x} where x can be any digit (0-9)
		// this represents one tile on the level and so we push that as a sf::Vector3i
		// to the vector at the position of the row we are currently on
		else if (stream.str().length() > 6 && stream.str()[0] == '{' && stream.str()[6] == '}')
		{
			customLevel.at(currentRow).push_back(sf::Vector3i(stream.str()[1] - '0', stream.str()[3] - '0', stream.str()[5] - '0'));
			stream.str("");
		}
	}

	return customLevel;
}

int getLevelsCleared()
{
	int clearedLevels = 0;
	if (fs::is_regular_file("savedata/levels_cleared.txt"))
	{
		std::ifstream fileStream;
		fileStream.open("savedata/levels_cleared.txt");
		std::string line;
		std::getline(fileStream, line);
		if (line.length() > 0)
		{
			clearedLevels = atoi(line.c_str());
			if (clearedLevels < 0)
			{
				clearedLevels = 0;
			}
		}
	}
	else
	{
		createSavedataFile("levels_cleared.txt");
		std::ofstream levelsCleared("savedata/levels_cleared.txt");
		levelsCleared << 0;
		levelsCleared.close();
	}

	return clearedLevels;
}
