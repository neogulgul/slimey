#pragma once

#include "Bullet.hpp"
#include "Global.hpp"

struct Turret
{
	sf::Vector2f bulletSpawn;
	Direction direction;

	Turret(sf::Vector2f bulletSpawn, Direction direction);
	void shoot(std::vector<Bullet> &bulletVector);
};
