#include <SFML/Graphics.hpp>

#include "headers/Game.hpp"

void Game::updateSplashScreen()
{
}

void Game::updateExitScreen()
{
	window->close();
}

void Game::updateMainMenu()
{
}

void Game::updateOptions()
{
}

void Game::updateStoryLevels()
{
}

void Game::updateLevelPlay()
{
	level.update();
}

void Game::updateLevelClear()
{
}

void Game::updateState()
{
	switch (state)
	{
		case SplashScreen:
			updateSplashScreen();
			break;

		case ExitScreen:
			updateExitScreen();
			break;

		case MainMenu:
			updateMainMenu();
			break;

		case Options:
			updateOptions();
			break;

		case StoryLevels:
			updateStoryLevels();
			break;

		case LevelPlay:
			updateLevelPlay();
			break;

		case LevelClear:
			updateLevelClear();
			break;
	}
}



void Game::drawSplashScreen()
{
}

void Game::drawExitScreen()
{
}

struct Poop
{
	sf::Color color;
	sf::Vector2f position;
	int rotation;
	int speed;

	bool flushed = false;

	Poop(sf::Color _color, sf::Vector2f _position, int _rotation, int _speed)
	{
		color    = _color;
		position = _position;
		rotation = _rotation;
		speed    = _speed;

		position.x -= viewWidth;
	}

	void update()
	{
		position.x += speed;
		rotation   += speed;
		if (rotation >= 360)
		{
			rotation = 0;
		}
	}
};

void Game::drawMainMenu()
{
	static std::vector<sf::Color> randomColors = {
		sf::Color(255, 139, 139, 63),
		sf::Color(245, 255, 162, 63),
		sf::Color(170, 255, 162, 63),
		sf::Color(159, 209, 255, 63),
		sf::Color(255, 174, 247, 63)
	};

	static std::vector<Poop> poopies;
	static std::uniform_int_distribution<> position_dist(0, viewWidth - 1);
	static std::uniform_int_distribution<> rotation_dist(0, 359);
	static std::uniform_int_distribution<> speed_dist(1, 5);
	static std::uniform_int_distribution<> color_dist(0, randomColors.size() - 1);

	if (changedState)
	{
		poopies.clear();
	}

	while (poopies.size() < 100)
	{
		poopies.push_back(Poop(randomColors.at(color_dist(rng)), sf::Vector2f(position_dist(rng), position_dist(rng)), rotation_dist(rng), speed_dist(rng)));
	}

	sf::Color randomColor(color_dist(rng), color_dist(rng), color_dist(rng));

	for (unsigned int i = 0; i < poopies.size(); i++)
	{
		Poop &poop = poopies.at(i);
		poop.update();
		if (poop.position.x > viewWidth + 32)
		{
			poop.flushed = true;
		}
		sprites.slimeyColorless.setOrigin(7, 6);
		sprites.slimeyColorless.setRotation(poop.rotation);
		sprites.slimeyColorless.setPosition(poop.position);
		sprites.slimeyColorless.setColor(poop.color);
		window->draw(sprites.slimeyColorless);
	}

	for (unsigned int i = 0; i < poopies.size(); i++)
	{
		if (poopies.at(i).flushed)
		{
			poopies.erase(poopies.begin() + i);
		}
	}

	text.draw("Slimey", Center, Center, sf::Vector2f(viewWidth * 0.5, viewHeight * 0.25), sf::Vector2f(2, 2));
}

void Game::drawOptions()
{
	text.draw("Options", Center, Center, sf::Vector2f(viewWidth * 0.5, viewHeight / 4), sf::Vector2f(2, 2));
}

void Game::drawStoryLevels()
{
	text.draw("Story Levels", Center, Center, sf::Vector2f(viewWidth * 0.5, viewHeight / 4), sf::Vector2f(2, 2));
}

void Game::drawLevelPlay()
{
	level.draw();
	if (level.paused)
	{
		text.draw("Paused", Center, Center, sf::Vector2f(viewWidth * 0.5, viewHeight / 4), sf::Vector2f(2, 2));
	}
}

void Game::drawLevelClear()
{
}

void Game::drawState()
{
	switch (state)
	{
		case SplashScreen:
			drawSplashScreen();
			break;

		case ExitScreen:
			drawExitScreen();
			break;

		case MainMenu:
			drawMainMenu();
			break;

		case Options:
			drawOptions();
			break;

		case StoryLevels:
			drawStoryLevels();
			break;

		case LevelPlay:
			drawLevelPlay();
			break;

		case LevelClear:
			drawLevelClear();
			break;
	}
}
