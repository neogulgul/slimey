#include <SFML/Graphics.hpp>

#include "headers/Sprites.hpp"

Sprites::Sprites()
{
	// textures
	slimeyTexture         .loadFromFile("assets/textures/slimey.png");
	slimeyAsleepTexture   .loadFromFile("assets/textures/slimey-asleep.png");
	slimeyAwakeTexture    .loadFromFile("assets/textures/slimey-awake.png");
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
	bookTexture           .loadFromFile("assets/textures/book.png");
	xMarkTexture          .loadFromFile("assets/textures/x-mark.png");
	creditsTexture        .loadFromFile("assets/textures/credits.png");

	// sprites
	slimey         .setTexture(slimeyTexture);
	slimeyAsleep   .setTexture(slimeyAsleepTexture);
	slimeyAwake    .setTexture(slimeyAwakeTexture);
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
	book           .setTexture(bookTexture);
	xMark          .setTexture(xMarkTexture);
	credits        .setTexture(creditsTexture);

	tilesets = {
		&tilesetNormal,
		&tilesetIce,
		&tilesetBounce,
		&tilesetOther
	};

	amountOfTilesets = tilesets.size();
}

void Sprites::resetScale()
{
	slimey         .setScale(1, 1);
	slimeyColorless.setScale(1, 1);
	slimeyFrames   .setScale(1, 1);
	slimeyDeath    .setScale(1, 1);
	offscreenCircle.setScale(1, 1);
	tilesetNormal  .setScale(1, 1);
	tilesetIce     .setScale(1, 1);
	tilesetBounce  .setScale(1, 1);
	tilesetOther   .setScale(1, 1);
	exitSign       .setScale(1, 1);
	sawblade       .setScale(1, 1);
	bullet         .setScale(1, 1);
	bulletExplosion.setScale(1, 1);
}
