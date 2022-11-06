#pragma once

#include "Animation.hpp"
#include "Audio.hpp"
#include "Bullet.hpp"
#include "StoryLevels.hpp"
#include "Global.hpp"
#include "Player.hpp"
#include "Sprites.hpp"
#include "Text.hpp"
#include "Transition.hpp"
#include "Turret.hpp"

#define default_turretTimer 120

struct Level
{
	sf::RenderWindow* window;
	sf::View* view;
	sf::FloatRect* viewport;

	Audio*      audio;
	Sprites*    sprites;
	Text*       text;
	Transition* transition;

	bool* paused;
	bool* debug;

	sf::RectangleShape collisionRect;

	int index;

	State destination;

	LevelVector level;

	sf::Vector2u levelSize;

	sf::Vector2i spawn;
	sf::Vector2i exit;

	Player player;

	std::vector<Turret> turrets;
	std::vector<Bullet> bullets;

	unsigned int turretTimer  = default_turretTimer;
	unsigned int turretFrames = 120;

	bool loaded  = false;
	bool cleared = false;

	Animation sawbladeAnimation;

	Level();
	Level(sf::RenderWindow* _window, sf::View* _view, sf::FloatRect* _viewport,
	      Audio* _audio, Sprites* _sprites, Text* _text, Transition* _transition, bool* _paused, bool* _debug);

	void reset();
	void loadLevel(LevelVector _level, State _destination);
	void loadLevel(LevelVector _level, int _index);
	void drawLevel();

	void updateTurrets();
	void updateBullets();
	void drawBullets();
	void destroyBullets();

	void drawPlayerCollisions();
	void drawDebugMode();

	void updateView(bool instant);

	void update();
	void draw();
};
