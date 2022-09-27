#include <SFML/Graphics.hpp>

#include "headers/Player.hpp"

#include <iostream>

Player::Player()
{}

Player::Player(sf::Sprite *_sprite, sf::Sprite *_offscreenCircle, mapVector *_map, sf::Vector2u _mapSize, sf::Vector2i _spawn, sf::Vector2u _exit)
: Collider(_sprite, _map, _mapSize)
{
	offscreenCircle = _offscreenCircle;

	spawn = _spawn;
	exit  = _exit;

	size.x = width;
	size.y = height;

	terminalVelocity.x = horizontalTerminalVelocity;
	terminalVelocity.y =   verticalTerminalVelocity;

	animation = Animation(animationFrameCount, animationFrameDuration);

	sprite->setTextureRect(sf::IntRect(0, 0, size.x, size.y));

	place(spawn.x, spawn.y);
}

void Player::death()
{
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

	if (pressing(key::J) || pressing(key::Space))
	{ jump = true; }
	else
	{ jump = false; }

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

	handleJump();

	if (down)
	{
		velocity.y += GRAVITY * 2;
	}
	else
	{
		velocity.y += GRAVITY;
	}

	velocity.x = std::clamp(velocity.x, -terminalVelocity.x, terminalVelocity.x);
	velocity.y = std::clamp(velocity.y, -terminalVelocity.y, terminalVelocity.y);
}

void Player::handleJump()
{
	if (preJumpTimer > 0)
	{
		preJumpTimer--;
		if (preJumpTimer == 0)
		{
			jumpedEarly = true;
		}
	}
	if (postJumpTimer > 0)
	{
		postJumpTimer--;
	}

	if (!onGround && jump && !jumpedEarly && preJumpTimer == 0)
	{
		preJumpTimer = preJumpFrames;
	}

	if (onGround && (preJumpTimer > 0 || jump && !jumped && !jumpedEarly) || !onGround && jump && !jumped && postJumpTimer > 0)
	{
		jumped = true;
		jumpTimer = jumpFrames;
	}

	if (jumpTimer > 0)
	{
		velocity.y = -jumpForce;
		if (!jump)
		{
			jumpTimer = 0;
		}
		else
		{
			jumpTimer--;
		}
	}

	if (!jump)
	{
		jumpTimer = 0;
		jumped = false;
		jumpedEarly = false;
	}
}

void Player::updatePosition()
{
	updateInput();
	handleInput();
	Collider::updatePosition();
}

void Player::handleCollision()
{
	if (hitUp)
	{
		jumpTimer = 0;
	}
	if (hitDown)
	{
		if (onGround)
		{
			preJumpTimer = 0;
			postJumpTimer = 0;
		}
		onGround = true;
	}
	else if (onGround == true) // if touched ground last frame but not current frame
	{
		postJumpTimer = postJumpFrames;
		onGround = false;
	}

	for (sf::Vector2i collision : collisions)
	{
		if (getTile(collision.x, collision.y) == sawbladeTile)
		{
			death();
			return;
		}
	}

	if (position.y > mapSize.y * tilesize)
	{
		death();
		return;
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

void Player::updateSprite()
{}

void Player::draw(sf::RenderWindow *window, sf::FloatRect viewPort, bool paused)
{
	animation.update();

	animationState = Idle;

	if (down)
	{
		animationState = CrouchingDown;
	}
	else if (left && !right)
	{
		animationState = MovingLeft;
	}
	else if (right && !left)
	{
		animationState = MovingRight;
	}
	else if (up)
	{
		animationState = LookingUp;
	}

	sprite->setTextureRect(sf::IntRect(size.x * animation.frame, size.y * animationState, size.x, size.y));

	if (!getHitbox().intersects(viewPort))
	{
		sf::Vector2f spritePosition = position;

		if (position.y + size.y <= viewPort.top)
		{
			spritePosition.y = viewPort.top;
		}
		else if (position.y >= viewPort.top + viewPort.height)
		{
			spritePosition.y = viewPort.top + viewPort.height - size.y;
		}
		if (position.x + size.x <= viewPort.left)
		{
			spritePosition.x = viewPort.left;
		}
		else if (position.x >= viewPort.left + viewPort.width)
		{
			spritePosition.x = viewPort.left + viewPort.width - size.x;
		}

		offscreenCircle->setPosition(spritePosition.x - offscreenCircle->getTexture()->getSize().x / 2 + size.x / 2, spritePosition.y - offscreenCircle->getTexture()->getSize().y / 2 + size.y / 2);
		window->draw(*offscreenCircle);

		sprite->setPosition(spritePosition);
	}
	else
	{
		sprite->setPosition(position);
	}

	window->draw(*sprite);
}
