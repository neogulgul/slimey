#include "headers/Animation.hpp"

Animation::Animation()
{}

Animation::Animation(unsigned int _frameCount, unsigned int _frameDuration)
{
	frameCount    = _frameCount;
	frameDuration = _frameDuration;
}

void Animation::update()
{
	timer++;
	if (timer % frameDuration == 0)
	{
		frame++;
	}
	if (frame == frameCount)
	{
		timer = 0;
		frame = 0;
	}
}
