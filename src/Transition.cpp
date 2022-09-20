#include <SFML/Graphics.hpp>
#include <cmath>

#include "headers/Transition.hpp"

Transition::Transition() {}

Transition::Transition(sf::RenderWindow *_window, State &_gameState)
{
	window    = _window;
	gameState = &_gameState;

	shape.setSize(sf::Vector2f(viewWidth, viewHeight));
}

void Transition::to(State state)
{
	transitioning = true;
	destination   = state;
}

void Transition::changeState()
{
	*gameState    = destination;
	atDestination = true;
	sleeping      = true;
}

void Transition::reset()
{
	transitioning = false;
	atDestination = false;
	alpha      = 0;
	opacity    = 0;
	sleepTimer = 0;
}

void Transition::update()
{
	if (sleeping)
	{
		sleepTimer++;
		if (sleepTimer >= sleepFrames)
		{
			sleeping = false;
		}
		return;
	}
	if (atDestination)
	{
		opacity -= opacityIncrement;
		alpha = std::floor(opacity * 255);
		if (alpha <= 0)
		{
			alpha = 0;
			reset();
		}
	}
	else
	{
		opacity += opacityIncrement;
		alpha = std::floor(opacity * 255);
		if (alpha >= 255)
		{
			alpha = 255;
			changeState();
		}
	}
}

void Transition::draw()
{
	shape.setFillColor(sf::Color(0, 0, 0, alpha));
	window->draw(shape);
}
