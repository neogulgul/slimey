#include <SFML/Graphics.hpp>

#include "headers/Player.hpp"

Player::Player()
{}

Player::Player(sf::Sprite *_sprite, mapVector *_map, sf::Vector2u _mapSize, sf::Vector2i _spawn, sf::Vector2u _exit) : Collider(_sprite, _map, _mapSize)
{
	spawn = _spawn;
	 exit = _exit;

	size.x = width;
	size.y = height;

	terminalVelocity.x = horizontalTerminalVelocity;
	terminalVelocity.y =   verticalTerminalVelocity;

	animation = Animation(animationFrameCount, animationFrameDuration);

	sprite->setTextureRect(sf::IntRect(0, 0, size.x, size.y));

	place(spawn.x, spawn.y);
}

void Player::place(int x, int y)
{
	position.x = x * (int)tilesize + 1;
	position.y = y * (int)tilesize + 4;
	velocity.x = 0;
	velocity.y = 0;
}

void Player::updateInput()
{
	using key = sf::Keyboard;

	if (pressing(key::W) || pressing(key::Up))
	{ up = true; }
	else
	{ up = false; }

	if (pressing(key::S) || pressing(key::Down))
	{ down = true; }
	else
	{ down = false; }

	if (pressing(key::A) || pressing(key::Left))
	{ left = true; }
	else
	{ left = false; }

	if (pressing(key::D) || pressing(key::Right))
	{ right = true; }
	else
	{ right = false; }

	if (pressing(key::R))
	{
		place(spawn.x, spawn.y);
	}
}

void Player::handleInput()
{
	if (left && !right && !down)
	{
		velocity.x -= acceleration;
	}
	else if (right && !left && !down)
	{
		velocity.x += acceleration;
	}
	else
	{
		if (velocity.x > 0)
		{
			velocity.x -= deceleration;
			if (velocity.x < 0)
			{
				velocity.x = 0;
			}
		}
		else if (velocity.x < 0)
		{
			velocity.x += deceleration;
			if (velocity.x > 0)
			{
				velocity.x = 0;
			}
		}
	}

	if (onGround && pressing(sf::Keyboard::Space))
	{
		velocity.y -= jumpForce; // todo: dynamic jump
	}

	if (down)
	{ velocity.y += GRAVITY * 2; }
	else
	{ velocity.y += GRAVITY; }

	velocity.x = std::clamp(velocity.x, -terminalVelocity.x, terminalVelocity.x);
	velocity.y = std::clamp(velocity.y, -terminalVelocity.y, terminalVelocity.y);
}

void Player::updatePosition()
{
	updateInput();
	handleInput();
	Collider::updatePosition();
}

void Player::handleCollision()
{
	onGround = false;

	if (hitDown)
	{
		onGround = true;
	}

	if (position.y > mapSize.y * tilesize)
	{
		// todo: death
	}
	else if (hitUp || hitDown)
	{
		velocity.y = 0;
	}

	if (position.x < 0)
	{
		position.x = 0;
		velocity.x = 0;
	}
	else
	if (position.x > mapSize.x * tilesize - size.x)
	{
		position.x = mapSize.x * tilesize - size.x;
		velocity.x = 0;
	}
	else if (hitLeft || hitRight)
	{
		velocity.x = 0;
	}
}

void Player::draw(sf::RenderWindow *window)
{
	animation.update();

	int poo = 0;

	if (down)
	{
		poo = 2;
	}
	else if (left && !right)
	{
		poo = 3;
	}
	else if (right && !left)
	{
		poo = 4;
	}
	else if (up)
	{
		poo = 1;
	}

	sprite->setTextureRect(sf::IntRect(size.x * animation.frame, size.y * poo, size.x, size.y));

	Collider::draw(window);
}
