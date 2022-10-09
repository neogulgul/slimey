#include "headers/Game.hpp"

#define amountOfRainbowSlimeys 100

RainbowSlimey::RainbowSlimey(sf::Color _color, sf::Vector2f _position, float _scale, int _rotation, int _speed)
{
	color    = _color;
	position = _position;
	scale    = _scale;
	rotation = _rotation;
	speed    = _speed;
}

void RainbowSlimey::update()
{
	position.x += speed;
	rotation   += speed;
	if (rotation >= 360)
	{
		rotation = 0;
	}
}



void Game::updateSplashScreen()
{
}

void Game::updateExitScreen()
{
	window->close();
}

void Game::updateMainMenu()
{
	static std::vector<sf::Color> randomColors = {
		sf::Color(255, 139, 139, 63),
		sf::Color(245, 255, 162, 63),
		sf::Color(170, 255, 162, 63),
		sf::Color(159, 209, 255, 63),
		sf::Color(255, 174, 247, 63)
	};

	static std::uniform_int_distribution<> position_dist(0, viewWidth - 1);
	static std::uniform_int_distribution<> rotation_dist(0, 359);
	static std::uniform_int_distribution<> speed_dist(1, 5);
	static std::uniform_int_distribution<> color_dist(0, randomColors.size() - 1);
	static std::uniform_int_distribution<> scale_dist(0.5 * 10, 2 * 10);

	if (changedState)
	{
		rainbowSlimeys.clear();
	}

	while (rainbowSlimeys.size() < amountOfRainbowSlimeys)
	{
		sf::Vector2f position(position_dist(rng), position_dist(rng));
		if (!changedState)
		{
			position.x -= viewWidth + sprites.slimeyColorlessTexture.getSize().x;
		}
		rainbowSlimeys.push_back(RainbowSlimey(randomColors.at(color_dist(rng)), position, scale_dist(rng) / 10.f, rotation_dist(rng), speed_dist(rng)));
	}

	for (unsigned int i = 0; i < rainbowSlimeys.size(); i++)
	{
		RainbowSlimey &rainbowSlimey = rainbowSlimeys.at(i);
		rainbowSlimey.update();
		if (rainbowSlimey.position.x > viewWidth + sprites.slimeyColorlessTexture.getSize().x)
		{
			rainbowSlimey.passed = true;
		}
	}

	unsigned int numberOfSlimeys = rainbowSlimeys.size();
	unsigned int   passedSlimeys = 0;

	for (unsigned int i = 0; i < numberOfSlimeys; i++)
	{
		unsigned int realIndex = i - passedSlimeys;
		if (rainbowSlimeys.at(realIndex).passed)
		{
			rainbowSlimeys.erase(rainbowSlimeys.begin() + realIndex);
			passedSlimeys++;
		}
	}
}

void Game::updateOptionsScreen()
{
	for (OptionButton &button : options.optionButtons)
	{
		button.active = false;
		if (button.bounds.contains(mousePosition))
		{
			button.active = true;
			handyCursor   = true;
			if (leftClick)
			{
				audio.click.play();
				toggle(*button.option);
				button.update();
			}
		}
	}
	options.resetButton.active = false;
	if (options.resetButton.bounds.contains(mousePosition))
	{
		options.resetButton.active = true;
		handyCursor   = true;
		if (leftClick)
		{
			audio.click.play();
			options.reset();
		}
	}
}

void Game::updateLevelEditor()
{
	editor.update();
}

void Game::updateStoryLevels()
{
}

void Game::updateCustomLevels()
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

		case OptionsScreen:
			updateOptionsScreen();
			break;

		case LevelEditor:
			updateLevelEditor();
			break;

		case StoryLevels:
			updateStoryLevels();
			break;

		case CustomLevels:
			updateCustomLevels();
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

void Game::drawMainMenu()
{
	for (RainbowSlimey rainbowSlimey : rainbowSlimeys)
	{
		sprites.slimeyColorless.setOrigin(sprites.slimeyColorlessTexture.getSize().x / 2, sprites.slimeyColorlessTexture.getSize().y / 2);
		sprites.slimeyColorless.setRotation(rainbowSlimey.rotation);
		sprites.slimeyColorless.setPosition(rainbowSlimey.position);
		sprites.slimeyColorless.setColor(rainbowSlimey.color);
		sprites.slimeyColorless.setScale(rainbowSlimey.scale, rainbowSlimey.scale);
		window->draw(sprites.slimeyColorless);
	}

	text.draw("Slimey", Center, Center, {viewWidth * 0.5, viewHeight * 0.25}, {2, 2});
}

void Game::drawOptionsScreen()
{
	text.draw("Options", Center, Center, {viewWidth * 0.5, viewHeight * 0.25}, {2, 2});
	text.draw("Music"        , Start, Center, {viewWidth * 0.25, viewHeight - 150});
	text.draw("Sound Effects", Start, Center, {viewWidth * 0.25, viewHeight - 120});
	text.draw("Show FPS"     , Start, Center, {viewWidth * 0.25, viewHeight -  90});
	text.draw("Debug Mode"   , Start, Center, {viewWidth * 0.25, viewHeight -  60});

	for (OptionButton &button : options.optionButtons)
	{
		button.draw(window, view, &text);
	}
	options.resetButton.draw(window, view, &text);
}

void Game::drawLevelEditor()
{
	editor.draw();
}

void Game::drawStoryLevels()
{
	text.draw("Story Levels", Center, Center, {viewWidth * 0.5, viewHeight * 0.25}, {2, 2});
}

void Game::drawCustomLevels()
{
	text.draw("Custom Levels", Center, Center, {viewWidth * 0.5, viewHeight * 0.25}, {2, 2});
	if (customLevelsCount == 0)
	{
		text.draw("No custom levels...", Center, Center, {viewWidth * 0.5, viewHeight * 0.5});
	}
	else
	{
		text.draw("Number of custom levels:" + std::to_string(customLevelsCount), Center, Center, {viewWidth * 0.5, viewHeight * 0.325});
	}
}

void Game::drawLevelPlay()
{
	level.draw();
}

void Game::drawLevelClear()
{
	text.draw("Level Cleared", Center, Center, {viewWidth * 0.5, viewHeight * 0.25}, {2, 2});
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

		case OptionsScreen:
			drawOptionsScreen();
			break;

		case LevelEditor:
			drawLevelEditor();
			break;

		case StoryLevels:
			drawStoryLevels();
			break;

		case CustomLevels:
			drawCustomLevels();
			break;

		case LevelPlay:
			drawLevelPlay();
			break;

		case LevelClear:
			drawLevelClear();
			break;
	}
}
