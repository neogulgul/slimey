#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>

#define LevelVector std::vector<std::vector<sf::Vector3i>>

#define emptyTile    sf::Vector3i(0, 0, 0)
#define spawnTile    sf::Vector3i(4, 0, 3)
#define exitTile     sf::Vector3i(4, 0, 4)
#define sawbladeTile sf::Vector3i(4, 1, 0)
#define turretUp     sf::Vector3i(4, 1, 1)
#define turretDown   sf::Vector3i(4, 1, 2)
#define turretLeft   sf::Vector3i(4, 1, 3)
#define turretRight  sf::Vector3i(4, 1, 4)

namespace fs = std::filesystem;

const unsigned int tilesize        = 16;
const unsigned int horizontalTiles = 16;
const unsigned int verticalTiles   = 16;

const unsigned int viewWidth       = tilesize * horizontalTiles;
const unsigned int viewHeight      = tilesize * verticalTiles;
const unsigned int windowViewScale = 3;
const unsigned int windowWidth     = viewWidth  * windowViewScale;
const unsigned int windowHeight    = viewHeight * windowViewScale;

const unsigned int targetFPS = 60;

const unsigned int sawbladeFrameCount    = 8;
const unsigned int sawbladeFrameDuration = 1;

const float GRAVITY = 0.16;

enum Direction { Up, Down, Left, Right };

enum MouseWheel { Forward = 1, Backward = -1 };

enum Alignment { Start, Center, End };

enum Tileset { Null, Normal, Ice, Bounce, Other };

enum State
{
	SplashScreen,
	ExitScreen,
	MainMenu,
	OptionsScreen,
	LevelEditor,
	StoryLevels,
	CustomLevels,
	LevelPlay,
	LevelClear
};

extern sf::Color                background;
extern sf::Color                foreground;
extern sf::Color inactiveMenuboxBackground;
extern sf::Color   activeMenuboxBackground;
extern sf::Color inactiveMenuboxForeground;
extern sf::Color   activeMenuboxForeground;
extern sf::Color                pauseColor;
// level
extern sf::Color collisionColor;
// editor
extern sf::Color     levelOutlineColor;
extern sf::Color levelCheckerEvenColor;
extern sf::Color  levelCheckerOddColor;
extern sf::Color   levelCrosshairColor;
extern sf::Color  levelRestrictedColor;
extern sf::Color          placeColor;
extern sf::Color          eraseColor;
extern sf::Color       selectionTilesetBackgroundColor;
extern sf::Color          selectionTilesetOutlineColor;
extern sf::Color selectionTilesetSelectionOutlineColor;

void toggle(bool &boolean);
std::string boolString(bool boolean);
bool pressing(sf::Keyboard::Key key);
bool pressing(sf::Mouse::Button button);
void handlePress(bool pressing, bool &press, bool &pressed);
sf::Vector2f relativeViewPosition(sf::View* view, sf::Vector2f position);
sf::Vector2f align(sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position);
LevelVector getStoryLevelVector(int index);
LevelVector getCustomLevelVector(std::string levelName);
