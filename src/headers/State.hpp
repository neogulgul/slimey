#pragma once

#include "Map.hpp"
#include "MenuBox.hpp"
#include "Player.hpp"

enum State { startMenuState, levelSelectState, gameState };

void startMenu  (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &entered, bool &escaped);
void levelSelect(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &entered, bool &escaped, Map &map, Player &player);
void game       (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &entered, bool &escaped, Map &map, Player &player, sf::Sprite &playerSprite, sf::Sprite &playerDeathSprite, sf::Sprite &offscreenCircleSprite, bool &paused);
