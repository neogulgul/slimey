#pragma once

#include "Animation.hpp"
#include "Collider.hpp"
#include "Player.hpp"

#define bulletSize 4
#define bulletSpeed 1
#define bulletExplosionSize 8

struct Bullet : Collider
{
	Player* player;

	sf::Sprite* explosion;
	sf::IntRect explosionRect;
	Animation explosionAnimation;
	unsigned int explosionAnimationFrameCount    = 4;
	unsigned int explosionAnimationFrameDuration = 4;
	bool exploding = false;
	bool destroyed = false;

	Bullet(sf::Sprite* _sprite, mapVector* _map, sf::Vector2u _mapSize, sf::Sprite* _explosion, Player* _player,
	       sf::Vector2f _position, sf::Vector2f _velocity);

	void explode();

	virtual void handleCollision();
	virtual void updateSprite();
	virtual void draw(sf::RenderWindow* window, sf::FloatRect viewport, bool paused);
};
