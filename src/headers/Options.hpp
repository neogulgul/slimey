#pragma once

#include "Button.hpp"

struct OptionButton : Button
{
	bool *option;

	OptionButton(Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f position, bool *_option);

	void update();
};

struct Options
{
	bool music = true;
	bool SFX   = true;
	bool FPS   = false;
	bool debug = false;

	std::vector<OptionButton> optionButtons;
	Button resetButton;

	Options();

	void reset();
};
