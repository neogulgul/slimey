#pragma once

struct Sprites
{
	// textures
	sf::Texture slimeyTexture;
	sf::Texture slimeyAsleepTexture;
	sf::Texture slimeyAwakeTexture;
	sf::Texture slimeyColorlessTexture;
	sf::Texture slimeyFramesTexture;
	sf::Texture slimeyDeathTexture;
	sf::Texture offscreenCircleTexture;
	sf::Texture tilesetNormalTexture;
	sf::Texture tilesetIceTexture;
	sf::Texture tilesetBounceTexture;
	sf::Texture tilesetOtherTexture;
	sf::Texture exitSignTexture;
	sf::Texture sawbladeTexture;
	sf::Texture bulletTexture;
	sf::Texture bulletExplosionTexture;
	sf::Texture bookTexture;
	sf::Texture xMarkTexture;

	// sprites
	sf::Sprite slimey;
	sf::Sprite slimeyAsleep;
	sf::Sprite slimeyAwake;
	sf::Sprite slimeyColorless;
	sf::Sprite slimeyFrames;
	sf::Sprite slimeyDeath;
	sf::Sprite offscreenCircle;
	sf::Sprite tilesetNormal;
	sf::Sprite tilesetIce;
	sf::Sprite tilesetBounce;
	sf::Sprite tilesetOther;
	sf::Sprite exitSign;
	sf::Sprite sawblade;
	sf::Sprite bullet;
	sf::Sprite bulletExplosion;
	sf::Sprite book;
	sf::Sprite xMark;

	std::vector<sf::Sprite*> tilesets;

	Sprites();
	void resetScale();
};
