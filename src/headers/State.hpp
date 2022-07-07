#pragma once

#include "global.hpp"
#include "Map.hpp"
#include "MenuBox.hpp"
#include "Player.hpp"
#include "Transition.hpp"

void startScreen(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape);
void levelSelect(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, Transition &transition, Map &map, Player &player);
void levelClear (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, Transition &transition, Map &map, Player &player);
void game       (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, Transition &transition, Map &map, Player &player, sf::Sprite &playerSprite, sf::Sprite &playerDeathSprite, sf::Sprite &offscreenCircleSprite, sf::Sprite &tilesetSprite, sf::Sprite &bulletSprite, sf::Sprite &bulletExplosionSprite, bool &paused);
