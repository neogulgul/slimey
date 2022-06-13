#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/Map.hpp"

Map::Map(std::string imagePath) {
	this->tileset.loadFromFile("resources/textures/tileset.png");
	this->image.loadFromFile(imagePath);
	this->size = this->image.getSize();

	for (int x = 0; x < this->size.x; x++) {
		for (int y = 0; y < this->size.y; y++) {
			if (this->image.getPixel(x, y) == sf::Color(90, 197, 79)) {
				this->spawn.x = x;
				this->spawn.y = y;
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
				if (pixel != sf::Color::Transparent && pixel != playerColor) {
					if (pixel == foreWall) {
						xCrop = 0;
						yCrop = 0;
					} else if (pixel == backWall) {
						xCrop = 1;
						yCrop = 0;
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
