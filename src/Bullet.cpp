#include <SFML/Graphics.hpp>
#include "headers/Bullet.hpp"
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/Player.hpp"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f velocity) {
	this->position = position;
	this->velocity = velocity;
}

void Bullet::update() {
	this->position.x += this->velocity.x;
	this->position.y += this->velocity.y;
}

void Bullet::draw(sf::RenderWindow &window, sf::View &view, sf::Sprite bulletSprite) {
	if (this->position.x + bulletRadius * 2 > view.getCenter().x - viewWidth  / 2 &&
		this->position.x                    < view.getCenter().x + viewWidth  / 2 &&
		this->position.y + bulletRadius * 2 > view.getCenter().y - viewHeight / 2 &&
		this->position.y                    < view.getCenter().y + viewHeight / 2) {
		bulletSprite.setPosition(this->position);
		window.draw(bulletSprite);
	}
}
