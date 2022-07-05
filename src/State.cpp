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

void loadLevel(sf::View &view, State &state, Transition &transition, Map &map, Player &player, int mapIndex) {
	map = Map(mapIndex);
	player = Player(map.spawn.x, map.spawn.y);
	updateView(view, player.x + player.width / 2, player.y + player.height / 2, map.size, true);
	transition.to(gameState);
}

void startScreen(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape) {
	if (menu.size() == 0) {
		menu.push_back(std::vector<MenuBox>{
			MenuBox(smallFont, smallFontSize, "Start", viewWidth / 2, viewHeight / 2     , true, true, false, false, 0, 0, 15),
			MenuBox(smallFont, smallFontSize, "Exit" , viewWidth / 2, viewHeight / 2 + 30, true, true, false, false, 0, 0, 15)
		});
	}

	textBox(window, view, largeFont, largeFontSize, "Slimey", viewWidth / 2, viewHeight / 4, true, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);

	/// controls ///
	textBox(window, view, smallFont, smallFontSize, "Up - W"         , 10            , viewHeight / 2 - 40, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize, "Down - S"       , 10            , viewHeight / 2 - 30, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize, "Left - A"       , 10            , viewHeight / 2 - 20, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize, "Right - D"      , 10            , viewHeight / 2 - 10, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize, "Jump - J"       , 10            , viewHeight / 2 + 10, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize, "Reset - R"      , 10            , viewHeight / 2 + 20, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize, "Confirm - Enter", viewWidth - 5 , viewHeight / 2 - 5 , false, true, true , false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize, "Back - Escape"  , viewWidth - 10, viewHeight / 2 + 5 , false, true, true , false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	/// controls ///

	drawMenu(window, view, menu, menuSelection);

	// keyboard inputs
	if (enter) {
		std::string menuBoxString = menu.at(menuSelection.x).at(menuSelection.y).string;
		if (menuBoxString == "Start") {
			state = levelSelectState;
		} else if (menuBoxString == "Exit") {
			window.close();
		}
	}
}

void levelSelect(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, Transition &transition, Map &map, Player &player) {
	if (menu.size() == 0) {
		int boxWidth      = 24;
		int boxHeight     = 24;
		int rowsMargin    = 10;
		int columnsMargin = 10;
		int rows          = 2;
		int columns       = 3;
		int currentRow    = 0;
		int currentColumn = 0;
		for (int i = 0; i < columns; i++) {
			std::vector<MenuBox> column;
			menu.push_back(column);
		}
		for (int i = 1; i <= rows * columns; i++) {
			menu.at(currentColumn).push_back(MenuBox
				(
				smallFont, smallFontSize, std::to_string(i),
				viewWidth / 2 + currentColumn * (boxWidth + columnsMargin) - (columns * boxWidth + (columns - 1) * columnsMargin) / 2 + boxWidth / 2,
				viewHeight / 2 + currentRow * (boxHeight + rowsMargin) - (rows * boxHeight + (rows - 1) * rowsMargin) / 2 + boxHeight / 2,
				true, true, false, false, boxWidth, boxHeight, 0
				)
			);
			currentColumn++;
			if (currentColumn == columns) {
				currentRow++;
				currentColumn = 0;
			}
		}
	}

	textBox(window, view, largeFont, largeFontSize, "Level Select", viewWidth / 2, 15, true, false, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);

	drawMenu(window, view, menu, menuSelection);

	if (transition.ongoing) {
		transition.draw(window, view);
	} else {
		// keyboard inputs
		if (escape) {
			state = startScreenState;
		} else if (enter) {
			int mapIndex = stoi(menu.at(menuSelection.x).at(menuSelection.y).string); // converts map string (number) to int
			loadLevel(view, state, transition, map, player, mapIndex);
		}
	}
}

void levelClear(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, Transition &transition, Map &map, Player &player) {
	if (menu.size() == 0) {
		menu.push_back(std::vector<MenuBox>{
			MenuBox(smallFont, smallFontSize, "Continue", viewWidth / 2, viewHeight / 2     , true, true, false, false, 0, 0, 15),
			MenuBox(smallFont, smallFontSize, "Restart" , viewWidth / 2, viewHeight / 2 + 30, true, true, false, false, 0, 0, 15),
			MenuBox(smallFont, smallFontSize, "Return"  , viewWidth / 2, viewHeight / 2 + 60, true, true, false, false, 0, 0, 15)
		});
	}

	drawMenu(window, view, menu, menuSelection);

	std::stringstream clearTime;
	clearTime << std::floor(map.clearTime * 100) / 100;
	textBox(window, view, smallFont, smallFontSize, clearTime.str(), 10, 10, false, false, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, largeFont, largeFontSize, "Continue to", viewWidth / 2, viewHeight / 4 - 10, true, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, largeFont, largeFontSize, "next level?", viewWidth / 2, viewHeight / 4 + 10, true, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);

	if (transition.ongoing) {
		transition.draw(window, view);
	} else {
		// keyboard inputs
		if (enter) {
			std::string menuBoxString = menu.at(menuSelection.x).at(menuSelection.y).string;
			if (menuBoxString == "Continue") {
				loadLevel(view, state, transition, map, player, map.index + 1);
			} else if (menuBoxString == "Restart") {
				loadLevel(view, state, transition, map, player, map.index);
			} else if (menuBoxString == "Return") {
				transition.to(levelSelectState);
			}
		}
	}
}

void game(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, Transition &transition, Map &map, Player &player, sf::Sprite &playerSprite, sf::Sprite &playerDeathSprite, sf::Sprite &offscreenCircleSprite, sf::Sprite &tilesetSprite, bool &paused) {
	if (menu.size() == 0) {
		menu.push_back(std::vector<MenuBox>{
			MenuBox(smallFont, smallFontSize, "Yes", viewWidth / 2 - 25, viewHeight / 2, true, true, false, false, 0, 0, 15)
		});
		menu.push_back(std::vector<MenuBox>{
			MenuBox(smallFont, smallFontSize, "No" , viewWidth / 2 + 25, viewHeight / 2, true, true, false, false, 0, 0, 15)
		});
	}

	map.draw(window, view, tilesetSprite, paused);

	player.update(map, paused, transition.ongoing);

	player.draw(window, view, playerSprite, playerDeathSprite, offscreenCircleSprite, paused);

	if (map.cleared) {
		transition.to(levelClearState);
	} else if (paused) {
		textBox(window, view, largeFont, largeFontSize, "", viewWidth / 2, viewHeight / 2, false, false, false, false, viewWidth, viewHeight, 0, textColor, sf::Color(0, 0, 0, 128), sf::Color::Transparent);
		textBox(window, view, largeFont, largeFontSize, "Go to level select?", viewWidth / 2, viewHeight / 4, true, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
		drawMenu(window, view, menu, menuSelection);
	} else {
		if (map.valid) {
			map.clearTime += 1.f / FPS;
			std::stringstream clearTime;
			clearTime << std::floor(map.clearTime * 100) / 100;
			textBox(window, view, smallFont, smallFontSize, clearTime.str(), 10, 10, false, false, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
		} else {
			textBox(window, view, largeFont, largeFontSize, "This is a bug", viewWidth / 2, viewHeight / 2, true, true, false, false, 0, 0, 25, textColor, sf::Color::Red, sf::Color::Transparent);
		}

		updateView(view, player.x + player.width / 2, player.y + player.height / 2, map.size, false);
	}

	if (transition.ongoing) {
		transition.draw(window, view);
	} else {
		// keyboard inputs
		if (escape) {
			if (paused) {
				paused = false;
			} else {
				paused = true;
				menuSelection = sf::Vector2f(1, 0);
			}
		} else if (enter) {
			if (paused) {
				std::string menuBoxString = menu.at(menuSelection.x).at(menuSelection.y).string;
				if (menuBoxString == "Yes") {
					transition.to(levelSelectState);
				} else if (menuBoxString == "No") {
					paused = false;
				}
			}
		}
	}
}
