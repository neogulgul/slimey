#pragma once

class MenuBox {
	public:
		sf::Font font;
		int fontSize;
		std::string string;
		int x;
		int y;
		bool centerX;
		bool centerY;
		bool rightOrigin;
		bool bottomOrigin;
		int width;
		int height;
		int padding;
		sf::Color backgroundColor;
		sf::Color outlineColor;
		sf::Color activeBackgroundColor;
		sf::Color activeOutlineColor;

		MenuBox(sf::Font font, int fontSize, std::string string, int x, int y, bool centerX, bool centerY, bool rightOrigin, bool bottomOrigin, int width, int height, int padding);
		void draw(sf::RenderWindow &window, sf::View &view, bool active);
};
