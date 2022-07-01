#include <cmath>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/MenuBox.hpp"
#include "headers/Player.hpp"
#include "headers/State.hpp"
#include "headers/Transition.hpp"

void drawMenu(sf::RenderWindow &window, sf::View &view, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f menuSelection) {
	for (int x = 0; x < menu.size(); x++) {
		for (int y = 0; y < menu.at(x).size(); y++) {
			bool active = false;
			if (x == menuSelection.x && y == menuSelection.y) {
				active = true;
			}
			menu.at(x).at(y).draw(window, view, active);
		}
	}
}

void startMenu(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &entered, bool &escaped) {
	if (menu.size() == 0) {
		menu.push_back(std::vector<MenuBox>{
			MenuBox(smallFont, smallFontSize, "Start", viewWidth / 2, viewHeight / 2, true, true, 0, 0, 15),
			MenuBox(smallFont, smallFontSize, "Exit", viewWidth / 2, viewHeight / 2 + 30, true, true, 0, 0, 15)
		});
	}

	textBox(window, view, largeFont, largeFontSize, "Slimey", viewWidth / 2, viewHeight / 4, true, true, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);

	drawMenu(window, view, menu, menuSelection);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !entered) {
		entered = true;
		std::string menuBoxString = menu.at(menuSelection.x).at(menuSelection.y).string;
		if (menuBoxString == "Start") {
			state = levelSelectState;
		} else if (menuBoxString == "Exit") {
			window.close();
		}
	}
}

void levelSelect(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &entered, bool &escaped, Map &map, Player &player, Transition &transition) {
	if (menu.size() == 0) {
		int boxWidth      = 24;
		int boxHeight     = 24;
		int rowsMargin    = 10;
		int columnsMargin = 10;
		int rows = 3;
		int columns = 5;
		int currentRow = 0;
		int currentColumn = 0;
		for (int i = 0; i < columns; i++) {
			std::vector<MenuBox> column;
			menu.push_back(column);
		}
		for (int i = 1; i <= rows * columns; i++) {
			menu.at(currentColumn).push_back(MenuBox(smallFont, smallFontSize, std::to_string(i),
				viewWidth / 2 + currentColumn * (boxWidth + columnsMargin) - (columns * boxWidth + (columns - 1) * columnsMargin) / 2 + boxWidth / 2,
				viewHeight / 2 + currentRow * (boxHeight + rowsMargin) - (rows * boxHeight + (rows - 1) * rowsMargin) / 2 + boxHeight / 2,
				true, true, boxWidth, boxHeight, 0));
			currentColumn++;
			if (currentColumn == columns) {
				currentRow++;
				currentColumn = 0;
			}
		}
	}

	textBox(window, view, largeFont, largeFontSize, "Level Select", viewWidth / 2, 15, true, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);

	drawMenu(window, view, menu, menuSelection);

	if (transition.ongoing) {
		transition.draw(window, view);
		if (transition.outwardComplete) {
			transition.reset();
		}
	} else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !escaped) {
			escaped = true;
			state = startMenuState;
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && !entered) {
			entered = true;
			map = Map(stoi(menu.at(menuSelection.x).at(menuSelection.y).string)); // converts map string (number) to int
			player = Player(map.spawn.x, map.spawn.y);
			state = gameState;
		}
	}
}

void game(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &entered, bool &escaped, Map &map, Player &player, Transition &transition, sf::Sprite &playerSprite, sf::Sprite &playerDeathSprite, sf::Sprite &offscreenCircleSprite, sf::Sprite &tilesetSprite, bool &paused) {
	if (menu.size() == 0) {
		menu.push_back(std::vector<MenuBox>{
			MenuBox(smallFont, smallFontSize, "Yes", viewWidth / 2 - 25, viewHeight / 2, true, true, 0, 0, 15)
		});
		menu.push_back(std::vector<MenuBox>{
			MenuBox(smallFont, smallFontSize, "No", viewWidth / 2 + 25, viewHeight / 2, true, true, 0, 0, 15)
		});
	}

	map.draw(window, view, tilesetSprite, paused);

	player.update(map, paused);
	player.draw(window, view, playerSprite, playerDeathSprite, offscreenCircleSprite, paused);

	if (paused) {
		textBox(window, view, largeFont, largeFontSize, "", viewWidth / 2, viewHeight / 2, false, false, viewWidth, viewHeight, 0, textColor, sf::Color(0, 0, 0, 128), sf::Color::Transparent);
		textBox(window, view, largeFont, largeFontSize, "Go to level select?", viewWidth / 2, viewHeight / 4, true, true, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
		drawMenu(window, view, menu, menuSelection);
	} else if (!map.cleared) {
		map.levelTime = map.levelClock.getElapsedTime().asSeconds() - map.pauseTime;

		std::stringstream levelTimer;
		levelTimer << std::floor(map.levelTime * 100) / 100;

		std::stringstream xPlayerPosition;
		std::stringstream yPlayerPosition;
		std::stringstream xPlayerVelocity;
		std::stringstream yPlayerVelocity;
		xPlayerPosition << "xPos: " << player.x;
		yPlayerPosition << "yPos: " << player.y;
		xPlayerVelocity << "xVel: " << player.xVelocity;
		yPlayerVelocity << "yVel: " << player.yVelocity;

		textBox(window, view, smallFont, smallFontSize, levelTimer.str()     , 10, 10                 , false, false, 0, 0, 5, textColor, sf::Color(0, 0, 0, 64), sf::Color(0, 0, 0, 128));
		textBox(window, view, smallFont, smallFontSize, xPlayerPosition.str(), 10, viewHeight / 2 + 20, false, true , 0, 0, 5, textColor, sf::Color(0, 0, 0, 64), sf::Color(0, 0, 0, 128));
		textBox(window, view, smallFont, smallFontSize, yPlayerPosition.str(), 10, viewHeight / 2 + 40, false, true , 0, 0, 5, textColor, sf::Color(0, 0, 0, 64), sf::Color(0, 0, 0, 128));
		textBox(window, view, smallFont, smallFontSize, xPlayerVelocity.str(), 10, viewHeight / 2 + 60, false, true , 0, 0, 5, textColor, sf::Color(0, 0, 0, 64), sf::Color(0, 0, 0, 128));
		textBox(window, view, smallFont, smallFontSize, yPlayerVelocity.str(), 10, viewHeight / 2 + 80, false, true , 0, 0, 5, textColor, sf::Color(0, 0, 0, 64), sf::Color(0, 0, 0, 128));

		updateView(view, player.x + player.width / 2, player.y + player.height / 2, map.size, false);
	} else {
		transition.ongoing = true;
	}

	if (transition.ongoing) {
		transition.draw(window, view);
		if (transition.inwardComplete) {
			transition.reset();
			state = levelSelectState;
		}
	} else {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && !escaped) {
			escaped = true;
			if (paused) {
				paused = false;
				map.pauseTime += map.pauseClock.getElapsedTime().asSeconds();
			} else {
				paused = true;
				menuSelection = sf::Vector2f(1, 0);
				map.pauseClock.restart();
			}
		} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) && paused) {
			entered = true;
			std::string menuBoxString = menu.at(menuSelection.x).at(menuSelection.y).string;
			if (menuBoxString == "Yes") {
				transition.ongoing = true;
			} else if (menuBoxString == "No") {
				paused = false;
			}
		}
	}
}
