#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

#include "headers/Collider.hpp"

#include <iostream>

Collider::Collider()
{}

Collider::Collider(sf::Sprite *_sprite, mapVector *_map, sf::Vector2u _mapSize)
{
	 sprite = _sprite;
	    map = _map;
	mapSize = _mapSize;
}

void Collider::updatePosition()
{
	position += velocity;
}

bool Collider::validCollisionTile(sf::Vector2i coord)
{
	if (coord.x < 0 || coord.x >= mapSize.x
	    ||
	    coord.y < 0 || coord.y >= mapSize.y)
	{
		return false;
	}

	sf::Vector3i tile = map->at(coord.x).at(coord.y);
	int tileset = tile.x;

	if (tileset == 0 || tile == exitTile)
	{
		return false;
	}
	
	return true;
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
					tileHitbox = sf::FloatRect(tileCoord.x * tilesize, tileCoord.y * tilesize, tilesize, tilesize);

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
					tileHitbox = sf::FloatRect(tileCoord.x * tilesize, tileCoord.y * tilesize, tilesize, tilesize);

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
					tileHitbox = sf::FloatRect(tileCoord.x * tilesize, tileCoord.y * tilesize, tilesize, tilesize);
					if (validCollisionTile(tileCoord) && colliderHitbox.intersects(tileHitbox))
					{
						hitLeft = true;
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
					tileHitbox = sf::FloatRect(tileCoord.x * tilesize, tileCoord.y * tilesize, tilesize, tilesize);
					if (validCollisionTile(tileCoord) && colliderHitbox.intersects(tileHitbox))
					{
						hitRight = true;
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

void Collider::draw(sf::RenderWindow *window)
{
	sprite->setPosition(position);
	window->draw(*sprite);
}
