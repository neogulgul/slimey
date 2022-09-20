#pragma once

#include "Map.hpp"
#include "SoundManager.hpp"

struct Player
{
	float x;
	float y;
	float xCordSpawn;
	float yCordSpawn;
	int width  = 14;
	int height = 12;
	float xVelocity = 0;
	float yVelocity = 0;
	float maxMoveVelocity  = 2;
	float terminalVelocity = 8;
	float acceleration = 0.2;
	float deceleration = 0.3;

	int animationFrames        = 4;
	int animationFrameDuration = 8;
	int animationTimer         = 0;

	int deathFrames         = 8;
	int deathFramesDuration = 3;
	int deathTimer          = 0;

	bool alive = true;
	bool aliveLastFrame = true;
	bool resurrection = false;

	bool up    = false;
	bool down  = false;
	bool left  = false;
	bool right = false;

	float jumpForce     = 2.2;
	int jumpFrames      = 11;
	int jumpTimer       = 0;
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
	void checkCollision(Map &map);
	void update(Map &map, bool locked, SoundManager &soundManager);
	void draw(sf::RenderWindow &window, sf::View &view, sf::Sprite &playerSprite, sf::Sprite &playerDeathSprite, sf::Sprite &offscreenCircleSprite, bool paused, bool drawHitbox);
};
