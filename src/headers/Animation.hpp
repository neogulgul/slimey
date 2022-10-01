#pragma once

struct Animation
{
	unsigned int timer = 0;
	unsigned int frame = 0;
	unsigned int frameCount;
	unsigned int frameDuration;

	Animation();
	Animation(unsigned int _frameCount, unsigned int _frameDuration = 1);

	void reset();
	void update();
};
