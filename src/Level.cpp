#include <SFML/Graphics.hpp>

#include "headers/Level.hpp"

Level::Level() {}

Level::Level(sf::RenderWindow *_window, sf::View *_view, sf::FloatRect *_viewport, Sprites *_sprites, Transition *_transition)
{
	window     = _window;
	view       = _view;
	viewport   = _viewport;
	sprites    = _sprites;
	transition = _transition;

	pauseShape.setSize(sf::Vector2f(viewWidth, viewHeight));
	pauseShape.setFillColor(pauseColor);

	sawbladeAnimation = Animation(sawbladeFrameCount, sawbladeFrameDuration);
}

void Level::updateView(bool instant = false)
{
	if (instant)
	{
		view->setCenter(player.getCenter());
	}
	else
	{
		sf::Vector2f start       = view->getCenter();
		sf::Vector2f target      = player.getCenter();
		sf::Vector2f delta       = target - start;
		sf::Vector2f distance    = {delta.x * 0.1f, delta.y * 0.1f};
		sf::Vector2f destination = start + distance;

		// limiting view horizontally
		if (destination.x < viewWidth * 0.5)
		{
			destination.x = viewWidth * 0.5;
		}
		else
		if (destination.x > mapSize.x * tilesize - viewWidth * 0.5)
		{
			destination.x = mapSize.x * tilesize - viewWidth * 0.5;
		}
		// limiting view vertically
		if (destination.y < viewHeight * 0.5)
		{
			destination.y = viewHeight * 0.5;
		}
		else
		if (destination.y > mapSize.y * tilesize - viewHeight * 0.5)
		{
			destination.y = mapSize.y * tilesize - viewHeight * 0.5;
		}

		view->setCenter(destination);
	}
}

void Level::reset()
{
	spawn = {0, 0};
	exit  = {0, 0};

	loaded  = false;
	cleared = false;

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

	player = Player(&sprites->slimeyFrames, &sprites->offscreenCircle, &cleared, &map, mapSize, spawn, exit);
}

void Level::drawMap()
{
	sf::Sprite *sprite;
	for (unsigned int x = 0; x < mapSize.x; x++)
	{
		for (unsigned int y = 0; y < mapSize.y; y++)
		{
			// only allowing tiles seen in view to be drawn
			if (!viewport->intersects(sf::FloatRect(x * tilesize, y * tilesize, tilesize, tilesize))) { continue; }

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
	if (exit.y != 0)
	{
		sprite = &sprites->exitSign;
		sprite->setPosition(exit.x * tilesize, (exit.y - 1) * tilesize);
		window->draw(*sprite);
	}
}

#include <iostream>

void Level::update()
{
	if (!window->hasFocus()) { return; }

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
	if (!loaded)
	{
		loaded = true;
		updateView(true);
	}
	updateView();
	if (cleared && !transition->transitioning)
	{
		transition->to(StoryLevels);
	}
}

void Level::draw()
{
	if (!paused) { sawbladeAnimation.update(); }
	drawMap();
	player.draw(window, *viewport, paused);
	if (paused)
	{
		pauseShape.setPosition(relativeViewPosition(*view, {0, 0}));
		window->draw(pauseShape);
	}
}
