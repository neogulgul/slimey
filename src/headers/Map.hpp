#pragma once

class Map {
	public:
		sf::Image image;
		sf::Vector2u size;

		Map(std::string imagePath);
		void draw(sf::RenderWindow &window, sf::View view);
};
