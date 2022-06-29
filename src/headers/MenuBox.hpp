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
		int width;
		int height;
		int padding;

		MenuBox(sf::Font font, int fontSize, std::string string, int x, int y, bool centerX, bool centerY, int width, int height, int padding);
		void draw(sf::RenderWindow &window, sf::View &view, bool active);
};
