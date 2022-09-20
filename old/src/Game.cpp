#include <SFML/Graphics.hpp>
#include "headers/Game.hpp"

Game::Game(sf::RenderWindow &_window, sf::View &_view)
{
	window = &_window;
	view = &_view;
}

void Game::updateFPS()
{
	FPS = std::ceil(1 / frameClock.restart().asSeconds());
}

void Game::handleInputs()
{
	using key = sf::Keyboard;

	if (key::isKeyPressed(key::W) || key::isKeyPressed(key::Up))
	{
		inputs.up = true;
	}
	else
	{
		inputs.up = false;
	}

	if (key::isKeyPressed(key::S) || key::isKeyPressed(key::Down))
	{
		inputs.down = true;
	}
	else
	{
		inputs.down = false;
	}

	if (key::isKeyPressed(key::A) || key::isKeyPressed(key::Left))
	{
		inputs.left = true;
	}
	else
	{
		inputs.left = false;
	}

	if (key::isKeyPressed(key::D) || key::isKeyPressed(key::Right))
	{
		inputs.right = true;
	}
	else
	{
		inputs.right = false;
	}

	if (key::isKeyPressed(key::Space))
	{
		inputs.space = true;
	}
	else
	{
		inputs.space = false;
	}

	if (key::isKeyPressed(key::Enter))
	{
		inputs.enter = true;
	}
	else
	{
		inputs.enter = false;
	}

	if (key::isKeyPressed(key::Escape))
	{
		inputs.escape = true;
	}
	else
	{
		inputs.escape = false;
	}
}

void Game::update()
{}

void Game::draw()
{}
