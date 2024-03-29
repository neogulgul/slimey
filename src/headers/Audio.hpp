#pragma once

#include <SFML/Audio.hpp>

struct Audio
{
	// music
	sf::Music titleTrack;

	// SFX
	sf::SoundBuffer startupBuffer;
	sf::Sound startup;

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

	void updateMusicVolume(bool notMuted, float volume);
	void updateSFXVolume(bool notMuted, float volume);
};
