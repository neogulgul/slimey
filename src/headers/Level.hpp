#pragma once

#include "Animation.hpp"
#include "DefaultMaps.hpp"
#include "Global.hpp"
#include "Player.hpp"
#include "Sprites.hpp"

struct Level
{
	sf::RenderWindow *window;
	sf::View *view;
	sf::FloatRect viewPort;

	Sprites *sprites;

	mapVector map;

	sf::Vector2u mapSize;

	sf::Vector2i spawn;
	sf::Vector2u exit;

	Player player;

	sf::RectangleShape pauseShape;
	sf::Keyboard::Key  pause = sf::Keyboard::Escape;

	bool paused       = false;
	bool pressedPause = false;

	int sawbladeFrameCount    = 8;
	int sawbladeFrameDuration = 2;
	Animation sawbladeAnimation;

	Level();
	Level(sf::RenderWindow *_window, sf::View *_view, Sprites *_sprites);

	void updateViewPort();
	void reset();
	void loadMap(mapVector _map);
	void drawMap();
	void drawPause();

	void update();
	void draw();
};
