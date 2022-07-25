#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/State.hpp"
#include "headers/Transition.hpp"

Transition::Transition(Type type) {
	this->changeType(type);
}

void Transition::to(State destination) {
	this->ongoing = true;
	this->destination = destination;
}

void Transition::changeType(Type type) {
	this->type = type;
	this->square.setFillColor(sf::Color::Black);
	switch (this->type) {
		case fade:
			this->square.setSize(sf::Vector2f(viewWidth, viewHeight));
			break;
		case spiral:
			this->square.setSize(sf::Vector2f(tilesize, tilesize));
			break;
	}
}

void Transition::reset() {
	this->transitionTimer      = 0;
	this->transitionDelayTimer = 0;
	switch (this->way) {
		case inward:
			this->way = outward;
			break;
		case outward:
			this->way = inward;
			break;
	}
	if (this->inwardComplete && this->outwardComplete) {
		this->inwardComplete  = false;
		this->outwardComplete = false;
		this->ongoing         = false;
	}
}

void Transition::fadeAnimation(sf::RenderWindow &window, sf::View view) {
	this->square.setPosition(view.getCenter().x - viewWidth / 2, view.getCenter().y - viewHeight / 2);
	switch (this->way) {
		case inward:
			this->square.setFillColor(sf::Color(0, 0, 0, ((float)(                         this->transitionTimer) / this->transitionFrames) * 255));
			break;
		case outward:
			this->square.setFillColor(sf::Color(0, 0, 0, ((float)(this->transitionFrames - this->transitionTimer) / this->transitionFrames) * 255));
			break;
	}
	window.draw(this->square);
}

void Transition::spiralAnimation(sf::RenderWindow &window, sf::View view) {
	this->direction = right;
	int x = 0;
	int y = 0;
	int startValue;
	int endValue;
	switch (this->way) {
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

void Transition::draw(sf::RenderWindow &window, sf::View view) {
	if (this->transitionTimer < this->transitionFrames) {
		this->transitionTimer += this->transitionSpeed;
	} else {
		switch (this->way) {
			case inward:
				this->transitionDelayTimer++;
				if (this->transitionDelayTimer == this->transitionDelayFrames) {
					this->inwardComplete = true;
					this->reset();
				}
				break;
			case outward:
				this->outwardComplete = true;
				this->reset();
				break;
		}
	}
	switch (this->type) {
		case fade:
			this->fadeAnimation(window, view);
			break;
		case spiral:
			this->spiralAnimation(window, view);
			break;
	}
}
