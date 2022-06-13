#include <SFML/Graphics.hpp>
#include "headers/global.hpp"

// colors
sf::Color background(66, 76, 110);
sf::Color playerColor(90, 197, 79);
sf::Color foreWall(146, 161, 185);
sf::Color backWall(42, 47, 78);

// functions
bool collision(float x1, float y1, int width1, int height1, float x2, float y2, int width2, int height2) {
	if (x1 > x2 - width1 &&
		x1 < x2 + width2 &&
		y1 > y2 - height1 &&
		y1 < y2 + height2) {
		return true;
	}

	return false;
}

void updateView(sf::View &view, float xTarget, float yTarget, sf::Vector2u mapSize, bool instant) {
	if (instant) {
		view.setCenter(xTarget, yTarget);
	} else {
		const float xStart = view.getCenter().x;
		const float yStart = view.getCenter().y;
		const float xDelta = xTarget - xStart;
		const float yDelta = yTarget - yStart;
		const float xDistance = xDelta * 0.1;
		const float yDistance = yDelta * 0.1;
		float xDestination = xStart + xDistance;
		float yDestination = yStart + yDistance;

		// limiting view
		if (xDestination < viewWidth / 2) {
			xDestination = viewWidth / 2;
		} else if (xDestination > mapSize.x * tilesize - (viewWidth / 2)) {
			xDestination = mapSize.x * tilesize - (viewWidth / 2);
		}
		if (yDestination < viewHeight / 2) {
			yDestination = viewHeight / 2;
		} else if (yDestination > mapSize.y * tilesize - (viewHeight / 2)) {
			yDestination = mapSize.y * tilesize - (viewHeight / 2);
		}

		view.setCenter(xDestination, yDestination);
	}
}
