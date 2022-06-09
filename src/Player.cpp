#include <SFML/Graphics.hpp>

#include "headers/Player.hpp"
#include "headers/constants.hpp"

Player::Player() {
	this->x = 0;
	this->y = 0;

	this->texture.loadFromFile("resources/textures/slimey.png");
	this->sprite.setTexture(this->texture);
}

void Player::input() {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		this->up = true;
	} else {
		this->up = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		this->down = true;
	} else {
		this->down = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		this->left = true;
	} else {
		this->left = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		this->right = true;
	} else {
		this->right = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		this->x = 0;
		this->y = 0;
		this->xVelocity = 0;
		this->yVelocity = 0;
	}
}

void Player::collision() {
}

void Player::update() {
	this->input();

	if (!this->onground) {
		this->yVelocity += gravity;
	}

	if (this->left && !this->right) {
		this->xVelocity -= this->acceleration;
	} else if (this->right && !this->left) {
		this->xVelocity += this->acceleration;
	} else if (this->onground) {
		if (this->xVelocity > 0) {
			this->xVelocity -= this->deceleration;
			if (this->xVelocity < 0) {
				this->xVelocity = 0;
			}
		} else if (this->xVelocity < 0) {
			this->xVelocity += this->deceleration;
			if (this->xVelocity > 0) {
				this->xVelocity = 0;
			}
		}
	}

	this->collision();

	this->x += this->xVelocity;
	this->y += this->yVelocity;
}

void Player::draw(sf::RenderWindow &window) {
	this->update();
	this->sprite.setPosition(sf::Vector2f(this->x, this->y));
	window.draw(this->sprite);
}
