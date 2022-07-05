#pragma once

class Map {
	public:
		sf::Image image;
		sf::Vector2u size;
		sf::Vector2u spawn;

		int index;
		bool valid = false;

		float clearTime = 0;
		bool cleared = false;

		Map();
		Map(int mapIndex);
		void resetTime();
		void draw(sf::RenderWindow &window, sf::View view, sf::Sprite &tilesetSprite, bool paused);
};
