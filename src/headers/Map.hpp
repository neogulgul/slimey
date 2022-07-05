#pragma once

#include "Bullet.hpp"
#include "Turret.hpp"

class Map {
	public:
		sf::Image image;
		sf::Vector2u size;
		sf::Vector2u spawn;

		std::vector<Turret> turretVector;
		std::vector<Bullet> bulletVector;

		int index;
		bool valid = false;

		int turretTimer = 0;
		int turretFrames = 120;

		float clearTime = 0;
		bool cleared = false;

		Map();
		Map(int mapIndex);
		void resetTime();
		void update();
		void draw(sf::RenderWindow &window, sf::View view, sf::Sprite &tilesetSprite, sf::Sprite bulletSprite);
};
