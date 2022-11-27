#include <cmath>
#include <fstream>

#include "headers/Level.hpp"

Level::Level() {}

Level::Level(sf::RenderWindow* _window, sf::View* _view, sf::FloatRect* _viewport,
             Audio* _audio, Sprites* _sprites, Text* _text, Transition* _transition, bool* _paused, bool* _debug)
{
	window     = _window;
	view       = _view;
	viewport   = _viewport;
	audio      = _audio;
	sprites    = _sprites;
	text       = _text;
	transition = _transition;
	paused     = _paused;
	debug      = _debug;

	collisionRect.setSize({tilesize, tilesize});
	collisionRect.setFillColor(sf::Color::Transparent);
	collisionRect.setOutlineColor(collisionColor);
	collisionRect.setOutlineThickness(-2);

	sawbladeAnimation = Animation(sawbladeFrameCount, sawbladeFrameDuration);
}

void Level::reset()
{
	turretTimer = default_turretTimer;
	for (Bullet &bullet : bullets)
	{
		bullet.explode();
	}
}

void Level::loadLevel(LevelVector _level, State _destination)
{
	destination = _destination;

	spawn = {0, 0};
	exit  = {0, 0};

	loaded  = false;
	cleared = false;

	turretTimer = default_turretTimer;

	turrets.clear();
	bullets.clear();

	level = _level;

	levelSize.x = level.size();
	levelSize.y = level.at(0).size();

	for (unsigned int x = 0; x < levelSize.x; x++)
	{
		for (unsigned int y = 0; y < levelSize.y; y++)
		{
			sf::Vector3i tile = level.at(x).at(y);
			if (tile == spawnTile)
			{
				spawn.x = x;
				spawn.y = y - 1;
			}
			else if (tile == exitTile)
			{
				exit.x = x;
				exit.y = y;
			}
			else if (tile == turretUp)
			{
				turrets.push_back(Turret(Up, {x, y}));
			}
			else if (tile == turretDown)
			{
				turrets.push_back(Turret(Down, {x, y}));
			}
			else if (tile == turretLeft)
			{
				turrets.push_back(Turret(Left, {x, y}));
			}
			else if (tile == turretRight)
			{
				turrets.push_back(Turret(Right, {x, y}));
			}
		}
	}

	player = Player(&sprites->slimeyFrames, &sprites->slimeyDeath, &sprites->offscreenCircle, &cleared, &level, levelSize, spawn);
}

void Level::loadLevel(LevelVector _level, int _index)
{
	loadLevel(_level, LevelClear);
	index = _index;
}

void Level::drawLevel()
{
	sf::Sprite* sprite;
	for (unsigned int x = 0; x < levelSize.x; x++)
	{
		for (unsigned int y = 0; y < levelSize.y; y++)
		{
			// only allowing tiles seen in view to be drawn
			if (!viewport->intersects(sf::FloatRect(x * tilesize, y * tilesize, tilesize, tilesize))) { continue; }

			sf::Vector3i tile = level.at(x).at(y);
			int tileset = tile.x;
			if (tileset != 0)
			{
				sf::IntRect crop;
				crop.width  = tilesize;
				crop.height = tilesize;

				if (tile == sawbladeTile)
				{
					sprite = &sprites->sawblade;
					crop.left = sawbladeAnimation.frame * tilesize;
					crop.top  = 0;
				}
				else
				{
					sprite = sprites->tilesets.at(tileset - 1);
					crop.left = tile.y * tilesize;
					crop.top  = tile.z * tilesize;
				}
				sprite->setPosition(x * tilesize, y * tilesize);
				sprite->setTextureRect(crop);
				window->draw(*sprite);
			}
		}
	}

	// drawing exit sign
	if (exit.y != 0)
	{
		sprite = &sprites->exitSign;
		sprite->setPosition(exit.x * tilesize, (exit.y - 1) * tilesize);
		window->draw(*sprite);
	}
}

void Level::updateTurrets()
{
	turretTimer++;
	if (turretTimer >= turretFrames)
	{
		turretTimer = 0;
		for (Turret &turret : turrets)
		{
			turret.shoot(&bullets, &sprites->bullet, &level, levelSize, &sprites->bulletExplosion, &player);
			audio->shoot.play();
		}
	}
}

void Level::updateBullets()
{
	for (Bullet &bullet : bullets)
	{
		bool bulletExplodingLastFrame = bullet.exploding;
		bullet.update();
		if (bullet.exploding && !bulletExplodingLastFrame)
		{ audio->explosion.play(); }
	}
}

void Level::drawBullets()
{
	for (Bullet &bullet : bullets)
	{
		bullet.draw(window, *viewport);
	}
}

void Level::destroyBullets()
{
	unsigned int  numberOfBullets = bullets.size();
	unsigned int destroyedBullets = 0;

	for (unsigned int i = 0; i < numberOfBullets; i++)
	{
		unsigned int realIndex = i - destroyedBullets;
		if (bullets.at(realIndex).destroyed)
		{
			bullets.erase(bullets.begin() + realIndex);
			destroyedBullets++;
		}
	}
}

void Level::drawPlayerCollisions()
{
	for (Collision collision : player.collisions)
	{
		collisionRect.setPosition(collision.coord.x * tilesize, collision.coord.y * tilesize);
		window->draw(collisionRect);
	}
}

void Level::drawDebugMode()
{
	drawPlayerCollisions();

	std::stringstream stream;
	stream << "alive:   " << boolString(player.alive);
	text->draw(stream.str(), Start, End, relativeViewPosition(view, {0, viewHeight - 50}));

	stream.str("");
	stream << "onGround:" << boolString(player.onGround);
	text->draw(stream.str(), Start, End, relativeViewPosition(view, {0, viewHeight - 40}));

	stream.str("");
	stream << "onIce:   " << boolString(player.onIce);
	text->draw(stream.str(), Start, End, relativeViewPosition(view, {0, viewHeight - 30}));

	stream.str("");
	stream << "coord:   (x:" << std::floor(player.position.x / tilesize) << ", y:" << std::floor(player.position.y / tilesize) << ")";
	text->draw(stream.str(), Start, End, relativeViewPosition(view, {0, viewHeight - 20}));

	stream.str("");
	stream << "position:(x:" << player.position.x << ", y:" << player.position.y << ")";
	text->draw(stream.str(), Start, End, relativeViewPosition(view, {0, viewHeight - 10}));

	stream.str("");
	stream << "velocity:(x:" << player.velocity.x << ", y:" << player.velocity.y << ")";
	text->draw(stream.str(), Start, End, relativeViewPosition(view, {0, viewHeight     }));
}

void Level::updateView(bool instant = false)
{
	sf::Vector2f destination;
	if (instant)
	{
		destination = player.getCenter();
	}
	else
	{
		sf::Vector2f start       = view->getCenter();
		sf::Vector2f target      = player.getCenter();
		sf::Vector2f delta       = target - start;
		sf::Vector2f distance    = {delta.x * 0.1f, delta.y * 0.1f};
		destination = start + distance;
	}
	// limiting view horizontally
	if (destination.x < viewWidth * 0.5)
	{
		destination.x = viewWidth * 0.5;
	}
	else
	if (destination.x > levelSize.x * tilesize - viewWidth * 0.5)
	{
		destination.x = levelSize.x * tilesize - viewWidth * 0.5;
	}
	// limiting view vertically
	if (destination.y < viewHeight * 0.5)
	{
		destination.y = viewHeight * 0.5;
	}
	else
	if (destination.y > levelSize.y * tilesize - viewHeight * 0.5)
	{
		destination.y = levelSize.y * tilesize - viewHeight * 0.5;
	}
	view->setCenter(destination);
}

void Level::update()
{
	if (!window->hasFocus()) { return; }
	if (!loaded)
	{
		loaded = true;
		updateView(true);
	}

	if (!cleared && transition->transitioning) { return; }

	if (*paused) { return; }

	bool playerAliveLastFrame  = player.alive;
	bool playerJumpedLastFrame = player.jumped;

	if (!cleared && player.alive)
	{
		updateTurrets();
	}
	updateBullets();
	destroyBullets();

	player.update();

	// playing some audio
	if (player.jumped && !playerJumpedLastFrame)
	{ audio->jump.play(); }
	if (player.hitVerticalBounce || player.hitHorizontalBounce)
	{ audio->bounce.play(); }
	if (!player.alive && playerAliveLastFrame)
	{ audio->hurt.play(); }
	if (player.alive && !playerAliveLastFrame)
	{ audio->spawn.play(); }

	updateView();

	if (cleared && !transition->transitioning)
	{
		if (destination == LevelClear)
		{
			bool needToCreateLevelsClearedFile = false;
			if (fs::is_directory("savedata"))
			{
				if (!(fs::exists("savedata/levels_cleared.txt") && !fs::is_directory("savedata/levels_cleared.txt")))
				{
					needToCreateLevelsClearedFile = true;
				}
			}
			else
			{
				fs::create_directory("savedata");
				needToCreateLevelsClearedFile = true;
			}

			if (needToCreateLevelsClearedFile)
			{
				std::ofstream levelsCleared("savedata/levels_cleared.txt");
				levelsCleared.close();
			}

			int clearedLevels = 0;
			std::ifstream readStream;
			readStream.open("savedata/levels_cleared.txt");
			std::string line;
			std::getline(readStream, line);
			readStream.close();
			clearedLevels = atoi(line.c_str());

			if (index == clearedLevels)
			{
				std::ofstream writeStream;
				writeStream.open("savedata/levels_cleared.txt");
				writeStream << index + 1;
				writeStream.close();
			}
		}
		transition->to(destination);
	}

	if (!cleared && !player.alive) { reset(); }

	sawbladeAnimation.update();
}

void Level::draw()
{
	drawLevel();
	drawBullets();
	player.draw(window, *viewport);
	if (*debug)
	{
		drawDebugMode();
	}
}
