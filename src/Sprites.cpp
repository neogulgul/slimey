#include <SFML/Graphics.hpp>

#include "headers/Sprites.hpp"

Sprites::Sprites()
{
	// textures
	slimeyTexture         .loadFromFile("assets/textures/slimey.png");
	slimeyColorlessTexture.loadFromFile("assets/textures/slimey-colorless.png");
	slimeyFramesTexture   .loadFromFile("assets/textures/slimey-frames.png");
	offscreenCircleTexture.loadFromFile("assets/textures/offscreen-circle.png");
	tilesetNormalTexture  .loadFromFile("assets/textures/tilesets/normal.png");
	tilesetIceTexture     .loadFromFile("assets/textures/tilesets/ice.png");
	tilesetBounceTexture  .loadFromFile("assets/textures/tilesets/bounce.png");
	tilesetOtherTexture   .loadFromFile("assets/textures/tilesets/other.png");
	exitSignTexture       .loadFromFile("assets/textures/exit-sign.png");
	sawbladeTexture       .loadFromFile("assets/textures/sawblade.png");

	// sprites
	slimey         .setTexture(slimeyTexture);
	slimeyColorless.setTexture(slimeyColorlessTexture);
	slimeyFrames   .setTexture(slimeyFramesTexture);
	offscreenCircle.setTexture(offscreenCircleTexture);
	tilesetNormal  .setTexture(tilesetNormalTexture);
	tilesetIce     .setTexture(tilesetIceTexture);
	tilesetBounce  .setTexture(tilesetBounceTexture);
	tilesetOther   .setTexture(tilesetOtherTexture);
	exitSign       .setTexture(exitSignTexture);
	sawblade       .setTexture(sawbladeTexture);

	tilesets = {
		&tilesetNormal,
		&tilesetIce,
		&tilesetBounce,
		&tilesetOther
	};
}
