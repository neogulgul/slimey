#include <cmath>
#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/Player.hpp"

Player::Player(int xCord, int yCord) {
	this->setPosition(xCord, yCord);
	this->xCordSpawn = xCord;
	this->yCordSpawn = yCord;
	this->texture.loadFromFile("resources/textures/slimey.png");
	this->sprite.setTexture(this->texture);
}

void Player::setPosition(int xCord, int yCord) {
	this->x = xCord * tilesize + (tilesize - this->width) / 2;
	this->y = yCord * tilesize + tilesize - this->height;
	this->xVelocity = 0;
	this->yVelocity = 0;
	this->onground = false;
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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
		if (!this->jump) {
			this->jump = true;
			this->preJumpTimer = this->preJumpFrames;
		}
	} else {
		this->jump = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
		this->setPosition(this->xCordSpawn, this->yCordSpawn);
	}
}

void Player::jumping() {
	this->yVelocity = -this->jumpForce;
	this->jumped = true;
}

void Player::checkCollision(Map map) {
	float xDelta = this->xVelocity;
	float yDelta = this->yVelocity;
	float xCurrent;
	float yCurrent;

	sf::Vector2f cell_0;
	sf::Vector2f cell_1;
	sf::Vector2f cell_2;
	sf::Vector2f cell_3;

	while (xDelta != 0 || yDelta != 0) {
		////////////////////////
		// VERTICAL COLLISION //
		////////////////////////
		if (yDelta > 0) {
			yDelta -= 1;
			if (yDelta < 0) {
				yDelta = 0;
			}
		} else if (yDelta < 0) {
			yDelta += 1;
			if (yDelta > 0) {
				yDelta = 0;
			}
		}

		yCurrent = this->y - yDelta;
		xCurrent = this->x - xDelta;

		cell_0.x = std::floor(xCurrent / tilesize);
		cell_0.y = std::floor(yCurrent / tilesize);

		cell_1.x = std::ceil(xCurrent / tilesize);
		cell_1.y = std::floor(yCurrent / tilesize);

		cell_2.x = std::floor(xCurrent / tilesize);
		cell_2.y = std::ceil(yCurrent / tilesize);

		cell_3.x = std::ceil(xCurrent / tilesize);
		cell_3.y = std::ceil(yCurrent / tilesize);

		// up
		if (cell_0.x >= 0 && cell_0.x <= map.size.x
			&&
			cell_0.y >= 0 && cell_0.y <= map.size.y
			&&
			map.image.getPixel(cell_0.x, cell_0.y) == foreWall
			&&
			collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* tile */ cell_0.x * tilesize, cell_0.y * tilesize, tilesize, tilesize)
			||
			cell_1.x >= 0 && cell_1.x <= map.size.x
			&&
			cell_1.y >= 0 && cell_1.y <= map.size.y
			&&
			map.image.getPixel(cell_1.x, cell_1.y) == foreWall
			&&
			collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* tile */ cell_1.x * tilesize, cell_1.y * tilesize, tilesize, tilesize)
			) {
			this->y = cell_0.y * tilesize + tilesize;
			this->yVelocity = 0;
			yDelta = 0;
		}
		// down
		else if (
			cell_2.x >= 0 && cell_2.x <= map.size.x
			&&
			cell_2.y >= 0 && cell_2.y <= map.size.y
			&&
			map.image.getPixel(cell_2.x, cell_2.y) == foreWall
			&&
			(
			collision(/* player */ xCurrent, yCurrent,           this->width, this->height, /* tile */ cell_2.x * tilesize, cell_2.y * tilesize, tilesize, tilesize)
			||
			collision(/* player */ xCurrent, yCurrent + gravity, this->width, this->height, /* tile */ cell_2.x * tilesize, cell_2.y * tilesize, tilesize, tilesize)
			)
			||
			cell_3.x >= 0 && cell_3.x <= map.size.x
			&&
			cell_3.y >= 0 && cell_3.y <= map.size.y
			&&
			map.image.getPixel(cell_3.x, cell_3.y) == foreWall
			&&
			(
			collision(/* player */ xCurrent, yCurrent,           this->width, this->height, /* tile */ cell_3.x * tilesize, cell_3.y * tilesize, tilesize, tilesize)
			||
			collision(/* player */ xCurrent, yCurrent + gravity, this->width, this->height, /* tile */ cell_3.x * tilesize, cell_3.y * tilesize, tilesize, tilesize)
			)
			) {
			this->onground = true;
			this->postJumpTimer = this->postJumpFrames;
			this->y = cell_3.y * tilesize - this->height;
			this->yVelocity = 0;
			yDelta = 0;
		} else {
			this->onground = false;
		}

		//////////////////////////
		// HORIZONTAL COLLISION //
		//////////////////////////
		if (xDelta > 0) {
			xDelta -= 1;
			if (xDelta < 0) {
				xDelta = 0;
			}
		} else if (xDelta < 0) {
			xDelta += 1;
			if (xDelta > 0) {
				xDelta = 0;
			}
		}

		yCurrent = this->y - yDelta;
		xCurrent = this->x - xDelta;

		cell_0.x = std::floor(xCurrent / tilesize);
		cell_0.y = std::floor(yCurrent / tilesize);

		cell_1.x = std::ceil(xCurrent / tilesize);
		cell_1.y = std::floor(yCurrent / tilesize);

		cell_2.x = std::floor(xCurrent / tilesize);
		cell_2.y = std::ceil(yCurrent / tilesize);

		cell_3.x = std::ceil(xCurrent / tilesize);
		cell_3.y = std::ceil(yCurrent / tilesize);

		if (xCurrent < 0) {
			this->x = 0;
			this->xVelocity = 0;
			xDelta = 0;
		} else if (xCurrent > map.size.x * tilesize - this->width) {
			this->x = map.size.x * tilesize - this->width;
			this->xVelocity = 0;
			xDelta = 0;
		} else {
			// left
			if (cell_0.x >= 0 && cell_0.x <= map.size.x
				&&
				cell_0.y >= 0 && cell_0.y <= map.size.y
				&&
				map.image.getPixel(cell_0.x, cell_0.y) == foreWall
				&&
				collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* tile */ cell_0.x * tilesize, cell_0.y * tilesize, tilesize, tilesize)
				||
				cell_2.x >= 0 && cell_2.x <= map.size.x
				&&
				cell_2.y >= 0 && cell_2.y <= map.size.y
				&&
				map.image.getPixel(cell_2.x, cell_2.y) == foreWall
				&&
				collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* tile */ cell_2.x * tilesize, cell_2.y * tilesize, tilesize, tilesize)
				) {
				this->x = cell_0.x * tilesize + tilesize;
				this->xVelocity = 0;
				xDelta = 0;
			}
			// right
			else if (
				cell_1.x >= 0 && cell_1.x <= map.size.x
				&&
				cell_1.y >= 0 && cell_1.y <= map.size.y
				&&
				map.image.getPixel(cell_1.x, cell_1.y) == foreWall
				&&
				collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* tile */ cell_1.x * tilesize, cell_1.y * tilesize, tilesize, tilesize)
				||
				cell_3.x >= 0 && cell_3.x <= map.size.x
				&&
				cell_3.y >= 0 && cell_3.y <= map.size.y
				&&
				map.image.getPixel(cell_3.x, cell_3.y) == foreWall
				&&
				collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* tile */ cell_3.x * tilesize, cell_3.y * tilesize, tilesize, tilesize)
				) {
				this->x = cell_3.x * tilesize - this->width;
				this->xVelocity = 0;
				xDelta = 0;
			}
		}
	}
}

void Player::update(Map map) {
	this->input();

	///////////////////////
	// VERTICAL MOVEMENT //
	///////////////////////
	if (this->onground) {
		if (this->preJumpTimer > 0 && !this->jumped) {
			this->jumping();
		} else {
			this->jumped = false;
		}
	} else {
		if (this->preJumpTimer > 0 && this->postJumpTimer > 0 && !this->jumped) {
			this->jumping();
		}
		this->yVelocity += gravity;
	}

	if (this->preJumpTimer > 0) {
		this->preJumpTimer--;
	}
	if (this->postJumpTimer > 0) {
		this->postJumpTimer--;
	}

	/////////////////////////
	// HORIZONTAL MOVEMENT //
	/////////////////////////
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

	this->x += this->xVelocity;
	this->y += this->yVelocity;

	this->checkCollision(map);
}

void Player::draw(sf::RenderWindow &window) {
	this->sprite.setPosition(this->x, this->y);
	window.draw(this->sprite);
}
