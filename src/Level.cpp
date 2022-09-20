#include <SFML/Graphics.hpp>

#include "headers/Level.hpp"

Level::Level() {}

Level::Level(sf::RenderWindow *_window, Sprites *_sprites)
{
	window  = _window;
	sprites = _sprites;

	pauseShape.setSize(sf::Vector2f(viewWidth, viewHeight));
	pauseShape.setFillColor(pauseColor);

	sawbladeAnimation = Animation(sawbladeFrameCount, sawbladeFrameDuration);
}

void Level::reset()
{
	spawn = { 0, 0 };
	exit  = { 0, 0 };
	paused       = false;
	pressedPause = false;
}

void Level::loadMap(mapVector _map)
{
	map = _map;

	mapSize.x = map.size();
	mapSize.y = map.at(0).size();

	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			sf::Vector3i tile = map.at(x).at(y);
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
		}
	}

	player = Player(&sprites->slimeyFrames, &map, mapSize, spawn, exit);
}

void Level::drawMap()
{
	sf::Sprite *sprite;
	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			sf::Vector3i tile = map.at(x).at(y);
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
	sprite = &sprites->exitSign;
	sprite->setPosition(exit.x * tilesize, (exit.y - 1) * tilesize);
	window->draw(*sprite);
}

void Level::update()
{
	if (pressing(pause))
	{
		if (!pressedPause)
		{
			pressedPause = true;
			toggle(paused);
		}
	}
	else
	{
		pressedPause = false;
	}

	if (paused) { return; }

	player.update();
}

void Level::draw()
{
	if (!paused) { sawbladeAnimation.update(); }
	drawMap();
	player.draw(window);
	if (paused) { window->draw(pauseShape); }
}
