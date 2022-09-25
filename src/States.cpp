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

struct RainbowSlimey
{
	sf::Color color;
	sf::Vector2f position;
	float scale;
	int rotation;
	int speed;

	bool passed = false;

	RainbowSlimey(sf::Color _color, sf::Vector2f _position, float _scale, int _rotation, int _speed)
	{
		color    = _color;
		position = _position;
		scale    = _scale;
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

	static std::vector<RainbowSlimey> rainbowSlimeys;
	static std::uniform_int_distribution<> position_dist(0, viewWidth - 1);
	static std::uniform_int_distribution<> rotation_dist(0, 359);
	static std::uniform_int_distribution<> speed_dist(1, 5);
	static std::uniform_int_distribution<> color_dist(0, randomColors.size() - 1);
	static std::uniform_int_distribution<> scale_dist(0.5 * 10, 2 * 10);

	if (changedState)
	{
		rainbowSlimeys.clear();
	}

	while (rainbowSlimeys.size() < 100)
	{
		rainbowSlimeys.push_back(RainbowSlimey(randomColors.at(color_dist(rng)), sf::Vector2f(position_dist(rng), position_dist(rng)), scale_dist(rng) / 10.f, rotation_dist(rng), speed_dist(rng)));
	}

	sf::Color randomColor(color_dist(rng), color_dist(rng), color_dist(rng));

	for (unsigned int i = 0; i < rainbowSlimeys.size(); i++)
	{
		RainbowSlimey &rainbowSlimey = rainbowSlimeys.at(i);
		rainbowSlimey.update();
		if (rainbowSlimey.position.x > viewWidth + sprites.slimeyColorlessTexture.getSize().x)
		{
			rainbowSlimey.passed = true;
		}
		sprites.slimeyColorless.setOrigin(sprites.slimeyColorlessTexture.getSize().x / 2, sprites.slimeyColorlessTexture.getSize().y / 2);
		sprites.slimeyColorless.setRotation(rainbowSlimey.rotation);
		sprites.slimeyColorless.setPosition(rainbowSlimey.position);
		sprites.slimeyColorless.setColor(rainbowSlimey.color);
		sprites.slimeyColorless.setScale(rainbowSlimey.scale, rainbowSlimey.scale);
		window->draw(sprites.slimeyColorless);
	}

	for (unsigned int i = 0; i < rainbowSlimeys.size(); i++)
	{
		if (rainbowSlimeys.at(i).passed)
		{
			rainbowSlimeys.erase(rainbowSlimeys.begin() + i);
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
