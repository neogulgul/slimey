#pragma once

#include "Global.hpp"
#include "Map.hpp"
#include "MenuBox.hpp"
#include "Player.hpp"
#include "SettingsManager.hpp"
#include "SoundManager.hpp"
#include "SpriteManager.hpp"
#include "Transition.hpp"

void splashScreen(sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, bool &enter, sf::Clock beginningOfTime);
void startScreen (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape                                  , SoundManager &soundManager);
void options     (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, SettingsManager &settingsManager, SoundManager &soundManager);
void fileSelect  (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape                                  , SoundManager &soundManager);
void levelSelect (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape                                  , SoundManager &soundManager                              , Map &map, Player &player);
void levelPlay   (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape, SettingsManager &settingsManager, SoundManager &soundManager, SpriteManager &spriteManager, Map &map, Player &player, bool &paused);
void levelClear  (sf::RenderWindow &window, sf::View &view, sf::Font smallFont, sf::Font largeFont, State &state, Transition &transition, std::vector<std::vector<MenuBox>> &menu, sf::Vector2f &menuSelection, bool &enter, bool &escape                                  , SoundManager &soundManager                              , Map &map, Player &player);
void exit        (sf::RenderWindow &window);
