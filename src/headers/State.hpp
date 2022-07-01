#pragma once

#include "Map.hpp"
#include "MenuBox.hpp"
#include "Player.hpp"
#include "Transition.hpp"

enum State { startMenuState, levelSelectState, gameState };

void startMenu  (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &entered, bool &escaped);
void levelSelect(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &entered, bool &escaped, Map &map, Player &player, Transition &transition);
void game       (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &entered, bool &escaped, Map &map, Player &player, Transition &transition, sf::Sprite &playerSprite, sf::Sprite &playerDeathSprite, sf::Sprite &offscreenCircleSprite, sf::Sprite &tilesetSprite, bool &paused);
