#pragma once

class Map {
	public:
		sf::Image tileset;
		sf::Image image;
		sf::Vector2u size;
		sf::Vector2u spawn;

		bool cleared = false;
		float clearTime;
		sf::Clock clock;

		Map(std::string imagePath);
		void draw(sf::RenderWindow &window, sf::View view);
};
