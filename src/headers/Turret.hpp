#pragma once

#include "Bullet.hpp"
#include "Global.hpp"
#include "Player.hpp"

struct Turret
{
	Direction direction;
	sf::Vector2f bulletSpawn;
	sf::Vector2f bulletVelocity;

	Turret(Direction _direction, sf::Vector2u coord);
	void shoot(std::vector<Bullet> *bullets, sf::Sprite *sprite, mapVector *map, sf::Vector2u mapSize, sf::Sprite *explosion, Player *player);
};
