#include <SFML/Graphics.hpp>
#include "headers/global.hpp"

// colors
sf::Color backgroundColor( 66,  76, 110);
sf::Color       textColor(255, 255, 255);
sf::Color        foreWall(146, 161, 185);
sf::Color        backWall( 42,  47,  78);
sf::Color      playerWall( 90, 197,  79);
sf::Color          danger(234,  50,  60);
sf::Color       levelExit( 27,  27,  27);
sf::Color             ice(148, 253, 255);
sf::Color          bounce(255, 235,  87);

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

void textBox(sf::RenderWindow &window, sf::View &view, sf::Font font, int fontSize, std::string string, int x, int y, bool centerX, bool centerY, bool rightOrigin, bool bottomOrigin, int width, int height, int padding, sf::Color textColor, sf::Color backgroundColor, sf::Color outlineColor) {
	sf::Text text(string, font, fontSize);
	text.setFillColor(textColor);
	sf::FloatRect textRect = text.getGlobalBounds();
	sf::Vector2f adjustment;
	if (centerX) {
		adjustment.x = -textRect.width / 2;
	} else if (rightOrigin) {
		adjustment.x = -textRect.width;
	}
	if (centerY) {
		adjustment.y = -textRect.height / 2;
	} else if (bottomOrigin) {
		adjustment.y = -textRect.height;
	}
	text.setOrigin(0, text.getCharacterSize() - textRect.height);
	text.setPosition(sf::Vector2f(view.getCenter().x - viewWidth / 2 + x + adjustment.x,view.getCenter().y - viewHeight / 2 + y + adjustment.y));

	float backgroundWidth, backgroundHeight, backgroundPositionX, backgroundPositionY;
	if (width == 0) {
		backgroundWidth = textRect.width + padding;
		backgroundPositionX = view.getCenter().x - viewWidth / 2 + x + adjustment.x - padding / 2;
	} else {
		backgroundWidth = width;
		backgroundPositionX = view.getCenter().x - viewWidth / 2 + x - width / 2;
	}
	if (height == 0) {
		backgroundHeight = textRect.height + padding;
		backgroundPositionY = view.getCenter().y - viewHeight / 2 + y + adjustment.y - padding / 2;
	} else {
		backgroundHeight = height;
		backgroundPositionY = view.getCenter().y - viewHeight / 2 + y - height / 2;
	}
	sf::RectangleShape textBackground(sf::Vector2f(backgroundWidth, backgroundHeight));
	textBackground.setPosition(sf::Vector2f(backgroundPositionX, backgroundPositionY));
	textBackground.setFillColor(backgroundColor);
	textBackground.setOutlineColor(outlineColor);
	textBackground.setOutlineThickness(-1);

	window.draw(textBackground);
	window.draw(text);
}

void updateView(sf::View &view, float xTarget, float yTarget, sf::Vector2u mapSize, bool instant) {
	float xDestination;
	float yDestination;

	if (instant) {
		xDestination = xTarget;
		yDestination = yTarget;
	} else {
		const float xStart = view.getCenter().x;
		const float yStart = view.getCenter().y;
		const float xDelta = xTarget - xStart;
		const float yDelta = yTarget - yStart;
		const float xDistance = xDelta * 0.1;
		const float yDistance = yDelta * 0.1;
		xDestination = xStart + xDistance;
		yDestination = yStart + yDistance;
	}

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
