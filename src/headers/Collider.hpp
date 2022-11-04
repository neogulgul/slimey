#pragma once

#include "Animation.hpp"
#include "DefaultMaps.hpp"
#include "Global.hpp"

struct Collision
{
	Direction direction;
	sf::Vector2i coord;
	bool invalid = false;

	Collision(Direction _direction, sf::Vector2i _coord);
};

struct Collider
{
	sf::Sprite* sprite;

	mapVector* map;
	sf::Vector2u mapSize;

	sf::Vector2f size;
	sf::Vector2f position;
	sf::Vector2f velocity;

	std::vector<Collision> collisions;

	bool hitUp    = false;
	bool hitDown  = false;
	bool hitLeft  = false;
	bool hitRight = false;

	Collider();
	Collider(sf::Sprite* _sprite, mapVector* _map, sf::Vector2u _mapSize);

	sf::Vector2f getCenter();
	sf::FloatRect getHitbox();
	sf::Vector3i getTile(sf::Vector2i coord);
	sf::FloatRect getTileHitbox(sf::Vector3i tile, sf::Vector2i tileCoord);

	        void setPosition(float x, float y);
	virtual void updatePosition();
	        bool validCoord(sf::Vector2i coord);
	        bool validCollisionTile(sf::Vector2i coord);
	        bool validSolidCollisionTile(sf::Vector2i coord);
	        void checkCollision();
	virtual void handleCollision();
	        void update();
	virtual void updateSprite();
	virtual void draw(sf::RenderWindow* window, sf::FloatRect viewport);
};
