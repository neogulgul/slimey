#pragma once

#include "Bullet.hpp"
#include "global.hpp"

class Turret {
	public:
		sf::Vector2f bulletSpawn;
		Direction direction;

		Turret(sf::Vector2f bulletSpawn, Direction direction);
		void shoot(std::vector<Bullet> &bulletVector);
};
