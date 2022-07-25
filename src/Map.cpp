#include <cmath>
#include <string>
#include <SFML/Graphics.hpp>
#include "headers/Bullet.hpp"
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/Turret.hpp"

Map::Map() {}

Map::Map(int mapIndex) {
	if (this->image.loadFromFile("resources/textures/maps/" + std::to_string(mapIndex) + ".png")) {
		this->valid = true;
		this->index = mapIndex;
	} else { // if unable to load map
		this->image.create(16, 16, sf::Color::Transparent);
		this->image.setPixel(0, 15, playerWall);
		for (int i = 1; i <= 15; i++) {
			this->image.setPixel(i, 15, foreWall);
		}
		this->image.setPixel(15, 14, levelExit);
		this->image.setPixel(15, 13, levelExit);
	}
	this->size = this->image.getSize();

	sf::Color pixel;

	// locating player and turrets
	for (int x = 0; x < this->size.x; x++) {
		for (int y = 0; y < this->size.y; y++) {
			pixel = this->image.getPixel(x, y);
			if (pixel == playerWall) {
				this->spawn.x = x;
				this->spawn.y = y - 1;
			} else if (pixel == turretUp) {
				this->turretVector.push_back(Turret(sf::Vector2f(x * tilesize + tilesize / 2 - bulletRadius, y * tilesize            - bulletRadius * 2), up));
			} else if (pixel == turretDown) {
				this->turretVector.push_back(Turret(sf::Vector2f(x * tilesize + tilesize / 2 - bulletRadius, y * tilesize + tilesize                   ), down));
			} else if (pixel == turretLeft) {
				this->turretVector.push_back(Turret(sf::Vector2f(x * tilesize            - bulletRadius * 2, y * tilesize + tilesize / 2 - bulletRadius), left));
			} else if (pixel == turretRight) {
				this->turretVector.push_back(Turret(sf::Vector2f(x * tilesize + tilesize                   , y * tilesize + tilesize / 2 - bulletRadius), right));
			}
		}
	}
}

void Map::reset() {
	this->clearTime = 0;
	this->turretTimer = 0;

	for (int i = 0; i < this->bulletVector.size(); i++) {
		this->bulletVector.at(i).exploding = true;
	}
}

void Map::update(sf::FloatRect playerRect, bool &playerDead, bool frozen, SoundManager &soundManager) {
	this->sawbladeTimer++;
	if (this->sawbladeTimer == this->sawbladeFrames) {
		this->sawbladeTimer = 0;
	}

	if (!frozen) {
		this->turretTimer++;
		if (this->turretTimer == this->turretFrames) {
			this->turretTimer = 0;

			for (Turret &turret : this->turretVector) {
				if (soundManager.playSFX) {
					soundManager.bulletShot.play();
				}
				turret.shoot(this->bulletVector);
			}
		}
	}

	for (int i = 0; i < this->bulletVector.size(); i++) {
		this->bulletVector.at(i).update(this->image, this->size, playerRect, playerDead);
	}

	for (int i = 0; i < this->bulletVector.size(); i++) {
		if (this->bulletVector.at(i).destroyed) {
			if (soundManager.playSFX) {
				soundManager.bulletHit.play();
			}
			this->bulletVector.at(i).destroy(this->bulletVector);
		}
	}
}

void Map::draw(sf::RenderWindow &window, sf::View view, sf::Sprite &tilesetSprite, sf::Sprite &bulletSprite, sf::Sprite &bulletExplosionSprite, bool drawHitbox) {
	sf::Color pixel;
	int xCrop, yCrop;

	for (int x = 0; x < this->size.x; x++) {
		for (int y = 0; y < this->size.y; y++) {
			if (x >= std::floor((view.getCenter().x - viewWidth / 2) / tilesize) && x <= std::floor((view.getCenter().x + viewWidth / 2) / tilesize)
				&&
				y >= std::floor((view.getCenter().y - viewHeight / 2) / tilesize) && y <= std::floor((view.getCenter().y + viewHeight / 2) / tilesize) ) {
				pixel = this->image.getPixel(x, y);
				if (pixel != sf::Color::Transparent) {
					tilesetSprite.setRotation(0);
					tilesetSprite.setPosition(x * tilesize, y * tilesize);

					xCrop = 0;
					yCrop = 0;

					if (pixel == backWall) {
						yCrop = 1;
					} else if (pixel == playerWall) {
						xCrop = 1;
					} else if (pixel == sawblade) {
						yCrop = 4;
						xCrop = this->sawbladeTimer;
						if (this->sawbladeTimer >= sawbladeFrames / 2) {
							yCrop++;
							xCrop -= sawbladeFrames / 2;
						}
					} else if (pixel == ice) {
						yCrop = 3;
					} else if (pixel == bounce) {
						xCrop = 1;
						yCrop = 3;
					} else if (pixel == turretUp || pixel == turretDown || pixel == turretLeft || pixel == turretRight) {
						yCrop = 2;
						if (pixel == turretLeft) {
							tilesetSprite.setRotation(-90);
							tilesetSprite.setPosition((x    ) * tilesize, (y + 1) * tilesize);
						} else if (pixel == turretDown) {
							tilesetSprite.setRotation(-180);
							tilesetSprite.setPosition((x + 1) * tilesize, (y + 1) * tilesize);
						} else if (pixel == turretRight) {
							tilesetSprite.setRotation(-270);
							tilesetSprite.setPosition((x + 1) * tilesize, (y    ) * tilesize);
						}
					} else if (pixel == levelExit) {
						xCrop = 1;
						if (this->image.getPixel(x, y + 1) == levelExit) {
							yCrop = 1;
						} else {
							yCrop = 2;
						}
					}

					tilesetSprite.setTextureRect(sf::IntRect(xCrop * tilesize, yCrop * tilesize, tilesize, tilesize));
					window.draw(tilesetSprite);

					if (pixel == sawblade && drawHitbox) {
						sf::RectangleShape hitbox(sf::Vector2f(14, 14));
						hitbox.setPosition(x * tilesize + 1, y * tilesize + 1);
						hitbox.setFillColor(sf::Color::Transparent);
						hitbox.setOutlineColor(sf::Color(255, 0, 0, 127));
						hitbox.setOutlineThickness(-1);
						window.draw(hitbox);
					}
				}
			}
		}
	}

	for (Bullet &bullet : this->bulletVector) {
		bullet.draw(window, view, bulletSprite, bulletExplosionSprite, drawHitbox);
	}
}
