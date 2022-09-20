#pragma once

#include <SFML/Audio.hpp>

struct SoundManager
{
	// sound buffers
	sf::SoundBuffer playerJumpBuffer;
	sf::SoundBuffer playerDeathBuffer;
	sf::SoundBuffer bulletShotBuffer;
	sf::SoundBuffer bulletHitBuffer;
	// sounds
	sf::Sound playerJump;
	sf::Sound playerDeath;
	sf::Sound bulletShot;
	sf::Sound bulletHit;
	// music
	sf::Music music;

	float musicVolume = 80;
	float SFXVolume   = 80;

	bool playSFX   = true;
	bool playMusic = true;

	SoundManager();
};
