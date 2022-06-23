#include <string>
#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/Player.hpp"

Map map("resources/textures/map.png");
Player player(map.spawn.x, map.spawn.y);
bool gaming = false;

// TODO: clean up, level select & multiple levels

void menu(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font bigFont) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
		gaming = true;
		map.clock.restart();
		updateView(view, player.x + player.width / 2, player.y + player.height / 2, map.size, true);
		return;
	}

	sf::Text title("Slimey", bigFont, bigFontSize);
	sf::FloatRect titleRect = title.getGlobalBounds();
	title.setPosition(sf::Vector2f(-titleRect.width / 2, -50));
	title.setOrigin(0, title.getCharacterSize() - titleRect.height);

	sf::Text text("Press enter to continue", smallFont, smallFontSize);
	sf::FloatRect textRect = text.getGlobalBounds();
	sf::RectangleShape textBackground(sf::Vector2f(textRect.width + 25, textRect.height + 25));
	text.setPosition(sf::Vector2f(-textRect.width / 2, -textRect.height / 2));
	text.setOrigin(0, text.getCharacterSize() - textRect.height);
	textBackground.setPosition(sf::Vector2f(-textBackground.getSize().x / 2, -textBackground.getSize().y / 2));
	textBackground.setFillColor(altBackground);

	window.draw(title);
	window.draw(textBackground);
	window.draw(text);

	view.setCenter(0, 0);
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
