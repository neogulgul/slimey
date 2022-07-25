#include <cmath>
#include <SFML/Graphics.hpp>
#include "headers/Bullet.hpp"
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/Player.hpp"

Bullet::Bullet(sf::Vector2f position, sf::Vector2f velocity, int index) {
	this->position = position;
	this->velocity = velocity;
	this->index = index;
}

void Bullet::destroy(std::vector<Bullet> &bulletVector) {
	bulletVector.erase(bulletVector.begin() + this->index);

	for (int i = 0; i < bulletVector.size(); i++) {
		bulletVector.at(i).index = i;
	}
}

bool Bullet::checkCollision(sf::Image &mapImage, sf::Vector2u &mapSize, sf::FloatRect playerRect, bool &playerDead) {
	sf::Vector2f delta = this->velocity;
	sf::Vector2f currentPos;
	sf::Vector2f currentTile;
	sf::Vector2f currentIntersections;

	while (delta.x != 0 || delta.y != 0) {
		////////////////////////
		// VERTICAL COLLISION //
		////////////////////////
		if (delta.y != 0) {
			if (delta.y > 0) {
				delta.y--;
				if (delta.y < 0) {
					delta.y = 0;
				}
			} else {
				delta.y++;
				if (delta.y > 0) {
					delta.y = 0;
				}
			}

			currentPos.x = this->position.x - delta.x;
			currentPos.y = this->position.y - delta.y;
			currentTile.x = std::floor(currentPos.x / tilesize);
			currentTile.y = std::floor(currentPos.y / tilesize);
			currentIntersections.x = std::floor((currentPos.x + bulletRadius * 2) / tilesize) - currentTile.x;
			currentIntersections.y = std::floor((currentPos.y + bulletRadius * 2) / tilesize) - currentTile.y;

			if (currentPos.y < 0 || currentPos.y > mapSize.y * tilesize - bulletRadius * 2) {
				delta.y = 0;
				return true;
			} else {
				for (int x = 0; x < 1 + currentIntersections.x; x++) {
					sf::Vector2u cord(currentTile.x + x, currentTile.y);
					if (this->velocity.y > 0) {
						cord.y += currentIntersections.y;
					}
					if (currentPos.x <= cord.x * tilesize - bulletRadius * 2 || currentPos.x >= cord.x * tilesize + tilesize) { break; }
					if (validCollisionTile(mapImage, mapSize, cord.x, cord.y)) {
						delta.y = 0;
						if (mapImage.getPixel(cord.x, cord.y) == bounce) {
							this->velocity.y *= -1;
							return false;
						}
						return true;
					}
				}
			}

			if (collision(playerRect.left, playerRect.top, playerRect.width, playerRect.height, currentPos.x, currentPos.y, bulletRadius * 2, bulletRadius * 2)) {
				playerDead = true;
				delta.y = 0;
				return true;
			}
		}

		//////////////////////////
		// HORIZONTAL COLLISION //
		//////////////////////////
		if (delta.x != 0) {
			if (delta.x > 0) {
				delta.x--;
				if (delta.x < 0) {
					delta.x = 0;
				}
			} else {
				delta.x++;
				if (delta.x > 0) {
					delta.x = 0;
				}
			}

			currentPos.x = this->position.x - delta.x;
			currentPos.y = this->position.y - delta.y;
			currentTile.x = std::floor(currentPos.x / tilesize);
			currentTile.y = std::floor(currentPos.y / tilesize);
			currentIntersections.x = std::floor((currentPos.x + bulletRadius * 2) / tilesize) - currentTile.x;
			currentIntersections.y = std::floor((currentPos.y + bulletRadius * 2) / tilesize) - currentTile.y;

			if (currentPos.x < 0 || currentPos.x > mapSize.x * tilesize - bulletRadius * 2) {
				delta.x = 0;
				return true;
			} else {
				for (int y = 0; y < 1 + currentIntersections.y; y++) {
					sf::Vector2u cord(currentTile.x, currentTile.y + y);
					if (this->velocity.x > 0) {
						cord.x += currentIntersections.x;
					}
					if (currentPos.y <= cord.y * tilesize - bulletRadius * 2 || currentPos.y >= cord.y * tilesize + tilesize) { break; }
					if (validCollisionTile(mapImage, mapSize, cord.x, cord.y)) {
						delta.x = 0;
						if (mapImage.getPixel(cord.x, cord.y) == bounce) {
							this->velocity.x *= -1;
							return false;
						}
						return true;
					}
				}
			}

			if (collision(playerRect.left, playerRect.top, playerRect.width, playerRect.height, currentPos.x, currentPos.y, bulletRadius * 2, bulletRadius * 2)) {
				playerDead = true;
				delta.x = 0;
				return true;
			}
		}
	}

	return false;
}

void Bullet::update(sf::Image &mapImage, sf::Vector2u &mapSize, sf::FloatRect playerRect, bool &playerDead) {
	if (!this->exploding) {
		this->position.x += this->velocity.x;
		this->position.y += this->velocity.y;

		this->exploding = this->checkCollision(mapImage, mapSize, playerRect, playerDead);
	} else {
		this->explosionTimer++;
		if (this->explosionTimer == this->explosionFrames * this->explosionFramesDuration) {
			this->destroyed = true;
		}
	}
}

void Bullet::draw(sf::RenderWindow &window, sf::View &view, sf::Sprite &bulletSprite, sf::Sprite &bulletExplosionSprite, bool drawHitbox) {
	if (this->position.x + bulletRadius * 2 > view.getCenter().x - viewWidth  / 2 &&
		this->position.x                    < view.getCenter().x + viewWidth  / 2 &&
		this->position.y + bulletRadius * 2 > view.getCenter().y - viewHeight / 2 &&
		this->position.y                    < view.getCenter().y + viewHeight / 2) {
		if (!this->exploding) {
			bulletSprite.setPosition(this->position);
			window.draw(bulletSprite);

			if (drawHitbox) {
				sf::RectangleShape hitbox(sf::Vector2f(bulletRadius * 2, bulletRadius * 2));
				hitbox.setPosition(this->position);
				hitbox.setFillColor(sf::Color::Transparent);
				hitbox.setOutlineColor(sf::Color(255, 0, 0, 127));
				hitbox.setOutlineThickness(-1);
				window.draw(hitbox);
			}
		} else if (!this->destroyed) {
			int frame = std::floor(this->explosionTimer / this->explosionFramesDuration);
			bulletExplosionSprite.setTextureRect(sf::IntRect(frame * 8, 0, 8, 8));
			bulletExplosionSprite.setPosition(this->position.x - 2, this->position.y - 2);
			window.draw(bulletExplosionSprite);
		}
	}
}
