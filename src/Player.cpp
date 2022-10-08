#include <SFML/Graphics.hpp>
#include <algorithm>

#include "headers/Player.hpp"

Player::Player()
{}

Player::Player(sf::Sprite *_sprite, sf::Sprite *_slimeyDeath, sf::Sprite *_offscreenCircle, bool *_levelCleared, mapVector *_map, sf::Vector2u _mapSize, sf::Vector2i _spawn, sf::Vector2u _exit)
: Collider(_sprite, _map, _mapSize)
{
	slimeyDeath     = _slimeyDeath;
	offscreenCircle = _offscreenCircle;
	levelCleared    = _levelCleared;

	spawn = _spawn;
	exit  = _exit;

	size.x = width;
	size.y = height;

	animation      = Animation(animationFrameCount, animationFrameDuration);
	deathAnimation = Animation(deathAnimationFrameCount, deathAnimationFrameDuration);

	sprite->setTextureRect(sf::IntRect(0, 0, size.x, size.y));

	place(spawn.x, spawn.y);
	alive = true;
}

void Player::place(int x, int y)
{
	position.x = x * (int)tilesize + 1;
	position.y = y * (int)tilesize + 4;
	velocity.x = 0;
	velocity.y = 0;
}

void Player::death()
{
	alive = false;
	velocity = {0, 0};
}

void Player::levelClear()
{
	*levelCleared = true;
	up    = false;
	down  = false;
	left  = false;
	right = false;
}

void Player::updateInput()
{
	if (*levelCleared) { return; }

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
		death();
	}
}

void Player::handleInput()
{
	if (left && !right && !down && velocity.x >= -maxMoveVelocity)
	{
		velocity.x -= acceleration;
		if (velocity.x < -maxMoveVelocity) { velocity.x = -maxMoveVelocity; }
	}
	else if (right && !left && !down && velocity.x <= maxMoveVelocity)
	{
		velocity.x += acceleration;
		if (velocity.x > maxMoveVelocity) { velocity.x = maxMoveVelocity; }
	}
	else if (onGround && !onIce)
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

	// preventing unnecessarily small numbers
	if (velocity.x > -0.1 && velocity.x < 0.1) { velocity.x = 0; }
	if (velocity.y > -0.1 && velocity.y < 0.1) { velocity.y = 0; }

	velocity.x = std::clamp(velocity.x, -terminalVelocity, terminalVelocity);
	velocity.y = std::clamp(velocity.y, -terminalVelocity, terminalVelocity);
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

	if (!onGround && jump && !jumped && !jumpedEarly && preJumpTimer == 0)
	{
		preJumpTimer = preJumpFrames;
	}

	if (!jump)
	{
		jumpTimer = 0;
		jumped = false;
		jumpedEarly = false;
	}

	if (onGround && (jump && !jumped && !jumpedEarly || preJumpTimer > 0) || !onGround && jump && !jumped && postJumpTimer > 0)
	{
		jumped = true;
		jumpTimer = jumpFrames;
	}

	if (jumpTimer > 0)
	{
		velocity.y = -jumpForce;
		jumpTimer--;
	}
}

void Player::updatePosition()
{
	if (resurrected)
	{
		alive       = true;
		resurrected = false;
	}
	if (!alive) { return; }
	updateInput();
	handleInput();
	Collider::updatePosition();
}

void Player::handleCollision()
{
	onIce = false;

	hitVerticalBounce   = false;
	hitHorizontalBounce = false;

	for (Collision collision : collisions)
	{
		sf::Vector3i tile = getTile(collision.position.x, collision.position.y);
		int tileset = tile.x;
		if (tile == sawbladeTile)
		{
			death();
			return;
		}
		else if (tileset == Ice)
		{
			if (collision.direction == Down)
			{
				onIce = true;
			}
		}
		else if (tileset == Bounce)
		{
			if (!hitVerticalBounce && (collision.direction == Up || collision.direction == Down))
			{
				if (collision.direction == Up && velocity.y > -baseBounce)
				{
					velocity.y = -baseBounce;
				}
				else if (collision.direction == Down && velocity.y < baseBounce)
				{
					velocity.y = baseBounce;
				}
				velocity.y *= -bounciness;
				hitVerticalBounce = true;
			}
			else if (!hitHorizontalBounce && (collision.direction == Left || collision.direction == Right))
			{
				if (collision.direction == Left && velocity.x > -baseBounce)
				{
					velocity.x = -baseBounce;
				}
				else if (collision.direction == Right && velocity.x < baseBounce)
				{
					velocity.x = baseBounce;
				}
				velocity.x *= -bounciness;
				hitHorizontalBounce = true;
			}
		}
	}

	if (hitUp)
	{
		jumpTimer = 0;
	}
	if (hitDown && !hitVerticalBounce)
	{
		if (onGround) // if also touched ground last frame
		{
			jumpTimer = 0;
			preJumpTimer = 0;
			postJumpTimer = 0;
		}
		onGround = true;
	}
	else if (onGround) // if touched ground last frame but not current frame
	{
		if (!jump)
		{
			postJumpTimer = postJumpFrames;
		}
		onGround = false;
	}

	if (position.y > mapSize.y * tilesize)
	{
		position.y = mapSize.y * tilesize - size.y / 2;
		death();
		return;
	}
	else if (!hitVerticalBounce && (hitUp || hitDown))
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
	else if (!hitHorizontalBounce && (hitLeft || hitRight))
	{
		velocity.x = 0;
	}

	if (getTile(exit.x, exit.y) == exitTile
	    &&
	    getHitbox().intersects(sf::FloatRect(exit.x * tilesize + 6, exit.y * tilesize + 6, 4, 4)))
	{
		levelClear();
	}
}

void Player::updateSprite()
{
	if (alive)
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
	}
	else
	{
		bool lastFrameLastFrame = (deathAnimation.frame == deathAnimation.frameCount - 1);

		deathAnimation.update();

		bool firstFrameThisFrame = (deathAnimation.frame == 0);

		if (lastFrameLastFrame && firstFrameThisFrame)
		{
			if (resurrecting)
			{
				resurrecting = false;
				resurrected  = true;
			}
			else
			{
				place(spawn.x, spawn.y);
				resurrecting = true;
			}
		}

		if (resurrecting)
		{
			slimeyDeath->setTextureRect(sf::IntRect((deathAnimationFrameCount - 1 - deathAnimation.frame) * tilesize, 0, tilesize, tilesize));
		}
		else
		{
			slimeyDeath->setTextureRect(sf::IntRect(deathAnimation.frame * tilesize, 0, tilesize, tilesize));
		}

		if (alive) { deathAnimation.reset(); }
	}
}

void Player::draw(sf::RenderWindow *window, sf::FloatRect viewport, bool paused)
{
	if (!paused)
	{
		updateSprite();
	}
	if (alive)
	{
		if (!getHitbox().intersects(viewport))
		{
			sf::Vector2f spritePosition = position;

			if (position.y + size.y <= viewport.top)
			{
				spritePosition.y = viewport.top;
			}
			else if (position.y >= viewport.top + viewport.height)
			{
				spritePosition.y = viewport.top + viewport.height - size.y;
			}
			if (position.x + size.x <= viewport.left)
			{
				spritePosition.x = viewport.left;
			}
			else if (position.x >= viewport.left + viewport.width)
			{
				spritePosition.x = viewport.left + viewport.width - size.x;
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
	else if (!(resurrecting && *levelCleared))
	{
		slimeyDeath->setPosition({position.x + size.x / 2 - tilesize / 2, position.y + size.y / 2 - tilesize / 2});
		window->draw(*slimeyDeath);
	}
}
