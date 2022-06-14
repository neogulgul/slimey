#pragma once

#include "Map.hpp"

class Player {
	public:
		float x;
		float y;
		float xCordSpawn;
		float yCordSpawn;
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

		float jumpForce = 3.2;
		int preJumpFrames = 10;
		int preJumpTimer = 0;
		int postJumpFrames = 5;
		int postJumpTimer = 0;
		bool jump = false;
		bool jumped = false;
		bool onground = false;

		Player(int xCord, int yCord);
		void setPosition(int xCord, int yCord);
		void input();
		void jumping();
		void checkCollision(Map map);
		void update(Map map);
		void draw(sf::RenderWindow &window);
};
