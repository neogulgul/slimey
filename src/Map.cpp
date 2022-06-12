#include <string>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "headers/constants.hpp"
#include "headers/Map.hpp"

Map::Map(std::string imagePath) {
	this->image.loadFromFile(imagePath);
	this->size = this->image.getSize();
}

void Map::draw(sf::RenderWindow &window, sf::View view) {
	sf::Image tileset;
	tileset.loadFromFile("resources/textures/tileset.png");
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Color pixel;
	sf::Color topPixel;
	int xCrop, yCrop;
	for (int x = 0; x < this->size.x; x++) {
		for (int y = 0; y < this->size.y; y++) {
			if (x >= std::floor((view.getCenter().x - viewWidth / 2) / 16) && x <= std::floor((view.getCenter().x + viewWidth / 2) / 16)
				&&
				y >= std::floor((view.getCenter().y - viewHeight / 2) / 16) && y <= std::floor((view.getCenter().y + viewHeight / 2) / 16) ) {
				pixel = this->image.getPixel(x, y);
				if (pixel != sf::Color::Transparent) {
					if (pixel == sf::Color(230, 156, 105)) {
						xCrop = 0;
					} else if (pixel == sf::Color(246, 202, 159)) {
						xCrop = 16;
					} else if (pixel == sf::Color(255, 255, 255)) {
						xCrop = 32;
					} else if (pixel == sf::Color(133, 133, 133)) {
						xCrop = 48;
					} else if (pixel == sf::Color(61, 61, 61)) {
						xCrop = 64;
					}

					if (y != 0 && this->image.getPixel(x, y - 1) != sf::Color::Transparent) {
						yCrop = 16;
					} else {
						yCrop = 0;
					}

					texture.loadFromImage(tileset, sf::IntRect(xCrop, yCrop, 16, 16));
					sprite.setTexture(texture);
					sprite.setPosition(x * 16, y * 16);
					window.draw(sprite);
				}
			}
		}
	}
}
