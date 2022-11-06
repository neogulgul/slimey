#pragma once

#include "Button.hpp"

struct OptionButton : Button
{
	bool* option;

	OptionButton(Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position, bool* _option);

	void updateString();
};

struct VolumeBar
{
	sf::RectangleShape shape;
	sf::RectangleShape volumeSeparator;
	sf::RectangleShape volumeSlider;
	sf::FloatRect bounds;
	float* volume;
	bool* hoveringVolumeSlider;
	bool* usingVolumeSlider;

	bool holdingSlider = false;

	VolumeBar();

	VolumeBar(sf::Vector2f position, float* _volume, bool* _hoveringVolumeSlider, bool* _usingVolumeSlider);

	void updateVolumeSlider();

	void update(sf::Vector2f mousePosition);

	void draw(sf::RenderWindow* window, sf::View* view);
};

struct Options
{
	static const bool default_music = false; // true
	static const bool default_SFX   =  true;
	static const bool default_FPS   = false;
	static const bool default_debug = false;

	float default_volumeMusic = 0.5;
	float default_volumeSFX   = 0.5;

	bool music = default_music;
	bool SFX   = default_SFX;
	bool FPS   = default_FPS;
	bool debug = default_debug;

	float volumeMusic = default_volumeMusic;
	float volumeSFX   = default_volumeSFX;

	bool hoveringVolumeSlider = false;
	bool    usingVolumeSlider = false;

	sf::Texture barOnTexture;
	sf::Texture barOffTexture;

	sf::Sprite barOn;
	sf::Sprite barOff;

	Button resetButton;
	std::vector<OptionButton> optionButtons;
	std::vector<VolumeBar> volumeBars;

	Options();

	void reset();
};
