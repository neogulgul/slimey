#pragma once

// constants
const int FPS = 60;

const float gravity = 0.16;

const int   bulletRadius   = 2;
const float bulletVelocity = 1;

const int tilesize        = 16;
const int horizontalTiles = 16;
const int verticalTiles   = 16;
const int viewWidth       = tilesize * horizontalTiles;
const int viewHeight      = tilesize * verticalTiles;
const int windowScale     = 3;

const int smallFontSize = 8;
const int largeFontSize = 16;

enum Direction { up, down, left, right };

enum State { startScreenState, levelSelectState, levelClearState, gameState };

// colors
extern sf::Color backgroundColor;
extern sf::Color textColor;
extern sf::Color foreWall;
extern sf::Color backWall;
extern sf::Color playerWall;
extern sf::Color sawblade;
extern sf::Color ice;
extern sf::Color levelExit;
extern sf::Color bounce;
extern sf::Color turretUp;
extern sf::Color turretDown;
extern sf::Color turretLeft;
extern sf::Color turretRight;

// functions
bool collision(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);
bool validCollisionTile(sf::Image mapImage, sf::Vector2u mapSize, int xCord, int yCord);
void updateView(sf::View &view, float xTarget, float yTarget, sf::Vector2u mapSize, bool instant);
void textBox(sf::RenderWindow &window, sf::View &view, sf::Font font, int fontSize, std::string string, int x, int y, bool centerX, bool centerY, bool rightOrigin, bool bottomOrigin, int width, int height, int padding, sf::Color textColor, sf::Color backgroundColor, sf::Color outlineColor);
