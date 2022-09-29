#include "headers/Audio.hpp"

Audio::Audio()
{
	// music

	// sfx
	clickBuffer.loadFromFile("assets/audio/sfx/click.wav");
	click.setBuffer(clickBuffer);
	click.setVolume(50);

	jumpBuffer.loadFromFile("assets/audio/sfx/jump.wav");
	jump.setBuffer(jumpBuffer);
	jump.setVolume(30);

	bounceBuffer.loadFromFile("assets/audio/sfx/bounce.wav");
	bounce.setBuffer(bounceBuffer);
	bounce.setVolume(60);
}
