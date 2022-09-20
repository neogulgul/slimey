#pragma once

struct Inputs
{
	bool up     = false;
	bool down   = false;
	bool left   = false;
	bool right  = false;
	bool space  = false;
	bool enter  = false;
	bool escape = false;
};

struct Sprites
{
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
};

struct Game
{
	sf::RenderWindow *window;
	sf::View *view;

	Inputs inputs;

	Sprites sprites;

	enum File
	{
		A,
		B,
		C,
		None
	};

	File file = None;

	Game(sf::RenderWindow &_window, sf::View &_view);

	void updateFPS();
	void handleInputs();
	void update();
	void draw();
};
