#include <SFML/Graphics.hpp>
#include <algorithm>
#include <cmath>

#include "headers/Collider.hpp"

#include <iostream>

void printHitbox(sf::FloatRect hitbox)
{
	std::cout << "top: " << hitbox.top << ", left: " << hitbox.left << ", width: " << hitbox.width << ", height: " << hitbox.height << "\n";
}

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
			colliderHitbox = sf::FloatRect((float)colliderPosition.x, (float)colliderPosition.y, (float)size.x, (float)size.y);
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
					tileHitbox = sf::FloatRect((float)tileCoord.x * (float)tilesize, (float)tileCoord.y * (float)tilesize, (float)tilesize, (float)tilesize);
					// if (!colliderHitbox.intersects(tileHitbox)) { break; }
					// if (colliderPosition.y + size.y > tileCoord.y * tilesize + tilesize && colliderHitbox.intersects(tileHitbox) && validCollisionTile(tileCoord))
					if (validCollisionTile(tileCoord) && colliderHitbox.intersects(tileHitbox)
					    && colliderPosition.y + size.y >= tileCoord.y * tilesize + tilesize
					    && colliderPosition.x < tileCoord.x * tilesize + tilesize)
					{
						// std::cout << "----- ----- ----- ----- -----" << "\n";
						// std::cout << "Up" << "\n";
						// printHitbox(colliderHitbox);
						// printHitbox(tileHitbox);
						// std::cout << (colliderHitbox.intersects(tileHitbox)) << "\n";
						hitUp = true;
						position.y = tileCoord.y * tilesize + tilesize;
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
					tileHitbox = sf::FloatRect((float)tileCoord.x * (float)tilesize, (float)tileCoord.y * (float)tilesize, (float)tilesize, (float)tilesize);
					// if (!colliderHitbox.intersects(tileHitbox)) { break; }
					// if (colliderPosition.y < tileCoord.y * tilesize && colliderPosition.x < tileCoord.x * tilesize + tilesize && colliderHitbox.intersects(tileHitbox) && validCollisionTile(tileCoord))
					if (validCollisionTile(tileCoord) && colliderHitbox.intersects(tileHitbox)
					    && colliderPosition.y < tileCoord.y * tilesize
					    && ((float)colliderPosition.x < (float)tileCoord.x * (float)tilesize + (float)tilesize))
					{
						std::cout << (float)colliderPosition.x << " < " << (float)tileCoord.x * (float)tilesize + (float)tilesize << ": "
						<< (         (float)colliderPosition.x      <      (float)tileCoord.x * (float)tilesize + (float)tilesize) << "\n";
						std::cout << (float)colliderPosition.x << " == " << (float)tileCoord.x * (float)tilesize + (float)tilesize << ": "
						<< (         (float)colliderPosition.x      ==      (float)tileCoord.x * (float)tilesize + (float)tilesize) << "\n";
						// std::cout << "----- ----- ----- ----- -----" << "\n";
						// std::cout << "Down" << "\n";
						// printHitbox(colliderHitbox);
						// printHitbox(tileHitbox);
						// std::cout << (colliderHitbox.intersects(tileHitbox)) << "\n";
						hitDown = true;
						position.y = tileCoord.y * tilesize - size.y;
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
			colliderHitbox = sf::FloatRect((float)colliderPosition.x, (float)colliderPosition.y, (float)size.x, (float)size.y);
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
					tileHitbox = sf::FloatRect((float)tileCoord.x * (float)tilesize, (float)tileCoord.y * (float)tilesize, (float)tilesize, (float)tilesize);
					// if (!colliderHitbox.intersects(tileHitbox)) { break; }
					// if (colliderPosition.x + size.x > tileCoord.x * tilesize + tilesize && colliderHitbox.intersects(tileHitbox) && validCollisionTile(tileCoord))
					if (validCollisionTile(tileCoord) && colliderHitbox.intersects(tileHitbox)
					    && colliderPosition.x > tileCoord.x * tilesize - size.x)
					{
						// std::cout << "----- ----- ----- ----- -----" << "\n";
						// std::cout << "Left" << "\n";
						// printHitbox(colliderHitbox);
						// printHitbox(tileHitbox);
						// std::cout << (colliderHitbox.intersects(tileHitbox)) << "\n";
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
					tileHitbox = sf::FloatRect((float)tileCoord.x * (float)tilesize, (float)tileCoord.y * (float)tilesize, (float)tilesize, (float)tilesize);
					// if (!colliderHitbox.intersects(tileHitbox)) { break; }
					// if (colliderPosition.x < tileCoord.x * tilesize && colliderHitbox.intersects(tileHitbox) && validCollisionTile(tileCoord))
					if (validCollisionTile(tileCoord) && colliderHitbox.intersects(tileHitbox)
					    && colliderPosition.x < tileCoord.x * tilesize)
					{
						// std::cout << "----- ----- ----- ----- -----" << "\n";
						// std::cout << "Right" << "\n";
						// printHitbox(colliderHitbox);
						// printHitbox(tileHitbox);
						// std::cout << (colliderHitbox.intersects(tileHitbox)) << "\n";
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
