#include "headers/Bullet.hpp"

Bullet::Bullet(sf::Sprite* _sprite, mapVector* _map, sf::Vector2u _mapSize, sf::Sprite* _explosion, Player* _player,
               sf::Vector2f _position, sf::Vector2f _velocity)
: Collider(_sprite, _map, _mapSize)
{
	explosion = _explosion;
	player    = _player;

	position = _position;
	velocity = _velocity;

	size = {bulletSize, bulletSize};

	explosionAnimation = Animation(explosionAnimationFrameCount, explosionAnimationFrameDuration);
}

void Bullet::explode()
{
	exploding = true;
	velocity = {0, 0};
}

void Bullet::handleCollision()
{
	if (exploding) { return; }
	if (getHitbox().intersects(player->getHitbox()))
	{
		player->death();
	}
	if (position.x > mapSize.x * tilesize ||
	    position.x < -bulletSize          ||
	    position.y > mapSize.y * tilesize ||
	    position.y < -bulletSize          ||
	      hitUp || hitDown                ||
	    hitLeft || hitRight)
	{
		explode();
		return;
	}
	for (Collision collision : collisions)
	{
		sf::Vector3i tile = getTile(collision.coord);
		if (tile == sawbladeTile)
		{
			explode();
			return;
		}
	}
}

void Bullet::updateSprite()
{
	if (exploding)
	{
		explosionAnimation.update();

		if (explosionAnimation.onLastFrame())
		{
			destroyed = true;
		}

		explosionRect = sf::IntRect(explosionAnimation.frame * bulletExplosionSize, 0, bulletExplosionSize, bulletExplosionSize);
	}
}

void Bullet::draw(sf::RenderWindow* window, sf::FloatRect viewport)
{
	if (!getHitbox().intersects(viewport)) { return; }
	if (exploding)
	{
		explosion->setPosition({position.x + bulletSize / 2 - bulletExplosionSize / 2, position.y + bulletSize / 2 - bulletExplosionSize / 2});
		explosion->setTextureRect(explosionRect);
		window->draw(*explosion);
	}
	else
	{
		sprite->setPosition(position);
		window->draw(*sprite);
	}
}
