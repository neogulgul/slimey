#pragma once

class Player {
	public:
		int width = 14;
		int height = 12;

		float x, y;

		float xVelocity = 0;
		float yVelocity = 0;
		float acceleration = 0.5;
		float deceleration = 0.1;

		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;

		bool onground = false;

		sf::Texture texture;
		sf::Sprite sprite;

		Player();
		void input();
		void collision();
		void update();
		void draw(sf::RenderWindow &window);
};
