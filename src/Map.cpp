#include <cmath>
#include <string>
#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/Map.hpp"

Map::Map() {}

Map::Map(int mapIndex) {
	this->tileset.loadFromFile("resources/textures/tileset.png");
	if (!this->image.loadFromFile("resources/textures/maps/" + std::to_string(mapIndex) + ".png")) {
		this->image.loadFromFile("resources/textures/maps/null.png");
	}
	this->size = this->image.getSize();

	sf::Color pixel;

	for (int x = 0; x < this->size.x; x++) {
		for (int y = 0; y < this->size.y; y++) {
			pixel = this->image.getPixel(x, y);
			if (pixel == playerWall) {
				this->spawn.x = x;
				this->spawn.y = y - 1;
				break;
			}
		}
	}
}

void Map::draw(sf::RenderWindow &window, sf::View view) {
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Color pixel;
	sf::Color topPixel;
	int xCrop, yCrop;

	for (int x = 0; x < this->size.x; x++) {
		for (int y = 0; y < this->size.y; y++) {
			if (x >= std::floor((view.getCenter().x - viewWidth / 2) / tilesize) && x <= std::floor((view.getCenter().x + viewWidth / 2) / tilesize)
				&&
				y >= std::floor((view.getCenter().y - viewHeight / 2) / tilesize) && y <= std::floor((view.getCenter().y + viewHeight / 2) / tilesize) ) {
				pixel = this->image.getPixel(x, y);
				if (pixel != sf::Color::Transparent) {
					xCrop = 0;
					yCrop = 0;
					if (pixel == backWall) {
						yCrop = 1;
					} else if (pixel == playerWall) {
						xCrop = 1;
					} else if (pixel == danger) {
						yCrop = 2;
					} else if (pixel == ice) {
						yCrop = 3;
					} else if (pixel == bounce) {
						xCrop = 1;
						yCrop = 3;
					} else if (pixel == levelExit) {
						xCrop = 1;
						if (this->image.getPixel(x, y + 1) == levelExit) {
							yCrop = 1;
						} else {
							yCrop = 2;
						}
					}

					texture.loadFromImage(this->tileset, sf::IntRect(xCrop * tilesize, yCrop * tilesize, tilesize, tilesize));
					sprite.setTexture(texture);
					sprite.setPosition(x * tilesize, y * tilesize);
					window.draw(sprite);
				}
			}
		}
	}
}
