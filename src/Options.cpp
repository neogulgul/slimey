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
	barOnTexture .loadFromFile("assets/textures/bar-on.png");
	barOffTexture.loadFromFile("assets/textures/bar-off.png");

	barOn .setTexture(barOnTexture);
	barOff.setTexture(barOffTexture);

	resetButton = Button("Reset Options", {viewWidth / 2, 20}, Center, End, {viewWidth / 2, viewHeight - 10});
	optionButtons = {
		OptionButton(End, End, {viewWidth * 0.75, viewHeight - 150}, &music),
		OptionButton(End, End, {viewWidth * 0.75, viewHeight - 110}, &SFX  ),
		OptionButton(End, End, {viewWidth * 0.75, viewHeight -  70}, &FPS  ),
		OptionButton(End, End, {viewWidth * 0.75, viewHeight -  40}, &debug)
	};

	volumeBarMusic = {
		new Button(&barOff, Center, Center, {viewWidth / 2 - 45, viewHeight - 140}),
		new Button(&barOff, Center, Center, {viewWidth / 2 - 35, viewHeight - 140}),
		new Button(&barOff, Center, Center, {viewWidth / 2 - 25, viewHeight - 140}),
		new Button(&barOff, Center, Center, {viewWidth / 2 - 15, viewHeight - 140}),
		new Button(&barOff, Center, Center, {viewWidth / 2 -  5, viewHeight - 140}),
		new Button(&barOff, Center, Center, {viewWidth / 2 +  5, viewHeight - 140}),
		new Button(&barOff, Center, Center, {viewWidth / 2 + 15, viewHeight - 140}),
		new Button(&barOff, Center, Center, {viewWidth / 2 + 25, viewHeight - 140}),
		new Button(&barOff, Center, Center, {viewWidth / 2 + 35, viewHeight - 140}),
		new Button(&barOff, Center, Center, {viewWidth / 2 + 45, viewHeight - 140})
	};
	volumeBarSFX = {
		new Button(&barOff, Center, Center, {viewWidth / 2 - 45, viewHeight - 100}),
		new Button(&barOff, Center, Center, {viewWidth / 2 - 35, viewHeight - 100}),
		new Button(&barOff, Center, Center, {viewWidth / 2 - 25, viewHeight - 100}),
		new Button(&barOff, Center, Center, {viewWidth / 2 - 15, viewHeight - 100}),
		new Button(&barOff, Center, Center, {viewWidth / 2 -  5, viewHeight - 100}),
		new Button(&barOff, Center, Center, {viewWidth / 2 +  5, viewHeight - 100}),
		new Button(&barOff, Center, Center, {viewWidth / 2 + 15, viewHeight - 100}),
		new Button(&barOff, Center, Center, {viewWidth / 2 + 25, viewHeight - 100}),
		new Button(&barOff, Center, Center, {viewWidth / 2 + 35, viewHeight - 100}),
		new Button(&barOff, Center, Center, {viewWidth / 2 + 45, viewHeight - 100})
	};
}

void Options::reset()
{
	music = default_music;
	SFX   = default_SFX;
	FPS   = default_FPS;
	debug = default_debug;
	for (OptionButton &button : optionButtons)
	{
		button.updateString();
	}
}
