#pragma once

const unsigned int targetFPS = 60;

const unsigned int tilesize        = 16;
const unsigned int horizontalTiles = 16;
const unsigned int verticalTiles   = 16;
const unsigned int viewWidth       = horizontalTiles * tilesize;
const unsigned int viewHeight      = verticalTiles   * tilesize;
const unsigned int windowScale     = 3;
const unsigned int windowWidth     = viewWidth  * windowScale;
const unsigned int windowHeight    = viewHeight * windowScale;

const float GRAVITY = 0.16;

enum Direction
{
	up,
	down,
	left,
	right
};

enum State
{
	STATE_SplashScreen,
	STATE_ExitScreen,
	STATE_MainMenu,
	STATE_Options,
	STATE_FileSelect,
	STATE_LevelSelect,
	STATE_LevelPlay,
	STATE_LevelClear,
	STATE_Cutscene
};

bool collision(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);
bool validCollisionTile(sf::Image mapImage, sf::Vector2u mapSize, int xCord, int yCord);
void updateView(sf::View &view, float xTarget, float yTarget, sf::Vector2u mapSize, bool instant);
void textBox(sf::RenderWindow &window, sf::View &view, sf::Font font, int fontSize, std::string string, int x, int y, bool centerX, bool centerY, bool rightOrigin, bool bottomOrigin, int width, int height, int padding, sf::Color textColor, sf::Color backgroundColor, sf::Color outlineColor);
