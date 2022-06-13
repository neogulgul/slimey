#pragma once

class Map {
	public:
		sf::Image tileset;
		sf::Image image;
		sf::Vector2u size;
		sf::Vector2u spawn;

		Map(std::string imagePath);
		void draw(sf::RenderWindow &window, sf::View view);
};
