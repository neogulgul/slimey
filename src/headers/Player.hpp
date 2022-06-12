#pragma once

#include "Map.hpp"

class Player {
	public:
		float x;
		float y;
		float xSpawn;
		float ySpawn;
		int width = 14;
		int height = 12;
		float xVelocity = 0;
		float yVelocity = 0;
		float maxHorizontalVelocity = 2;
		float acceleration = 0.2;
		float deceleration = 0.3;

		sf::Texture texture;
		sf::Sprite sprite;

		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;

		bool onground = false;
		bool jump = false;

		Player(int xCord, int yCord);
		void input();
		void checkCollision(Map map);
		void update(Map map);
		void draw(sf::RenderWindow &window);
};
