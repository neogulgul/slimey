#include <string>
#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/Player.hpp"

Map map("resources/textures/map.png");
Player player(map.spawn.x, map.spawn.y);
bool gaming = false;

bool onExit = false;
bool inputted = false;

// TODO: clean up, level select & multiple levels

void textBox(sf::RenderWindow &window, int x, int y, bool centerX, bool centerY, std::string string, sf::Font font, int fontSize, int padding, sf::Color backgroundColor, bool active) {
	sf::Text text(string, font, fontSize);
	sf::FloatRect textRect = text.getGlobalBounds();
	sf::RectangleShape background(sf::Vector2f(textRect.width + padding, textRect.height + padding));
	sf::Vector2f adjustment;
	if (centerX) {
		adjustment.x = -textRect.width / 2;
	}
	if (centerY) {
		adjustment.y = -textRect.height / 2;
	}

	text.setOrigin(0, text.getCharacterSize() - textRect.height);
	text.setPosition(sf::Vector2f(x + adjustment.x, y + adjustment.y));
	background.setPosition(sf::Vector2f(x + adjustment.x - padding / 2, y + adjustment.y - padding / 2));
	background.setFillColor(backgroundColor);
	if (active) {
		background.setOutlineColor(sf::Color::Green);
		background.setOutlineThickness(-1);
	}

	window.draw(background);
	window.draw(text);
}

void menu(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font bigFont) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		if (onExit) {
			window.close();
		} else {
			gaming = true;
			map.clock.restart();
			updateView(view, player.x + player.width / 2, player.y + player.height / 2, map.size, true);
		}
		return;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		if (!inputted) {
			if (onExit) {
				onExit = false;
			} else {
				onExit = true;
			}
			inputted = true;
		}
	} else {
		inputted = false;
	}

	textBox(window, viewWidth / 2, viewHeight / 4, true, true, "slimey", bigFont, bigFontSize, 0, sf::Color::Transparent, false);
	if (!onExit) {
		textBox(window, viewWidth / 2, viewHeight / 2, true, true, "start", smallFont, smallFontSize, 20, altBackground, true);
		textBox(window, viewWidth / 2, viewHeight / 2 + 30, true, true, "exit", smallFont, smallFontSize, 20, altBackground, false);
	} else {
		textBox(window, viewWidth / 2, viewHeight / 2, true, true, "start", smallFont, smallFontSize, 20, altBackground, false);
		textBox(window, viewWidth / 2, viewHeight / 2 + 30, true, true, "exit", smallFont, smallFontSize, 20, altBackground, true);
	}

	view.setCenter(viewWidth / 2, viewHeight / 2);
}

void game(sf::RenderWindow &window, sf::View &view, Map &map, Player &player) {
	if (map.cleared) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
			map.cleared = false;
			map.clock.restart();
			player.setPosition(player.xCordSpawn, player.yCordSpawn);
			updateView(view, player.x + player.width / 2, player.y + player.height / 2, map.size, true);
			return;
		}

		sf::Font font;
		font.loadFromFile("resources/fonts/FiveByFive.ttf");
		// making the font not blurry
		sf::Texture& fontTexture = const_cast<sf::Texture&>(font.getTexture(smallFontSize));
		fontTexture.setSmooth(false);
		std::string textString = "You did it!\nIt took you " + std::to_string(map.clearTime) + " seconds.\nPress enter if you want to restart.";

		sf::Text text(textString, font, smallFontSize);
		sf::FloatRect textRect = text.getGlobalBounds();
		text.setPosition(sf::Vector2f(-textRect.width / 2, -textRect.height / 2));
		text.setOrigin(0, text.getCharacterSize() - textRect.height);
		window.draw(text);

		view.setCenter(0, 0);
		return;
	}

	map.draw(window, view);

	player.update(map);
	player.draw(window);

	updateView(view, player.x + player.width / 2, player.y + player.height / 2, map.size, false);
}

int main() {
	sf::RenderWindow window(sf::VideoMode(viewWidth * windowScale, viewHeight * windowScale), "Slimey", sf::Style::Default);
	window.setFramerateLimit(FPS);
	sf::View view(sf::Vector2f(0, 0), sf::Vector2f(viewWidth, viewHeight));
	sf::Image icon;
	icon.loadFromFile("resources/textures/icon.png");
	sf::Event event;

	sf::Font smallFont;
	sf::Font bigFont;
	smallFont.loadFromFile("resources/fonts/FiveByFive.ttf");
	bigFont.loadFromFile("resources/fonts/FiveByFive.ttf");
	// avoiding blurry font
	sf::Texture& smallFontTexture = const_cast<sf::Texture&>(smallFont.getTexture(smallFontSize));
	sf::Texture& bigFontTexture = const_cast<sf::Texture&>(bigFont.getTexture(bigFontSize));
	smallFontTexture.setSmooth(false);
	bigFontTexture.setSmooth(false);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (window.hasFocus()) {
			window.clear(background);

			if (gaming) {
				game(window, view, map, player);
			} else {
				menu(window, view, smallFont, bigFont);
			}

			window.setView(view);
			window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
			window.display();
		}
	}

	return 0;
}
