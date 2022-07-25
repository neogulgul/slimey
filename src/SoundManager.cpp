#include <SFML/Audio.hpp>
#include "headers/SoundManager.hpp"

SoundManager::SoundManager() {
	this->playerJumpBuffer .loadFromFile("resources/sounds/playerJump.wav");
	this->playerDeathBuffer.loadFromFile("resources/sounds/playerDeath.wav");
	this->bulletShotBuffer .loadFromFile("resources/sounds/bulletShot.wav");
	this->bulletHitBuffer  .loadFromFile("resources/sounds/bulletHit.wav");

	this->playerJump .setBuffer(playerJumpBuffer);
	this->playerDeath.setBuffer(playerDeathBuffer);
	this->bulletShot .setBuffer(bulletShotBuffer);
	this->bulletHit  .setBuffer(bulletHitBuffer);

	this->playerJump .setVolume(this->SFXVolume);
	this->playerDeath.setVolume(this->SFXVolume);
	this->bulletShot .setVolume(this->SFXVolume);
	this->bulletHit  .setVolume(this->SFXVolume);

	this->music.openFromFile("resources/sounds/music.wav");
	this->music.setVolume(this->musicVolume);
	this->music.setLoop(true);
}
