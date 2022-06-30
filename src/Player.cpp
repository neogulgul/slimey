#include <cmath>
#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/Player.hpp"

Player::Player() {}

Player::Player(int xCord, int yCord) {
	this->xCordSpawn = xCord;
	this->yCordSpawn = yCord;
	this->setPosition(xCordSpawn, yCordSpawn);
}

void Player::setPosition(int xCord, int yCord) {
	this->x = xCord * tilesize + (tilesize - this->width) / 2;
	this->y = yCord * tilesize + tilesize - this->height;
	this->xVelocity = 0;
	this->yVelocity = 0;
	this->onGround = false;
}

void Player::input(Map &map) {
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
		map.resetTime();
	}
}

void Player::jumping() {
	this->jumpTimer++;
	this->yVelocity = -this->jumpForce;
	this->jumped = true;
}

void Player::death(Map &map) {
	this->alive = false;
	this->preJumpTimer = 0;
	this->postJumpTimer = 0;
	map.resetTime();
}

void Player::levelClear(Map &map) {
	map.cleared = true;
}

bool Player::validTile(Map map, int xCord, int yCord) {
	if (xCord >= 0 && xCord < map.size.x
		&&
		yCord >= 0 && yCord < map.size.y
		&&
		map.image.getPixel(xCord, yCord) != sf::Color::Transparent
		&&
		map.image.getPixel(xCord, yCord) != backWall) {
		return true;
	}

	return false;
}

void Player::checkCollision(Map &map) {
	float xDelta = this->xVelocity;
	float yDelta = this->yVelocity;
	float xCurrent;
	float yCurrent;
	sf::Vector2f currentTile;
	sf::Vector2f currentIntersections;

	while (xDelta != 0 || yDelta != 0) {
		if (yDelta > 0) {
			yDelta--;
			if (yDelta < 0) {
				yDelta = 0;
			}
		} else if (yDelta < 0) {
			yDelta++;
			if (yDelta > 0) {
				yDelta = 0;
			}
		}

		xCurrent = this->x - xDelta;
		yCurrent = this->y - yDelta;

		currentTile.x = std::floor(xCurrent / tilesize);
		currentTile.y = std::floor(yCurrent / tilesize);
		currentIntersections.x = std::floor((xCurrent + this->width) / tilesize) - currentTile.x;
		currentIntersections.y = std::floor((yCurrent + this->height) / tilesize) - currentTile.y;

		if (yCurrent > map.size.y * tilesize) {
			this->y = yCurrent - this->height;
			this->death(map);
			yDelta = 0;
		} else {
			this->hitCeil       = false;
			this->hitCeilBounce = false;

			// up
			if (this->yVelocity < 0) {
				for (int x = 0; x < 1 + currentIntersections.x; x++) {
					int xCord = currentTile.x + x;
					int yCord = currentTile.y;
					if (xCurrent <= xCord * tilesize - this->width || xCurrent >= xCord * tilesize + tilesize) { break; }
					if (validTile(map, xCord, yCord)) {
						sf::Color pixel = map.image.getPixel(xCord, yCord);
						if (pixel == danger && collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* danger */ xCord * tilesize + 1, yCord * tilesize + 1, tilesize - 2, tilesize - 2)) {
							this->death(map);
						} else if (pixel == levelExit && map.image.getPixel(xCord, yCord + 1) != levelExit && collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* levelExit */ xCord * tilesize + 7, yCord * tilesize + 7, 2, 2)) {
							this->levelClear(map);
						} else if (pixel != danger && pixel != levelExit) {
							this->y = yCord * tilesize + tilesize;
							if (pixel == bounce) {
								this->hitCeilBounce = true;
							} else {
								this->hitCeil = true;
							}
							this->preJumpTimer = 0;
							yDelta = 0;
						}
					}
				}
			}

			if (this->hitCeilBounce) {
				this->y++; // to avoid a bug
				this->yVelocity *= -this->bounciness;
				if (this->yVelocity < baseBounce) {
					this->yVelocity = baseBounce;
				}
			} else if (this->hitCeil) {
				this->yVelocity = 0;
			}

			this->onGround       = false;
			this->onIce          = false;
			this->hitFloor       = false;
			this->hitFloorBounce = false;

			// down
			for (int x = 0; x < 1 + currentIntersections.x; x++) {
				int xCord = currentTile.x + x;
				int yCord = currentTile.y + currentIntersections.y;
				if (xCurrent <= xCord * tilesize - this->width || xCurrent >= xCord * tilesize + tilesize) { break; }
				if (validTile(map, xCord, yCord)) {
					sf::Color pixel = map.image.getPixel(xCord, yCord);
					if (pixel == danger && collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* danger */ xCord * tilesize + 1, yCord * tilesize + 1, tilesize - 2, tilesize - 2)) {
						this->death(map);
					} else if (pixel == levelExit && map.image.getPixel(xCord, yCord + 1) != levelExit && collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* levelExit */ xCord * tilesize + 7, yCord * tilesize + 7, 2, 2)) {
						this->levelClear(map);
					} else if (pixel != danger && pixel != levelExit) {
						this->y = yCord * tilesize - this->height;
						if (pixel == bounce) {
							this->hitFloorBounce = true;
						} else {
							this->hitFloor = true;
						}
						this->postJumpTimer = this->postJumpFrames;
						if (pixel == ice) {
							this->onIce = true;
						}
						yDelta = 0;
					}
				}
			}

			if (this->hitFloorBounce) {
				this->y--; // to avoid a bug
				this->yVelocity *= -this->bounciness;
				if (this->yVelocity > -baseBounce) {
					this->yVelocity = -baseBounce;
				}
			} else if (this->hitFloor) {
				this->yVelocity = 0;
				this->onGround = true;
			}
		}

		//////////////////////////
		// HORIZONTAL COLLISION //
		//////////////////////////
		if (xDelta > 0) {
			xDelta--;
			if (xDelta < 0) {
				xDelta = 0;
			}
		} else if (xDelta < 0) {
			xDelta++;
			if (xDelta > 0) {
				xDelta = 0;
			}
		}

		xCurrent = this->x - xDelta;
		yCurrent = this->y - yDelta;

		currentTile.x = std::floor(xCurrent / tilesize);
		currentTile.y = std::floor(yCurrent / tilesize);
		currentIntersections.x = std::floor((xCurrent + this->width) / tilesize) - currentTile.x;
		currentIntersections.y = std::floor((yCurrent + this->height) / tilesize) - currentTile.y;

		if (xCurrent < 0) {
			this->x = 0;
			this->xVelocity = 0;
			xDelta = 0;
		} else if (xCurrent > map.size.x * tilesize - this->width) {
			this->x = map.size.x * tilesize - this->width;
			this->xVelocity = 0;
			xDelta = 0;
		} else {
			this->hitWall       = false;
			this->hitWallBounce = false;

			// left
			if (this->xVelocity < 0) {
				for (int y = 0; y < 1 + currentIntersections.y; y++) {
					int xCord = currentTile.x;
					int yCord = currentTile.y + y;
					if (yCurrent <= yCord * tilesize - this->height || yCurrent >= yCord * tilesize + tilesize) { break; }
					if (validTile(map, xCord, yCord)) {
						sf::Color pixel = map.image.getPixel(xCord, yCord);
						if (pixel == danger && collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* danger */ xCord * tilesize + 1, yCord * tilesize + 1, tilesize - 2, tilesize - 2)) {
							this->death(map);
						} else if (pixel == levelExit && map.image.getPixel(xCord, yCord + 1) != levelExit && collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* levelExit */ xCord * tilesize + 7, yCord * tilesize + 7, 2, 2)) {
							this->levelClear(map);
						} else if (pixel != danger && pixel != levelExit) {
							this->x = xCord * tilesize + tilesize;
							if (pixel == bounce) {
								this->hitWallBounce = true;
							} else {
								this->hitWall = true;
							}
							xDelta = 0;
						}
					}
				}
			}

			// right
			if (this->xVelocity > 0) {
				for (int y = 0; y < 1 + currentIntersections.y; y++) {
					int xCord = currentTile.x + currentIntersections.x;
					int yCord = currentTile.y + y;
					if (yCurrent <= yCord * tilesize - this->height || yCurrent >= yCord * tilesize + tilesize) { break; }
					if (validTile(map, xCord, yCord)) {
						sf::Color pixel = map.image.getPixel(xCord, yCord);
						if (pixel == danger && collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* danger */ xCord * tilesize + 1, yCord * tilesize + 1, tilesize - 2, tilesize - 2)) {
							this->death(map);
						} else if (pixel == levelExit && map.image.getPixel(xCord, yCord + 1) != levelExit && collision(/* player */ xCurrent, yCurrent, this->width, this->height, /* levelExit */ xCord * tilesize + 7, yCord * tilesize + 7, 2, 2)) {
							this->levelClear(map);
						} else if (pixel != danger && pixel != levelExit) {
							this->x = xCord * tilesize - this->width;
							if (pixel == bounce) {
								this->hitWallBounce = true;
							} else {
								this->hitWall = true;
							}
							xDelta = 0;
						}
					}
				}
			}

			if (this->hitWallBounce) {
				if (this->xVelocity > 0 && this->xVelocity < this->baseBounce) {
					this->xVelocity = this->baseBounce;
				} else if (this->xVelocity < 0 && this->xVelocity > -this->baseBounce) {
					this->xVelocity = -this->baseBounce;
				}
				this->xVelocity *= -this->bounciness;
			} else if (this->hitWall) {
				this->xVelocity = 0;
			}
		}
	}
}

void Player::update(Map &map, bool paused) {
	if (!this->alive || paused) {
		return;
	}

	this->input(map);

	if (this->jumpTimer > 0) {
		if (this->yVelocity == 0) {
			this->jumpTimer = 0;
		} else if (this->jump) {
			this->yVelocity = -this->jumpForce;

			this->jumpTimer++;
			if (this->jumpTimer == this->jumpFrames) {
				this->jumpTimer = 0;
			}
		} else {
			this->jumpTimer = 0;
		}
	}

	///////////////////////
	// VERTICAL MOVEMENT //
	///////////////////////
	if (this->onGround) {
		if (this->preJumpTimer > 0 && !this->jumped) {
			this->jumping();
		} else {
			this->jumped = false;
		}
	} else {
		if (this->preJumpTimer > 0 && this->postJumpTimer > 0 && !this->jumped) {
			this->jumping();
		}
		if (this->down) {
			this->yVelocity += gravity * 2;
		} else {
			this->yVelocity += gravity;
		}
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
	if (this->left && !this->right && !this->down && this->xVelocity >= -this->maxMoveVelocity) {
		this->xVelocity -= this->acceleration;
		if (this->xVelocity < -this->maxMoveVelocity) {
			this->xVelocity = -this->maxMoveVelocity;
		}
	} else if (this->right && !this->left && !this->down && this->xVelocity <= this->maxMoveVelocity) {
		this->xVelocity += this->acceleration;
		if (this->xVelocity > this->maxMoveVelocity) {
			this->xVelocity = this->maxMoveVelocity;
		}
	} else if (this->onGround && !this->onIce) {
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

	// preventing too small numbers
	if (this->xVelocity > 0 && this->xVelocity < 0.1) {
		this->xVelocity = 0;
	} else if (this->xVelocity < 0 && this->xVelocity > -0.1) {
		this->xVelocity = 0;
	}
	if (this->yVelocity > 0 && this->yVelocity < 0.1) {
		this->yVelocity = 0;
	} else if (this->yVelocity < 0 && this->yVelocity > -0.1) {
		this->yVelocity = 0;
	}

	this->x += this->xVelocity;
	this->y += this->yVelocity;

	this->checkCollision(map);

	// horizontal speed cap
	if (this->xVelocity > this->maxVelocity) {
		this->xVelocity = this->maxVelocity;
	} else if (this->xVelocity < -this->maxVelocity) {
		this->xVelocity = -this->maxVelocity;
	}
	// vertical speed cap
	if (this->yVelocity > this->maxVelocity) {
		this->yVelocity = this->maxVelocity;
	} else if (this->yVelocity < -this->maxVelocity) {
		this->yVelocity = -this->maxVelocity;
	}
}

void Player::draw(sf::RenderWindow &window, sf::View &view, sf::Sprite &playerSprite, sf::Sprite &playerDeathSprite, sf::Sprite &offscreenCircleSprite, bool paused) {
	if (this->alive) {
		int animation = 0;

		if (this->down) {
			animation = 2;
		} else if (this->left && !this->right) {
			animation = 3;
		} else if(this->right && !this->left) {
			animation = 4;
		} else if (this->up) {
			animation = 1;
		}

		if (this->animationTimer == this->animationFrames * this->animationFrameDuration) {
			this->animationTimer = 0;
		}

		int frame = std::floor(animationTimer / animationFrameDuration);
		playerSprite.setTextureRect(sf::IntRect(frame * this->width, animation * this->height, this->width, this->height));
		float xSpritePosition = this->x;
		float ySpritePosition = this->y;
		bool outsideView = false;

		if (this->x + this->width <= view.getCenter().x - viewWidth / 2) {
			xSpritePosition = view.getCenter().x - viewWidth / 2;
			outsideView = true;
		} else if (this->x >= view.getCenter().x + viewWidth / 2) {
			xSpritePosition = view.getCenter().x + viewWidth / 2 - this->width;
			outsideView = true;
		}
		if (this->y + this->height <= view.getCenter().y - viewHeight / 2) {
			ySpritePosition = view.getCenter().y - viewHeight / 2;
			outsideView = true;
		} else if (this->y >= view.getCenter().y + viewHeight / 2) {
			ySpritePosition = view.getCenter().y + viewHeight / 2 - this->height;
			outsideView = true;
		}

		if (outsideView) {
			offscreenCircleSprite.setPosition(xSpritePosition - offscreenCircleSprite.getTexture()->getSize().x / 2 + this->width / 2, ySpritePosition - offscreenCircleSprite.getTexture()->getSize().y / 2 + this->height / 2);
			window.draw(offscreenCircleSprite);
		}

		playerSprite.setPosition(xSpritePosition, ySpritePosition);
		window.draw(playerSprite);

		if (this->hitbox) {
			sf::RectangleShape hitbox(sf::Vector2f(this->width, this->height));
			hitbox.setPosition(sf::Vector2f(this->x, this->y));
			hitbox.setFillColor(sf::Color::Transparent);
			hitbox.setOutlineColor(sf::Color::Red);
			hitbox.setOutlineThickness(-1);
			window.draw(hitbox);
		}

		if (!paused) {
			animationTimer++;
		}
	} else {
		if (this->deathTimer == this->deathFrames * this->deathFramesDuration) {
			this->deathTimer = 0;
			this->setPosition(this->xCordSpawn, this->yCordSpawn);
			this->alive = true;
		} else {
			int frame = std::floor(deathTimer / deathFramesDuration);
			playerDeathSprite.setTextureRect(sf::IntRect(frame * tilesize, 0, tilesize, tilesize));
			playerDeathSprite.setPosition(this->x - (tilesize - this->width) / 2, this->y - (tilesize - this->height) / 2);
			window.draw(playerDeathSprite);

			if (!paused) {
				deathTimer++;
			}
		}
	}
}
