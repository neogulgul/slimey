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

void Game::updateLevelSelect()
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

		case LevelSelect:
			updateLevelSelect();
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
	text.draw("Slimey", Center, Center, sf::Vector2f(viewWidth / 2, viewHeight / 4), sf::Vector2f(2, 2));
}

void Game::drawOptions()
{
	text.draw("Options", Center, Center, sf::Vector2f(viewWidth / 2, viewHeight / 4), sf::Vector2f(2, 2));
}

void Game::drawLevelSelect()
{
	text.draw("Level Select", Center, Center, sf::Vector2f(viewWidth / 2, viewHeight / 4), sf::Vector2f(2, 2));
	text.draw("Default", Center, Center, sf::Vector2f(viewWidth / 2, verticalPositionOfDefaultSection));
	text.draw("Custom", Center, Center, sf::Vector2f(viewWidth / 2, verticalPositionOfCustomSection));
}

void Game::drawLevelPlay()
{
	level.draw();
	if (level.paused)
	{
		text.draw("Paused", Center, Center, sf::Vector2f(viewWidth / 2, viewHeight / 4), sf::Vector2f(2, 2));
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

		case LevelSelect:
			drawLevelSelect();
			break;

		case LevelPlay:
			drawLevelPlay();
			break;

		case LevelClear:
			drawLevelClear();
			break;
	}
}
