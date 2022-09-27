#pragma once

#include "Animation.hpp"
#include "DefaultMaps.hpp"
#include "Global.hpp"
#include "Player.hpp"
#include "Sprites.hpp"
#include "Transition.hpp"

struct Level
{
	sf::RenderWindow *window;
	sf::View *view;
	sf::FloatRect *viewport;

	Sprites *sprites;
	Transition *transition;

	mapVector map;

	sf::Vector2u mapSize;

	sf::Vector2i spawn;
	sf::Vector2u exit;

	Player player;

	sf::RectangleShape pauseShape;
	sf::Keyboard::Key  pause = sf::Keyboard::Escape;

	bool loaded  = false;
	bool cleared = false;

	bool paused       = false;
	bool pressedPause = false;

	int sawbladeFrameCount    = 8;
	int sawbladeFrameDuration = 2;
	Animation sawbladeAnimation;

	Level();
	Level(sf::RenderWindow *_window, sf::View *_view, sf::FloatRect *_viewport, Sprites *_sprites, Transition *_transition);

	void updateView(bool instant);
	void reset();
	void loadMap(mapVector _map);
	void drawMap();
	void drawPause();

	void update();
	void draw();
};
