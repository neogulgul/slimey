#pragma once

#include <SFML/Audio.hpp>

struct Audio
{
	// music

	// sfx
	sf::SoundBuffer clickBuffer;
	sf::Sound click;

	sf::SoundBuffer jumpBuffer;
	sf::Sound jump;

	sf::SoundBuffer bounceBuffer;
	sf::Sound bounce;

	Audio();
};
