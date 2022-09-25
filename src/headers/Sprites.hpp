#pragma once

struct Sprites
{
	// textures
	sf::Texture slimeyTexture;
	sf::Texture slimeyColorlessTexture;
	sf::Texture slimeyFramesTexture;
	sf::Texture offscreenCircleTexture;
	sf::Texture tilesetNormalTexture;
	sf::Texture tilesetIceTexture;
	sf::Texture tilesetBounceTexture;
	sf::Texture tilesetOtherTexture;
	sf::Texture exitSignTexture;
	sf::Texture sawbladeTexture;

	// sprites
	sf::Sprite slimey;
	sf::Sprite slimeyColorless;
	sf::Sprite slimeyFrames;
	sf::Sprite offscreenCircle;
	sf::Sprite tilesetNormal;
	sf::Sprite tilesetIce;
	sf::Sprite tilesetBounce;
	sf::Sprite tilesetOther;
	sf::Sprite exitSign;
	sf::Sprite sawblade;

	std::vector<sf::Sprite *> tilesets;

	Sprites();
};
