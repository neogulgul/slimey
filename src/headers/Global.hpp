#pragma once

#define emptyTile    sf::Vector3i(0, 0, 0)
#define spawnTile    sf::Vector3i(4, 0, 3)
#define exitTile     sf::Vector3i(4, 0, 4)
#define sawbladeTile sf::Vector3i(4, 1, 0)
#define turretUp     sf::Vector3i(4, 1, 1)
#define turretDown   sf::Vector3i(4, 1, 2)
#define turretLeft   sf::Vector3i(4, 1, 3)
#define turretRight  sf::Vector3i(4, 1, 4)

const unsigned int tilesize        = 16;
const unsigned int horizontalTiles = 16;
const unsigned int verticalTiles   = 16;

const unsigned int viewWidth    = tilesize * horizontalTiles;
const unsigned int viewHeight   = tilesize * verticalTiles;
const unsigned int windowWidth  = viewWidth  * 2;
const unsigned int windowHeight = viewHeight * 2;

const unsigned int FPS = 60;

const float GRAVITY = 0.16;

enum Direction { Up, Down, Left, Right };

enum Alignment { Start, Center, End };

enum Tileset { Null, Normal, Ice, Bounce, Other };

enum State
{
	SplashScreen,
	ExitScreen,
	MainMenu,
	Options,
	Editor,
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

void toggle(bool &boolean);
bool pressing(sf::Keyboard::Key key);
bool pressing(sf::Mouse::Button button);
sf::Vector2f relativeViewPosition(sf::View view, sf::Vector2f position);
sf::Vector2f align(sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position);
