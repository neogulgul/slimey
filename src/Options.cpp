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

Options::Options()
{
	optionButtons = {
		OptionButton(End, Center, {viewWidth * 0.75, viewHeight - 150}, &music),
		OptionButton(End, Center, {viewWidth * 0.75, viewHeight - 120}, &SFX  ),
		OptionButton(End, Center, {viewWidth * 0.75, viewHeight -  90}, &FPS  ),
		OptionButton(End, Center, {viewWidth * 0.75, viewHeight -  60}, &debug)
	};
	resetButton = Button("Reset Options", {viewWidth / 2, 20}, Center, Center, {viewWidth / 2, viewHeight - 30});
}

void Options::reset()
{
	music = true;
	SFX   = true;
	FPS   = false;
	debug = false;
	for (OptionButton &button : optionButtons)
	{
		button.updateString();
	}
}
