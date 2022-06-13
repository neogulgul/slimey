#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "headers/constants.hpp"
#include "headers/Map.hpp"

Map::Map(std::string imagePath) {
	this->tileset.loadFromFile("resources/textures/tileset.png");
	this->image.loadFromFile(imagePath);
	this->size = this->image.getSize();
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
					if (pixel == sf::Color(230, 156, 105)) {
						xCrop = 0;
					} else if (pixel == sf::Color(246, 202, 159)) {
						xCrop = tilesize * 1;
					} else if (pixel == sf::Color(255, 255, 255)) {
						xCrop = tilesize * 2;
					} else if (pixel == sf::Color(133, 133, 133)) {
						xCrop = tilesize * 3;
					} else if (pixel == sf::Color(61, 61, 61)) {
						xCrop = tilesize * 4;
					}

					if (y != 0 && this->image.getPixel(x, y - 1) != sf::Color::Transparent) {
						yCrop = tilesize;
					} else {
						yCrop = 0;
					}

					texture.loadFromImage(this->tileset, sf::IntRect(xCrop, yCrop, tilesize, tilesize));
					sprite.setTexture(texture);
					sprite.setPosition(x * tilesize, y * tilesize);
					window.draw(sprite);
				}
			}
		}
	}
}
