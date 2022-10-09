#pragma once

#include <SFML/Audio.hpp>

struct Audio
{
	// music

	// SFX
	sf::SoundBuffer clickBuffer;
	sf::Sound click;

	sf::SoundBuffer jumpBuffer;
	sf::Sound jump;

	sf::SoundBuffer bounceBuffer;
	sf::Sound bounce;

	sf::SoundBuffer hurtBuffer;
	sf::Sound hurt;

	sf::SoundBuffer spawnBuffer;
	sf::Sound spawn;

	sf::SoundBuffer shootBuffer;
	sf::Sound shoot;

	sf::SoundBuffer explosionBuffer;
	sf::Sound explosion;

	Audio();

	void updateMusicVolume(bool notMuted);
	void updateSFXVolume(bool notMuted);
};
