#pragma once

#include "Animation.hpp"
#include "DefaultMaps.hpp"
#include "Global.hpp"

struct Collider
{
	sf::Sprite *sprite;

	mapVector *map;
	sf::Vector2u mapSize;

	sf::Vector2f size;
	sf::Vector2f position;
	sf::Vector2f velocity;

	std::vector<sf::Vector2i> collisions;

	bool hitUp    = false;
	bool hitDown  = false;
	bool hitLeft  = false;
	bool hitRight = false;

	Collider();
	Collider(sf::Sprite *_sprite, mapVector *_map, sf::Vector2u _mapSize);

	sf::Vector3i getTile(unsigned int x, unsigned int y);
	sf::FloatRect getHitbox();

	        void setPosition(float x, float y);
	virtual void updatePosition();
	        bool validCoord(sf::Vector2i coord);
	        bool validCollisionTile(sf::Vector2i coord);
	        void checkCollision();
	virtual void handleCollision();
	        void update();
	virtual void updateSprite();
	virtual void draw(sf::RenderWindow *window, sf::FloatRect viewPort, bool paused);
};
