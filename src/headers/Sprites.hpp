#pragma once

struct Sprites
{
	// textures
	sf::Texture slimeyTexture;
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

	// sprites
	sf::Sprite slimey;
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

	std::vector<sf::Sprite *> tilesets;

	Sprites();
	void resetScale();
};
