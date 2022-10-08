#include <SFML/Graphics.hpp>
#include <cmath>

#include "headers/Collider.hpp"

sf::FloatRect getTileHitbox(sf::Vector3i tile, sf::Vector2i tileCoord)
{
	if (tile == sawbladeTile)
	{
		return sf::FloatRect(tileCoord.x * tilesize + 1, tileCoord.y * tilesize + 1, tilesize - 2, tilesize - 2);
	}

	return sf::FloatRect(tileCoord.x * tilesize, tileCoord.y * tilesize, tilesize, tilesize);
}

Collision::Collision(Direction _direction, sf::Vector2i _position)
{
	direction = _direction;
	position  = _position;
}

Collider::Collider()
{}

Collider::Collider(sf::Sprite *_sprite, mapVector *_map, sf::Vector2u _mapSize)
{
	 sprite = _sprite;
	    map = _map;
	mapSize = _mapSize;
}

sf::Vector3i Collider::getTile(unsigned int x, unsigned int y)
{
	return map->at(x).at(y);
}

sf::FloatRect Collider::getHitbox()
{
	return sf::FloatRect(position.x, position.y, size.x, size.y);
}

sf::Vector2f Collider::getCenter()
{
	return {position.x + size.x / 2, position.y + size.y / 2};
}

void Collider::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Collider::updatePosition()
{
	position += velocity;
}

bool Collider::validCoord(sf::Vector2i coord)
{
	return (coord.x >= 0 && coord.x < mapSize.x
	        &&
	        coord.y >= 0 && coord.y < mapSize.y);
}

bool Collider::validCollisionTile(sf::Vector2i coord)
{
	sf::Vector3i tile = map->at(coord.x).at(coord.y);
	int tileset = tile.x;

	return (!(tileset == 0 || tile == exitTile));
}

void Collider::checkCollision()
{
	sf::Vector2f delta = velocity;
	sf::Vector2f colliderPosition;
	sf::FloatRect colliderHitbox;
	sf::FloatRect     tileHitbox;
	sf::Vector2i colliderCoord;
	sf::Vector2i     tileCoord;
	sf::Vector2u colliderIntersections;

	hitUp    = false;
	hitDown  = false;
	hitLeft  = false;
	hitRight = false;

	collisions.clear();

	while (delta.x != 0 || delta.y != 0)
	{
		////////////////////////
		// VERTICAL COLLISION //
		////////////////////////
		if (delta.y != 0)
		{
			if (delta.y > 0)
			{
				delta.y--;
				if (delta.y < 0)
				{
					delta.y = 0;
				}
			}
			else if (delta.y < 0)
			{
				delta.y++;
				if (delta.y > 0)
				{
					delta.y = 0;
				}
			}

			colliderPosition = position - delta;
			colliderHitbox = sf::FloatRect(colliderPosition.x, colliderPosition.y, size.x, size.y);
			colliderCoord.x = std::floor(colliderPosition.x / tilesize);
			colliderCoord.y = std::floor(colliderPosition.y / tilesize);
			colliderIntersections.x = std::floor((colliderPosition.x + size.x) / tilesize) - colliderCoord.x;
			colliderIntersections.y = std::floor((colliderPosition.y + size.y) / tilesize) - colliderCoord.y;

			// Up
			if (velocity.y < 0)
			{
				for (unsigned int x = 0; x <= colliderIntersections.x; x++)
				{
					tileCoord.x = colliderCoord.x + x;
					tileCoord.y = colliderCoord.y;
					if (!validCoord(tileCoord)) { continue; }
					tileHitbox = getTileHitbox(getTile(tileCoord.x, tileCoord.y), tileCoord);

					if (tileCoord.x == 0)
					{
						/*
						fix for a stupid bug
						*/
						colliderHitbox.left += tilesize;
						    tileHitbox.left += tilesize;
					}

					if (validCollisionTile(tileCoord) && colliderHitbox.intersects(tileHitbox))
					{
						hitUp = true;
						collisions.push_back(Collision(Up, tileCoord));
						position.y = tileCoord.y * tilesize + tilesize;
					}

					if (tileCoord.x == 0)
					{
						/*
						when the x-coord of the tile we're collision checking against is equal to zero some weird shit occurs
						checking for an intersection between the collider and a tile returns true when it shouldn't
						*/
						colliderHitbox.left -= tilesize;
						    tileHitbox.left -= tilesize;
					}
				}
			}
			// Down
			else if (velocity.y > 0)
			{
				for (unsigned int x = 0; x <= colliderIntersections.x; x++)
				{
					tileCoord.x = colliderCoord.x + x;
					tileCoord.y = colliderCoord.y + colliderIntersections.y;
					if (!validCoord(tileCoord)) { continue; }
					tileHitbox = getTileHitbox(getTile(tileCoord.x, tileCoord.y), tileCoord);

					if (tileCoord.x == 0)
					{
						/*
						i fixed it by moving the hitboxes a bit to the right (for the check) and then back again
						the move distance doesn't matter as long as the x-coord doesn't stay zero
						*/
						colliderHitbox.left += tilesize;
						    tileHitbox.left += tilesize;
					}

					if (validCollisionTile(tileCoord) && colliderHitbox.intersects(tileHitbox))
					{
						hitDown = true;
						collisions.push_back(Collision(Down, tileCoord));
						position.y = tileCoord.y * tilesize - size.y;
					}

					if (tileCoord.x == 0)
					{
						/*
						i was too stupid to figure out why this happens so i came up with this scuffed solution
						in the end it works, so who am i to complain
						*/
						colliderHitbox.left -= tilesize;
						    tileHitbox.left -= tilesize;
					}
				}
			}

			if (hitUp || hitDown)
			{
				delta.y = 0;
			}
		}

		//////////////////////////
		// HORIZONTAL COLLISION //
		//////////////////////////
		if (delta.x != 0)
		{
			if (delta.x > 0)
			{
				delta.x--;
				if (delta.x < 0)
				{
					delta.x = 0;
				}
			}
			else if (delta.x < 0)
			{
				delta.x++;
				if (delta.x > 0)
				{
					delta.x = 0;
				}
			}

			colliderPosition = position - delta;
			colliderHitbox = sf::FloatRect(colliderPosition.x, colliderPosition.y, size.x, size.y);
			colliderCoord.x = std::floor(colliderPosition.x / tilesize);
			colliderCoord.y = std::floor(colliderPosition.y / tilesize);
			colliderIntersections.x = std::floor((colliderPosition.x + size.x) / tilesize) - colliderCoord.x;
			colliderIntersections.y = std::floor((colliderPosition.y + size.y) / tilesize) - colliderCoord.y;

			// Left
			if (velocity.x < 0)
			{
				for (unsigned int y = 0; y <= colliderIntersections.y; y++)
				{
					tileCoord.x = colliderCoord.x;
					tileCoord.y = colliderCoord.y + y;
					if (!validCoord(tileCoord)) { continue; }
					tileHitbox = getTileHitbox(getTile(tileCoord.x, tileCoord.y), tileCoord);

					if (validCollisionTile(tileCoord) && colliderHitbox.intersects(tileHitbox))
					{
						hitLeft = true;
						collisions.push_back(Collision(Left, tileCoord));
						position.x = tileCoord.x * tilesize + tilesize;
					}
				}
			}
			// Right
			else if (velocity.x > 0)
			{
				for (unsigned int y = 0; y <= colliderIntersections.y; y++)
				{
					tileCoord.x = colliderCoord.x + colliderIntersections.x;
					tileCoord.y = colliderCoord.y + y;
					if (!validCoord(tileCoord)) { continue; }
					tileHitbox = getTileHitbox(getTile(tileCoord.x, tileCoord.y), tileCoord);

					if (validCollisionTile(tileCoord) && colliderHitbox.intersects(tileHitbox))
					{
						hitRight = true;
						collisions.push_back(Collision(Right, tileCoord));
						position.x = tileCoord.x * tilesize - size.x;
					}
				}
			}

			if (hitLeft || hitRight)
			{
				delta.x = 0;
			}
		}
	}
}

void Collider::handleCollision()
{}

void Collider::update()
{
	updatePosition();
	checkCollision();
	handleCollision();
}

void Collider::updateSprite()
{
}

void Collider::draw(sf::RenderWindow *window, sf::FloatRect viewport, bool paused)
{
	updateSprite();
	if (!getHitbox().intersects(viewport)) { return; }
	sprite->setPosition(position);
	window->draw(*sprite);
}
