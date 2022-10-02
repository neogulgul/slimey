#pragma once

#include "Animation.hpp"
#include "DefaultMaps.hpp"
#include "Global.hpp"

struct Collision
{
	Direction direction;
	sf::Vector2i position;

	Collision(Direction _direction, sf::Vector2i _position);
};

struct Collider
{
	sf::Sprite *sprite;

	mapVector *map;
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
	Collider(sf::Sprite *_sprite, mapVector *_map, sf::Vector2u _mapSize);

	sf::Vector3i getTile(unsigned int x, unsigned int y);
	sf::FloatRect getHitbox();
	sf::Vector2f getCenter();

	        void setPosition(float x, float y);
	virtual void updatePosition();
	        bool validCoord(sf::Vector2i coord);
	        bool validCollisionTile(sf::Vector2i coord);
	        void checkCollision();
	virtual void handleCollision();
	        void update();
	virtual void updateSprite();
	virtual void draw(sf::RenderWindow *window, sf::FloatRect viewport, bool paused);
};
