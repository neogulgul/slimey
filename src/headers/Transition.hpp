#pragma once

#include "global.hpp"
#include "State.hpp"

class Transition {
	public:
		int transitionSpeed       = 2;
		int transitionTimer       = 0;
		int transitionFrames      = 128;
		int transitionDelayTimer  = 0;
		int transitionDelayFrames = 32;

		sf::RectangleShape square;

		Direction direction;
		enum Type { inward, outward };
		Type type = inward;

		State destination;

		bool ongoing = false;
		bool inwardComplete = false;
		bool outwardComplete = false;

		Transition();
		void to(State destination);
		void reset();
		void draw(sf::RenderWindow &window, sf::View view);
};
