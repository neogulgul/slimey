#include <SFML/Graphics.hpp>
#include "headers/SpriteManager.hpp"

SpriteManager::SpriteManager() {
	// loading textures
	this->playerTexture         .loadFromFile("resources/textures/slimey.png");
	this->playerDeathTexture    .loadFromFile("resources/textures/death.png");
	this->offscreenCircleTexture.loadFromFile("resources/textures/offscreen-circle.png");
	this->tilesetTexture        .loadFromFile("resources/textures/tileset.png");
	this->bulletTexture         .loadFromFile("resources/textures/bullet.png");
	this->bulletExplosionTexture.loadFromFile("resources/textures/bullet-explosion.png");
	// setting textures
	this->playerSprite          .setTexture(playerTexture);
	this->playerDeathSprite     .setTexture(playerDeathTexture);
	this->offscreenCircleSprite .setTexture(offscreenCircleTexture);
	this->tilesetSprite         .setTexture(tilesetTexture);
	this->bulletSprite          .setTexture(bulletTexture);
	this->bulletExplosionSprite .setTexture(bulletExplosionTexture);
}
