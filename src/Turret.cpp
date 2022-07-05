#include <SFML/Graphics.hpp>
#include "headers/Bullet.hpp"
#include "headers/global.hpp"
#include "headers/Turret.hpp"

Turret::Turret(sf::Vector2f bulletSpawn, Direction direction) {
	this->bulletSpawn = bulletSpawn;
	this->direction = direction;
}

void Turret::shoot(std::vector<Bullet> &bulletVector) {
	switch (this->direction) {
		case up:
			bulletVector.push_back(Bullet(this->bulletSpawn, sf::Vector2f(0, -bulletVelocity)));
			break;
		case down:
			bulletVector.push_back(Bullet(this->bulletSpawn, sf::Vector2f(0, bulletVelocity)));
			break;
		case left:
			bulletVector.push_back(Bullet(this->bulletSpawn, sf::Vector2f(-bulletVelocity, 0)));
			break;
		case right:
			bulletVector.push_back(Bullet(this->bulletSpawn, sf::Vector2f(bulletVelocity, 0)));
			break;
	}
}
