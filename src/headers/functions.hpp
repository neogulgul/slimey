#pragma once

bool collision(float x1, float y1, int width1, int height1, float x2, float y2, int width2, int height2);
void adjustView(sf::View &view, float xTarget, float yTarget, sf::Vector2u mapSize, bool instant);
