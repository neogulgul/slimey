#include <algorithm>
#include <cmath>

#include "headers/Options.hpp"

#define sizeOfOptionButton {20, 20}

OptionButton::OptionButton(Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position, bool* _option)
: Button("", sizeOfOptionButton, horizontalAlignment, verticalAlignment, position)
{
	option = _option;
	updateString();
}

void OptionButton::updateString()
{
	if (*option)
	{
		string = "x";
	}
	else
	{
		string = "";
	}
}

VolumeBar::VolumeBar() {}

VolumeBar::VolumeBar(sf::Vector2f position, float* _volume, bool* _hoveringVolumeSlider, bool* _usingVolumeSlider)
{
	volume               = _volume;
	hoveringVolumeSlider = _hoveringVolumeSlider;
	usingVolumeSlider    = _usingVolumeSlider;

	// shape
	sf::Vector2f shapeSize(viewWidth / 2, 10);
	shape.setSize(shapeSize);
	shape.setOrigin(shapeSize.x / 2, shapeSize.y / 2);
	shape.setPosition(position);
	shape.setFillColor(inactiveMenuboxBackground);
	shape.setOutlineColor(inactiveMenuboxForeground);
	shape.setOutlineThickness(-2);

	bounds = shape.getGlobalBounds();
	bounds.top    += 2;
	bounds.left   += 2;
	bounds.width  -= 4;
	bounds.height -= 4;

	// separator
	sf::Vector2f separatorSize(2, 6);
	volumeSeparator.setSize(separatorSize);
	volumeSeparator.setOrigin(separatorSize.x / 2, separatorSize.y / 2);
	volumeSeparator.setFillColor(inactiveMenuboxForeground);

	// slider
	sf::Vector2f sliderSize(4, 8);
	volumeSlider.setSize(sliderSize);
	volumeSlider.setOrigin(sliderSize.x / 2, sliderSize.y / 2);
	volumeSlider.setFillColor(activeMenuboxBackground);
	volumeSlider.setOutlineColor(activeMenuboxForeground);
	volumeSlider.setOutlineThickness(-1);

	updateVolumeSlider();
}

void VolumeBar::updateVolumeSlider()
{
	volumeSlider.setPosition(bounds.left + bounds.width * *volume, bounds.top + bounds.height / 2);
}

void VolumeBar::update(sf::Vector2f mousePosition)
{
	if (bounds.contains(mousePosition) || volumeSlider.getGlobalBounds().contains(mousePosition))
	{
		*hoveringVolumeSlider = true;
		if (pressing(sf::Mouse::Left) && !*usingVolumeSlider)
		{
			holdingSlider = true;
			*usingVolumeSlider = true;
		}
	}

	if (!pressing(sf::Mouse::Left))
	{
		holdingSlider = false;
		*usingVolumeSlider = false;
	}

	if (holdingSlider)
	{
		*volume = std::clamp(std::ceil((mousePosition.x - bounds.left) / bounds.width * 100) / 100, 0.01f, 0.99f);
		updateVolumeSlider();
	}
}

void VolumeBar::draw(sf::RenderWindow* window, sf::View* view)
{
	window->draw(shape);

	// drawing the volume separators
	for (unsigned int i = 1; i < 10; i++)
	{
		volumeSeparator.setPosition(bounds.left + i * bounds.width / 10, bounds.top + bounds.height / 2);
		window->draw(volumeSeparator);
	}

	window->draw(volumeSlider);
}

Options::Options()
{
	resetButton = Button("Reset Options", {viewWidth / 2, 20}, Center, End, {viewWidth / 2, viewHeight - 10});
	optionButtons = {
		OptionButton(End, End, {viewWidth * 0.75, viewHeight - 150}, &music),
		OptionButton(End, End, {viewWidth * 0.75, viewHeight - 110}, &SFX  ),
		OptionButton(End, End, {viewWidth * 0.75, viewHeight -  70}, &FPS  ),
		OptionButton(End, End, {viewWidth * 0.75, viewHeight -  40}, &debug)
	};

	volumeBars = {
		VolumeBar({viewWidth / 2, viewHeight - 140}, &volumeMusic, &hoveringVolumeSlider, &usingVolumeSlider),
		VolumeBar({viewWidth / 2, viewHeight - 100}, &volumeSFX, &hoveringVolumeSlider, &usingVolumeSlider)
	};
}

void Options::reset()
{
	music = default_music;
	SFX   = default_SFX;
	FPS   = default_FPS;
	debug = default_debug;

	volumeMusic = default_volumeMusic;
	volumeSFX   = default_volumeSFX;

	for (OptionButton &button : optionButtons)
	{
		button.updateString();
	}

	for (VolumeBar &bar : volumeBars)
	{
		bar.updateVolumeSlider();
	}
}
