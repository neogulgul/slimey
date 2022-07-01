#pragma once

class Transition {
	public:
		int transitionTimer       = 0;
		int transitionFrames      = 128;
		int transitionDelayTimer  = 0;
		int transitionDelayFrames = 32;

		sf::RectangleShape square;

		enum Direction { up, down, left, right };
		Direction direction;
		enum Type { inward, outward };
		Type type = inward;

		bool ongoing = false;
		bool inwardComplete = false;
		bool outwardComplete = false;

		Transition();
		void reset();
		void draw(sf::RenderWindow &window, sf::View view);
};
