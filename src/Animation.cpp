#include "headers/Animation.hpp"

Animation::Animation() {}

Animation::Animation(unsigned int _frameCount, unsigned int _frameDuration)
{
	frameCount    = _frameCount;
	frameDuration = _frameDuration;
	if (frameCount < 1) { frameCount = 1; }
	if (frameDuration < 1) { frameDuration = 1; }
}

bool Animation::onLastFrame()
{
	return (frame == frameCount - 1 && timer == frameDuration - 1);
}

void Animation::reset()
{
	timer = 0;
	frame = 0;
}

void Animation::update()
{
	timer++;
	if (timer == frameDuration)
	{
		timer = 0;
		frame++;
	}
	if (frame == frameCount)
	{
		frame = 0;
	}
}
