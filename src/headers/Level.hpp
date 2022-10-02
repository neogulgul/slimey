#pragma once

#include "Animation.hpp"
#include "Audio.hpp"
#include "Bullet.hpp"
#include "DefaultMaps.hpp"
#include "Global.hpp"
#include "Player.hpp"
#include "Sprites.hpp"
#include "Transition.hpp"
#include "Turret.hpp"

struct Level
{
	sf::RenderWindow *window;
	sf::View *view;
	sf::FloatRect *viewport;

	Audio *audio;
	Sprites *sprites;
	Transition *transition;

	mapVector map;

	sf::Vector2u mapSize;

	sf::Vector2i spawn;
	sf::Vector2u exit;

	Player player;

	std::vector<Turret> turrets;
	std::vector<Bullet> bullets;

	unsigned int turretTimer  = 0;
	unsigned int turretFrames = 120;

	sf::RectangleShape pauseShape;
	sf::Keyboard::Key  pause = sf::Keyboard::Escape;

	bool loaded  = false;
	bool cleared = false;

	bool paused       = false;
	bool pressedPause = false;

	Animation sawbladeAnimation;
	int sawbladeFrameCount    = 8;
	int sawbladeFrameDuration = 1;

	Level();
	Level(sf::RenderWindow *_window, sf::View *_view, sf::FloatRect *_viewport, Audio *_audio, Sprites *_sprites, Transition *_transition);

	void reset();
	void loadMap(mapVector _map);
	void drawMap();

	void updateTurrets();
	void updateBullets();
	void drawBullets();
	void destroyBullets();

	void updateView(bool instant);

	void update();
	void draw();
};
