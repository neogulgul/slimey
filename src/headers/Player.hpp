#pragma once

#include "Map.hpp"

class Player {
	public:
		float x;
		float y;
		float xCordSpawn;
		float yCordSpawn;
		int width  = 14;
		int height = 12;
		float xVelocity = 0;
		float yVelocity = 0;
		float maxMoveVelocity = 2;
		float maxAirVelocity  = 8;
		float acceleration = 0.2;
		float deceleration = 0.3;

		bool hitbox = false;

		int animationFrames        = 4;
		int animationFrameDuration = 8;
		int animationTimer         = 0;

		int deathFrames         = 8;
		int deathFramesDuration = 3;
		int deathTimer          = 0;

		bool alive = true;

		bool up    = false;
		bool down  = false;
		bool left  = false;
		bool right = false;

		float jumpForce     = 3.2;
		int preJumpFrames   = 10;
		int preJumpTimer    = 0;
		int postJumpFrames  = 5;
		int postJumpTimer   = 0;
		bool jump           = false;
		bool jumped         = false;
		bool onGround       = false;
		bool onIce          = false;
		bool hitCeil        = false;
		bool hitCeilBounce  = false;
		bool hitFloor       = false;
		bool hitFloorBounce = false;
		bool hitWall        = false;
		bool hitWallBounce  = false;
		float bounciness    = 1.2;   // the amount the velocity is multiplied with after bounce
		float baseBounce    = 2;     // the minimum amount of velocity after bounce

		Player();
		Player(int xCord, int yCord);
		void setPosition(int xCord, int yCord);
		void input(Map &map);
		void jumping();
		void death(Map &map);
		void levelClear(Map &map);
		bool validTile(Map map, int xCord, int yCord);
		void checkCollision(Map &map);
		void update(Map &map);
		void draw(sf::RenderWindow &window, sf::View &view, sf::Sprite playerSprite, sf::Sprite playerDeathSprite, sf::Sprite offscreenCircleSprite);
};
