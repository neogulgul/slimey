#include <SFML/Graphics.hpp>

#include "headers/CollisionObject.hpp"

CollisionObject::CollisionObject(float x, float y, int width, int height) {
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;

	this->rectangle.setSize(sf::Vector2f(this->width, this->height));
	this->rectangle.setPosition(this->x, this->y);
}

void CollisionObject::draw(sf::RenderWindow &window) {
	window.draw(this->rectangle);
}
