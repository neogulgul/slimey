#include <SFML/Graphics.hpp>
#include <iostream>

#include "headers/Player.hpp"
#include "headers/constants.hpp"

Player::Player() {
	this->x = 0;
	this->y = 0;

	this->texture.loadFromFile("resources/textures/slimey.png");
	this->sprite.setTexture(this->texture);

	this->outline.setSize(sf::Vector2f(this->width - this->outlineThickness * 2, this->height - this->outlineThickness * 2));
	this->outline.setFillColor(sf::Color::Transparent);
	this->outline.setOutlineColor(sf::Color(255, 0, 0));
	this->outline.setOutlineThickness(this->outlineThickness);
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		this->jump = true;
	} else {
		this->jump = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		this->x = 0;
		this->y = 0;
		this->xVelocity = 0;
		this->yVelocity = 0;
	}
}

void Player::collision(std::vector<CollisionObject> CollisionObjects) {
	this->xDelta = this->xVelocity;
	this->yDelta = this->yVelocity;

	while (this->xDelta != 0 || this->yDelta != 0) {
		if (this->xDelta > 0) {
			this->xDelta -= 1;
			if (this->xDelta < 0) {
				this->xDelta = 0;
			}
		} else {
			this->xDelta += 1;
			if (this->xDelta > 0) {
				this->xDelta = 0;
			}
		}

		for (int i = 0; i < CollisionObjects.size(); i++) {
			CollisionObject collider = CollisionObjects.at(i);
			if (this->x + this->xVelocity - this->xDelta > collider.x - this->width &&
				this->x + this->xVelocity - this->xDelta < collider.x + collider.width &&
				this->y + this->yVelocity - this->yDelta > collider.y - this->height &&
				this->y + this->yVelocity - this->yDelta < collider.y + collider.height) {
				if (xVelocity > 0) {
					this->x = collider.x - this->width;
					this->xVelocity = 0;
				} else {
					this->x = collider.x + collider.width;
					this->xVelocity = 0;
				}
				this->xDelta = 0;
			}
		}

		if (this->yDelta > 0) {
			this->yDelta -= 1;
			if (this->yDelta < 0) {
				this->yDelta = 0;
			}
		} else {
			this->yDelta += 1;
			if (this->yDelta > 0) {
				this->yDelta = 0;
			}
		}

		for (int i = 0; i < CollisionObjects.size(); i++) {
			CollisionObject collider = CollisionObjects.at(i);
			if (this->x + this->xVelocity - this->xDelta > collider.x - this->width &&
				this->x + this->xVelocity - this->xDelta < collider.x + collider.width &&
				this->y + this->yVelocity - this->yDelta > collider.y - this->height &&
				this->y + this->yVelocity - this->yDelta < collider.y + collider.height) {
				if (yVelocity > 0) {
					this->y = collider.y - this->height;
					this->yVelocity = 0;
					this->onground = true;
				} else {
					this->y = collider.y + collider.height;
					this->yVelocity = 0;
				}
				this->yDelta = 0;
			}
		}
	}
}

void Player::update(std::vector<CollisionObject> CollisionObjects) {
	this->input();

	if (!this->onground) {
		this->yVelocity += gravity;
	} else if (this->jump) {
		this->yVelocity = -3;
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

	if (this->xVelocity > this->maxHorizontalVelocity) {
		this->xVelocity = this->maxHorizontalVelocity;
	} else if (this->xVelocity < -this->maxHorizontalVelocity) {
		this->xVelocity = -this->maxHorizontalVelocity;
	}

	this->onground = false;
	this->collision(CollisionObjects);

	this->x += this->xVelocity;
	this->y += this->yVelocity;
}

void Player::draw(sf::RenderWindow &window, std::vector<CollisionObject> CollisionObjects) {
	this->update(CollisionObjects);
	this->sprite.setPosition(sf::Vector2f(this->x, this->y));
	this->outline.setPosition(sf::Vector2f(this->x + this->outlineThickness, this->y + this->outlineThickness));
	window.draw(this->sprite);
	window.draw(this->outline);
}
