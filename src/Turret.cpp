#include "headers/Turret.hpp"

Turret::Turret(Direction _direction, sf::Vector2u coord)
{
	direction = _direction;

	switch (direction)
	{
		case Up:
			bulletSpawn.x = coord.x * tilesize + tilesize / 2 - bulletSize / 2;
			bulletSpawn.y = coord.y * tilesize - bulletSize;
			bulletVelocity.y = -bulletSpeed;
			break;

		case Down:
			bulletSpawn.x = coord.x * tilesize + tilesize / 2 - bulletSize / 2;
			bulletSpawn.y = coord.y * tilesize + tilesize;
			bulletVelocity.y = bulletSpeed;
			break;

		case Left:
			bulletSpawn.x = coord.x * tilesize - bulletSize;
			bulletSpawn.y = coord.y * tilesize + tilesize / 2 - bulletSize / 2;
			bulletVelocity.x = -bulletSpeed;
			break;

		case Right:
			bulletSpawn.x = coord.x * tilesize + tilesize;
			bulletSpawn.y = coord.y * tilesize + tilesize / 2 - bulletSize / 2;
			bulletVelocity.x = bulletSpeed;
			break;
	}
}

void Turret::shoot(std::vector<Bullet> *bullets, sf::Sprite *sprite, mapVector *map, sf::Vector2u mapSize, sf::Sprite *explosion, Player *player)
{
	bullets->push_back(Bullet(sprite, map, mapSize, explosion, player, bulletSpawn, bulletVelocity));
}
