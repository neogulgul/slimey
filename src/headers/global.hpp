#pragma once

// constants
const int fontsize = 8;
const int FPS = 60;
const int tilesize = 16;
const int viewWidth = tilesize * 16;
const int viewHeight = tilesize * 16;
const int windowScale = 3;
const float gravity = 0.16;

// colors
extern sf::Color background;
extern sf::Color altBackground;
extern sf::Color foreWall;
extern sf::Color backWall;
extern sf::Color playerWall;
extern sf::Color danger;
extern sf::Color ice;
extern sf::Color levelExit;

// functions
bool collision(float x1, float y1, int width1, int height1, float x2, float y2, int width2, int height2);
void updateView(sf::View &view, float xTarget, float yTarget, sf::Vector2u mapSize, bool instant);
