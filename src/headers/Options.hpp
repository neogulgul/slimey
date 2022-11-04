#pragma once

#include "Button.hpp"

struct OptionButton : Button
{
	bool* option;

	OptionButton(Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position, bool* _option);

	void updateString();
};

struct Options
{
	static const bool default_music = false; // true
	static const bool default_SFX   =  true;
	static const bool default_FPS   = false;
	static const bool default_debug = false;

	bool music = default_music;
	bool SFX   = default_SFX;
	bool FPS   = default_FPS;
	bool debug = default_debug;

	float volumeMusic = 0.5;
	float volumeSFX   = 0.5;

	sf::Texture barOnTexture;
	sf::Texture barOffTexture;

	sf::Sprite barOn;
	sf::Sprite barOff;

	Button resetButton;
	std::vector<OptionButton> optionButtons;
	std::vector<Button*> volumeBarMusic;
	std::vector<Button*> volumeBarSFX;

	Options();

	void reset();
};
