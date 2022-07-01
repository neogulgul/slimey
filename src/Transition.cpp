#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/State.hpp"
#include "headers/Transition.hpp"

Transition::Transition() {
	this->square = sf::RectangleShape(sf::Vector2f(tilesize, tilesize));
	this->square.setFillColor(sf::Color::Black);
}

void Transition::to(State destination) {
	this->ongoing = true;
	this->destination = destination;
}

void Transition::reset() {
	this->transitionTimer      = 0;
	this->transitionDelayTimer = 0;
	switch (this->type) {
		case inward:
			this->type = outward;
			break;
		case outward:
			this->type = inward;
			break;
	}
	if (this->inwardComplete && this->outwardComplete) {
		this->inwardComplete  = false;
		this->outwardComplete = false;
		this->ongoing         = false;
	}
}

void Transition::draw(sf::RenderWindow &window, sf::View view) {
	if (this->transitionTimer < this->transitionFrames) {
		this->transitionTimer += this->transitionSpeed;
	} else {
		this->transitionDelayTimer++;
		if (this->transitionDelayTimer == this->transitionDelayFrames) {
			switch (this->type) {
				case inward:
					this->inwardComplete = true;
					break;
				case outward:
					this->outwardComplete = true;
					break;
			}
			this->reset();
		}
	}
	this->direction = right;
	int x = 0;
	int y = 0;
	int startValue;
	int endValue;
	switch (this->type) {
		case inward:
			startValue = 0;
			endValue = this->transitionTimer;
			break;
		case outward:
			startValue = this->transitionTimer;
			endValue = this->transitionFrames;
			break;
	}
	for (int i = startValue; i < endValue; i++) {
		// origin square
		this->square.setPosition(view.getCenter().x - viewWidth / 2 + tilesize * x      , view.getCenter().y - viewHeight / 2 + tilesize * y      );
		window.draw(this->square);

		// mirrored square
		this->square.setPosition(view.getCenter().x + viewWidth / 2 - tilesize * (x + 1), view.getCenter().y + viewHeight / 2 - tilesize * (y + 1));
		window.draw(this->square);

		switch (this->direction) {
			case up:
				if (y == x + 1) {
					this->direction = right;
					x++;
				} else {
					y--;
				}
				break;
			case down:
				if (y == x - 1) {
					this->direction = left;
					x--;
				} else {
					y++;
				}
				break;
			case left:
				if (x == horizontalTiles - y - 1) {
					this->direction = up;
					y--;
				} else {
					x--;
				}
				break;
			case right:
				if (x == horizontalTiles - y - 1) {
					this->direction = down;
					y++;
				} else {
					x++;
				}
				break;
		}
	}
}
