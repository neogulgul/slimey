#pragma once

#include "Collider.hpp"

struct Player : Collider
{
	sf::Vector2i spawn;
	sf::Vector2u exit;

	int width  = 14;
	int height = 12;

	sf::Vector2f terminalVelocity;

	float horizontalTerminalVelocity = 2.0;
	float verticalTerminalVelocity   = 6.0;
	float acceleration = 0.3;
	float deceleration = 0.2;

	bool up    = false;
	bool down  = false;
	bool left  = false;
	bool right = false;

	float jumpForce    = 5;
	bool jump          = false;
	bool jumped        = false;
	int jumpTimer      = 0;
	int jumpFrames     = 11;
	int preJumpTimer   = 0;
	int preJumpFrames  = 10;
	int postJumpTimer  = 0;
	int postJumpFrames = 5;

	bool onGround   = false;
	bool onIce      = false;

	bool alive = true;

	int animationFrameCount    = 2;
	int animationFrameDuration = 15;
	Animation animation;

	Player();
	Player(sf::Sprite *_sprite, mapVector *_map, sf::Vector2u _mapSize, sf::Vector2i _spawn, sf::Vector2u _exit);

	void place(int x, int y);
	void updateInput();
	void handleInput();
	virtual void updatePosition();
	virtual void handleCollision();
	virtual void draw(sf::RenderWindow *window);
};
