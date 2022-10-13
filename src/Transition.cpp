#include <cmath>

#include "headers/Transition.hpp"

Transition::Transition() {}

Transition::Transition(sf::RenderWindow* _window, sf::View* _view, State &_gameState)
{
	window    = _window;
	view      = _view;
	gameState = &_gameState;

	shape.setSize({viewWidth, viewHeight});
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
	shape.setPosition(relativeViewPosition(view, {0, 0}));
	shape.setFillColor(sf::Color(0, 0, 0, alpha));
	window->draw(shape);
}
