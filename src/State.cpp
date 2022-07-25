#include <cmath>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "headers/Bullet.hpp"
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/MenuBox.hpp"
#include "headers/Player.hpp"
#include "headers/SettingsManager.hpp"
#include "headers/SoundManager.hpp"
#include "headers/SpriteManager.hpp"
#include "headers/State.hpp"
#include "headers/Transition.hpp"

std::string boolToString(bool boolean) {
	if (boolean) {
		return "true";
	}

	return "false";
}

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
	transition.to(STATE_game);
}

void splashScreen(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, bool &enter, sf::Clock beginningOfTime) {
	sf::Texture slimeyTexture;
	slimeyTexture.loadFromFile("resources/textures/slimey.png");
	sf::Sprite slimeySprite;
	slimeySprite.setTexture(slimeyTexture);
	slimeySprite.setTextureRect(sf::IntRect(0, 0, 14, 12));
	slimeySprite.setScale(5, 5);
	slimeySprite.setPosition(view.getCenter().x - 7 * 5, view.getCenter().y - 6 * 5);
	window.draw(slimeySprite);

	textBox(window, view, largeFont, largeFontSize, "Thanks for playing!", viewWidth / 2, viewHeight / 4, true, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);

	if (transition.ongoing) {
		transition.draw(window, view);
	} else if (enter || beginningOfTime.getElapsedTime().asSeconds() > 1) {
		transition.to(STATE_startScreen);
	}
}

void startScreen(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, SoundManager &soundManager) {
	if (menu.size() == 0) {
		menu.push_back(std::vector<MenuBox>{
			MenuBox(smallFont, smallFontSize, "Start"  , viewWidth / 2, viewHeight / 2     , true, true, false, false, 60, 20, 0),
			MenuBox(smallFont, smallFontSize, "Options", viewWidth / 2, viewHeight / 2 + 30, true, true, false, false, 60, 20, 0),
			MenuBox(smallFont, smallFontSize, "Exit"   , viewWidth / 2, viewHeight / 2 + 60, true, true, false, false, 60, 20, 0)
		});
	}

	if (soundManager.music.getStatus() == sf::SoundSource::Status::Stopped) {
		soundManager.music.play();
	}

	textBox(window, view, largeFont, largeFontSize, "Slimey", viewWidth / 2, viewHeight / 4, true, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);

	/// controls ///
	textBox(window, view, smallFont, smallFontSize,      "Up - W"     , 10            , viewHeight / 2 - 40, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize,    "Down - S"     , 10            , viewHeight / 2 - 30, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize,    "Left - A"     , 10            , viewHeight / 2 - 20, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize,   "Right - D"     , 10            , viewHeight / 2 - 10, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize,    "Jump - J"     , 10            , viewHeight / 2 + 10, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize,   "Reset - R"     , 10            , viewHeight / 2 + 20, false, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize, "Confirm - Enter" , viewWidth - 5 , viewHeight / 2 - 5 , false, true, true , false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	textBox(window, view, smallFont, smallFontSize,    "Back - Escape", viewWidth - 10, viewHeight / 2 + 5 , false, true, true , false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	/// controls ///

	drawMenu(window, view, menu, menuSelection);

	if (transition.ongoing) {
		transition.draw(window, view);
		if (transition.destination == STATE_exit) {
			static const float originalVolume = soundManager.music.getVolume();
			soundManager.music.setVolume(originalVolume * (1 - (float)transition.transitionTimer / transition.transitionFrames));
		}
	} else {
		if (transition.type == Transition::Type::fade) {
			transition.changeType(Transition::Type::spiral);
		}
		// keyboard inputs
		if (enter) {
			std::string menuBoxString = menu.at(menuSelection.x).at(menuSelection.y).string;
			if (menuBoxString == "Start") {
				state = STATE_levelSelect;
			} else if (menuBoxString == "Options") {
				state = STATE_optionsMenu;
			} else if (menuBoxString == "Exit") {
				transition.changeType(Transition::Type::fade);
				transition.to(STATE_exit);
			}
		}
	}
}

void optionsMenu (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, SettingsManager &settingsManager, SoundManager &soundManager) {
	menu.clear();
	std::vector<MenuBox> menuColumn;
	for (int i = 0; i < settingsManager.settings.size(); i++) {
		std::string X = "";
		if (settingsManager.settings.at(i)->active) {
			X = "X"; // revert poo
		}
		menuColumn.push_back(
			MenuBox(smallFont, smallFontSize, X, viewWidth / 2 + 14, viewHeight - 14 - (settingsManager.settings.size() - i) * 20, true, true, false, true, 12, 12, 0)
		);
	}
	menuColumn.push_back(
		MenuBox(smallFont, smallFontSize, "Reset Window Size", viewWidth / 2, viewHeight - 14, true, true, false, true, 128, 12, 0)
	);
	menu.push_back(menuColumn);

	for (int i = 0; i < settingsManager.settings.size(); i++) {
		textBox(window, view, smallFont, smallFontSize, settingsManager.settings.at(i)->name, viewWidth / 2 - 8, viewHeight - 14 - (settingsManager.settings.size() - i) * 20, false, true, true, true, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
	}

	textBox(window, view, largeFont, largeFontSize, "Options", viewWidth / 2, viewHeight / 4, true, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);

	drawMenu(window, view, menu, menuSelection);

	if (escape) {
		state = STATE_startScreen;
	} else if (enter) {
		std::string menuBoxString = menu.at(menuSelection.x).at(menuSelection.y).string;
		if (menuBoxString == "Reset Window Size") {
			window.setSize(sf::Vector2u(windowWidth, windowHeight));
		} else {
			if (settingsManager.settings.at(menuSelection.y)->active) {
				settingsManager.settings.at(menuSelection.y)->active = false;
			} else {
				settingsManager.settings.at(menuSelection.y)->active = true;
			}
		}
	}
}

void levelSelect(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, SoundManager &soundManager, Map &map, Player &player) {
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
			state = STATE_startScreen;
		} else if (enter) {
			int mapIndex = stoi(menu.at(menuSelection.x).at(menuSelection.y).string); // converts map string (number) to int
			loadLevel(view, state, transition, map, player, mapIndex);
		}
	}
}

void levelClear(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, SoundManager &soundManager, Map &map, Player &player) {
	if (menu.size() == 0) {
		menu.push_back(std::vector<MenuBox>{
			MenuBox(smallFont, smallFontSize, "Continue", viewWidth / 2, viewHeight / 2     , true, true, false, false, 60, 20, 0),
			MenuBox(smallFont, smallFontSize, "Restart" , viewWidth / 2, viewHeight / 2 + 30, true, true, false, false, 60, 20, 0),
			MenuBox(smallFont, smallFontSize, "Return"  , viewWidth / 2, viewHeight / 2 + 60, true, true, false, false, 60, 20, 0)
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
				transition.to(STATE_levelSelect);
			}
		}
	}
}

void game(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, SettingsManager &settingsManager, SoundManager &soundManager, SpriteManager &spriteManager, Map &map, Player &player, bool &paused) {
	if (menu.size() == 0) {
		menu.push_back(std::vector<MenuBox>{
			MenuBox(smallFont, smallFontSize, "Yes", viewWidth / 2 - 25, viewHeight / 2, true, true, false, false, 0, 0, 15)
		});
		menu.push_back(std::vector<MenuBox>{
			MenuBox(smallFont, smallFontSize, "No" , viewWidth / 2 + 25, viewHeight / 2, true, true, false, false, 0, 0, 15)
		});
	}

	if (!paused && window.hasFocus()) {
		bool playerDead = false;
		bool frozen     = false;
		if (transition.ongoing || !player.alive) {
			frozen = true;
		}
		map.update(sf::FloatRect(player.x, player.y, player.width, player.height), playerDead, frozen, soundManager);
		if (playerDead) {
			player.death(map);
		}
	}
	map.draw(window, view, spriteManager.tilesetSprite, spriteManager.bulletSprite, spriteManager.bulletExplosionSprite, settingsManager.debugMode.active);

	if (!paused && window.hasFocus()) {
		player.update(map, transition.ongoing, soundManager);
	}
	player.draw(window, view, spriteManager.playerSprite, spriteManager.playerDeathSprite, spriteManager.offscreenCircleSprite, paused, settingsManager.debugMode.active);

	if (settingsManager.debugMode.active) {
		std::stringstream posX;
		std::stringstream posY;
		std::stringstream velX;
		std::stringstream velY;
		std::stringstream alive;
		std::stringstream onground;
		std::stringstream onice;
		std::stringstream jumped;

		posX     << "pos.x: "    << player.x;
		posY     << "pos.y: "    << player.y;
		velX     << "vel.x: "    << player.xVelocity;
		velY     << "vel.y: "    << player.yVelocity;
		alive    << "alive: "    << boolToString(player.alive);
		jumped   << "jumped: "   << boolToString(player.jumped);
		onground << "onground: " << boolToString(player.onGround);
		onice    << "onice: "    << boolToString(player.onIce);

		textBox(window, view, smallFont, smallFontSize,     posX.str(), 2, viewHeight - 65, false, false, false, true, 0, 0, 4, sf::Color::Green, sf::Color(0, 0, 0, 127), sf::Color::Transparent);
		textBox(window, view, smallFont, smallFontSize,     posY.str(), 2, viewHeight - 56, false, false, false, true, 0, 0, 4, sf::Color::Green, sf::Color(0, 0, 0, 127), sf::Color::Transparent);
		textBox(window, view, smallFont, smallFontSize,     velX.str(), 2, viewHeight - 47, false, false, false, true, 0, 0, 4, sf::Color::Green, sf::Color(0, 0, 0, 127), sf::Color::Transparent);
		textBox(window, view, smallFont, smallFontSize,     velY.str(), 2, viewHeight - 38, false, false, false, true, 0, 0, 4, sf::Color::Green, sf::Color(0, 0, 0, 127), sf::Color::Transparent);
		textBox(window, view, smallFont, smallFontSize,    alive.str(), 2, viewHeight - 29, false, false, false, true, 0, 0, 4, sf::Color::Green, sf::Color(0, 0, 0, 127), sf::Color::Transparent);
		textBox(window, view, smallFont, smallFontSize,   jumped.str(), 2, viewHeight - 20, false, false, false, true, 0, 0, 4, sf::Color::Green, sf::Color(0, 0, 0, 127), sf::Color::Transparent);
		textBox(window, view, smallFont, smallFontSize, onground.str(), 2, viewHeight - 11, false, false, false, true, 0, 0, 4, sf::Color::Green, sf::Color(0, 0, 0, 127), sf::Color::Transparent);
		textBox(window, view, smallFont, smallFontSize,    onice.str(), 2, viewHeight -  2, false, false, false, true, 0, 0, 4, sf::Color::Green, sf::Color(0, 0, 0, 127), sf::Color::Transparent);
	}

	if (map.cleared) {
		transition.to(STATE_levelClear);
	} else if (paused) {
		textBox(window, view, largeFont, largeFontSize, "", viewWidth / 2, viewHeight / 2, false, false, false, false, viewWidth, viewHeight, 0, textColor, sf::Color(0, 0, 0, 127), sf::Color::Transparent);
		textBox(window, view, largeFont, largeFontSize, "Go to level select?", viewWidth / 2, viewHeight / 4, true, true, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
		drawMenu(window, view, menu, menuSelection);
	} else {
		if (map.valid) {
			if (window.hasFocus()) {
				map.clearTime += 1.f / targetFPS;
			}
			std::stringstream clearTime;
			clearTime << std::floor(map.clearTime * 100) / 100;
			textBox(window, view, smallFont, smallFontSize, clearTime.str(), 10, 10, false, false, false, false, 0, 0, 0, textColor, sf::Color::Transparent, sf::Color::Transparent);
		} else {
			textBox(window, view, largeFont, largeFontSize, "This is a bug", viewWidth / 2, viewHeight / 2, true, true, false, false, 0, 0, 25, textColor, sf::Color::Red, sf::Color::Transparent);
		}

		if (window.hasFocus()) {
			updateView(view, player.x + player.width / 2, player.y + player.height / 2, map.size, false);
		}
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
					transition.to(STATE_levelSelect);
				} else if (menuBoxString == "No") {
					paused = false;
				}
			}
		}
	}
}

void exit(sf::RenderWindow &window) {
	window.close();
}
