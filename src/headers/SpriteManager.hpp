#pragma once

class SpriteManager {
	public:
		// textures
		sf::Texture playerTexture;
		sf::Texture playerDeathTexture;
		sf::Texture offscreenCircleTexture;
		sf::Texture tilesetTexture;
		sf::Texture bulletTexture;
		sf::Texture bulletExplosionTexture;
		// sprites
		sf::Sprite playerSprite;
		sf::Sprite playerDeathSprite;
		sf::Sprite offscreenCircleSprite;
		sf::Sprite tilesetSprite;
		sf::Sprite bulletSprite;
		sf::Sprite bulletExplosionSprite;

		SpriteManager();
};
