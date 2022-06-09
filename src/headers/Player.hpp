#pragma once

#include "CollisionObject.hpp"

class Player {
	public:
		int width = 14;
		int height = 12;

		float x, y;

		float xDelta, yDelta;

		float xVelocity = 0;
		float yVelocity = 0;
		float maxHorizontalVelocity = 2;
		float acceleration = 0.2;
		float deceleration = 0.3;

		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;

		bool jump = false;

		bool onground = false;

		sf::Texture texture;
		sf::Sprite sprite;

		sf::RectangleShape outline;
		float outlineThickness = 0.5;

		Player();
		void input();
		void collision(std::vector<CollisionObject> CollisionObjects);
		void update(std::vector<CollisionObject> CollisionObjects);
		void draw(sf::RenderWindow &window, std::vector<CollisionObject> CollisionObjects);
};
