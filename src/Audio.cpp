#include "headers/Audio.hpp"

#define titleTrackVolume 50

#define startupVolume   55
#define clickVolume     40
#define jumpVolume      20
#define bounceVolume    10
#define hurtVolume      45
#define spawnVolume     15
#define shootVolume     10
#define explosionVolume 10

Audio::Audio()
{
	// music
	titleTrack.openFromFile("assets/audio/music/slimey.ogg");
	titleTrack.setVolume(titleTrackVolume);
	titleTrack.setLoop(true);

	// SFX
	startupBuffer.loadFromFile("assets/audio/sfx/startup.wav");
	startup.setBuffer(startupBuffer);
	startup.setVolume(startupVolume);

	clickBuffer.loadFromFile("assets/audio/sfx/click.wav");
	click.setBuffer(clickBuffer);
	click.setVolume(clickVolume);

	jumpBuffer.loadFromFile("assets/audio/sfx/jump.wav");
	jump.setBuffer(jumpBuffer);
	jump.setVolume(jumpVolume);

	bounceBuffer.loadFromFile("assets/audio/sfx/bounce.wav");
	bounce.setBuffer(bounceBuffer);
	bounce.setVolume(bounceVolume);

	hurtBuffer.loadFromFile("assets/audio/sfx/hurt.wav");
	hurt.setBuffer(hurtBuffer);
	hurt.setVolume(hurtVolume);

	spawnBuffer.loadFromFile("assets/audio/sfx/spawn.wav");
	spawn.setBuffer(spawnBuffer);
	spawn.setVolume(spawnVolume);

	shootBuffer.loadFromFile("assets/audio/sfx/shoot.wav");
	shoot.setBuffer(shootBuffer);
	shoot.setVolume(shootVolume);

	explosionBuffer.loadFromFile("assets/audio/sfx/explosion.wav");
	explosion.setBuffer(explosionBuffer);
	explosion.setVolume(explosionVolume);
}

void Audio::updateMusicVolume(bool notMuted, float volume)
{
	if (notMuted)
	{
		titleTrack.setVolume(titleTrackVolume * volume);
	}
	else
	{
		titleTrack.setVolume(0);
	}
}

void Audio::updateSFXVolume(bool notMuted, float volume)
{
	if (notMuted)
	{
		  startup.setVolume(startupVolume   * volume);
		    click.setVolume(clickVolume     * volume);
		     jump.setVolume(jumpVolume      * volume);
		   bounce.setVolume(bounceVolume    * volume);
		     hurt.setVolume(hurtVolume      * volume);
		    spawn.setVolume(spawnVolume     * volume);
		    shoot.setVolume(shootVolume     * volume);
		explosion.setVolume(explosionVolume * volume);
	}
	else
	{
		  startup.setVolume(0);
		    click.setVolume(0);
		     jump.setVolume(0);
		   bounce.setVolume(0);
		     hurt.setVolume(0);
		    spawn.setVolume(0);
		    shoot.setVolume(0);
		explosion.setVolume(0);
	}
}
