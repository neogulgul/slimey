#pragma once

// constants
const int FPS = 60;
const int tilesize = 16;
const int viewWidth = tilesize * 16;
const int viewHeight = tilesize * 16;
const float gravity = 0.16;

// colors
extern sf::Color background;
extern sf::Color playerColor;
extern sf::Color foreWall;
extern sf::Color backWall;

// functions
bool collision(float x1, float y1, int width1, int height1, float x2, float y2, int width2, int height2);
void updateView(sf::View &view, float xTarget, float yTarget, sf::Vector2u mapSize, bool instant);
