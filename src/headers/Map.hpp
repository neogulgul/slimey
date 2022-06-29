#pragma once

class Map {
	public:
		sf::Image tileset;
		sf::Image image;
		sf::Vector2u size;
		sf::Vector2u spawn;

		bool cleared = false;
		sf::Clock clock;

		Map();
		Map(int mapIndex);
		void draw(sf::RenderWindow &window, sf::View view);
};
