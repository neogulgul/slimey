#include <SFML/Graphics.hpp>

#include "headers/Sprites.hpp"

Sprites::Sprites()
{
	// textures
	slimeyTexture         .loadFromFile("assets/textures/slimey.png");
	slimeyColorlessTexture.loadFromFile("assets/textures/slimey-colorless.png");
	slimeyFramesTexture   .loadFromFile("assets/textures/slimey-frames.png");
	slimeyDeathTexture    .loadFromFile("assets/textures/slimey-death.png");
	offscreenCircleTexture.loadFromFile("assets/textures/offscreen-circle.png");
	tilesetNormalTexture  .loadFromFile("assets/textures/tilesets/normal.png");
	tilesetIceTexture     .loadFromFile("assets/textures/tilesets/ice.png");
	tilesetBounceTexture  .loadFromFile("assets/textures/tilesets/bounce.png");
	tilesetOtherTexture   .loadFromFile("assets/textures/tilesets/other.png");
	exitSignTexture       .loadFromFile("assets/textures/exit-sign.png");
	sawbladeTexture       .loadFromFile("assets/textures/sawblade.png");
	bulletTexture         .loadFromFile("assets/textures/bullet.png");
	bulletExplosionTexture.loadFromFile("assets/textures/bullet-explosion.png");

	// sprites
	slimey         .setTexture(slimeyTexture);
	slimeyColorless.setTexture(slimeyColorlessTexture);
	slimeyFrames   .setTexture(slimeyFramesTexture);
	slimeyDeath    .setTexture(slimeyDeathTexture);
	offscreenCircle.setTexture(offscreenCircleTexture);
	tilesetNormal  .setTexture(tilesetNormalTexture);
	tilesetIce     .setTexture(tilesetIceTexture);
	tilesetBounce  .setTexture(tilesetBounceTexture);
	tilesetOther   .setTexture(tilesetOtherTexture);
	exitSign       .setTexture(exitSignTexture);
	sawblade       .setTexture(sawbladeTexture);
	bullet         .setTexture(bulletTexture);
	bulletExplosion.setTexture(bulletExplosionTexture);

	tilesets = {
		&tilesetNormal,
		&tilesetIce,
		&tilesetBounce,
		&tilesetOther
	};
}
