#pragma once

class Map {
	public:
		sf::Image tileset;
		sf::Image image;
		sf::Vector2u size;
		sf::Vector2u spawn;

		bool cleared = false;
		float levelTime = 0;
		float pauseTime = 0;
		sf::Clock levelClock;
		sf::Clock pauseClock;

		Map();
		Map(int mapIndex);
		void resetTime();
		void draw(sf::RenderWindow &window, sf::View view, bool paused);
};
