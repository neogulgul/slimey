#include <cmath>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/Player.hpp"
#include "headers/State.hpp"
#include "headers/Transition.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(viewWidth * windowScale, viewHeight * windowScale), "Slimey", sf::Style::Default);
	window.setFramerateLimit(FPS);
	sf::View view(sf::Vector2f(0, 0), sf::Vector2f(viewWidth, viewHeight));
	sf::Event event;

	sf::Font smallFont;
	sf::Font largeFont;
	smallFont.loadFromFile("resources/fonts/FiveByFive.ttf");
	largeFont.loadFromFile("resources/fonts/FiveByFive.ttf");
	// avoiding blurry font
	sf::Texture& smallFontTexture = const_cast<sf::Texture&>(smallFont.getTexture(smallFontSize));
	sf::Texture& largeFontTexture = const_cast<sf::Texture&>(largeFont.getTexture(largeFontSize));
	smallFontTexture.setSmooth(false);
	largeFontTexture.setSmooth(false);

	State state = startScreenState;
	State lastState = state;

	std::vector<std::vector<MenuBox>> menu;
	sf::Vector2f menuSelection;
	int menuSelectionTimer = 0;
	int menuSelectionTimerFrames = 6;

	bool enter   = false;
	bool entered = true;
	bool escape  = false;
	bool escaped = true;

	bool paused  = false;

	// loading textures
	sf::Texture playerTexture;
	sf::Texture playerDeathTexture;
	sf::Texture offscreenCircleTexture;
	sf::Texture tilesetTexture;
	sf::Sprite playerSprite;
	sf::Sprite playerDeathSprite;
	sf::Sprite offscreenCircleSprite;
	sf::Sprite tilesetSprite;
	playerTexture.loadFromFile("resources/textures/slimey.png");
	playerDeathTexture.loadFromFile("resources/textures/death.png");
	offscreenCircleTexture.loadFromFile("resources/textures/offscreen-circle.png");
	tilesetTexture.loadFromFile("resources/textures/tileset.png");
	playerSprite.setTexture(playerTexture);
	playerDeathSprite.setTexture(playerDeathTexture);
	offscreenCircleSprite.setTexture(offscreenCircleTexture);
	tilesetSprite.setTexture(tilesetTexture);

	Player player;
	Map map;

	Transition transition;

	sf::Clock fpsClock;
	float currentTime   = 0;
	float lastTime      = 0;
	float fpsCounter    = 0;
	int fpsUpdateTimer  = 0;
	int fpsUpdateFrames = 4;

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (window.hasFocus()) {
			window.clear(backgroundColor);

			enter  = false;
			escape = false;
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
				if (!entered) {
					entered = true;
					enter = true;
				}
			} else {
				entered = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
				if (!escaped) {
					escaped = true;
					escape = true;
				}
			} else {
				escaped = false;
			}

			if (state != lastState) {
				lastState = state;
				menu.clear();
				menuSelection = sf::Vector2f(0, 0);
				paused = false;
			}

			if (menuSelectionTimer > 0) {
				menuSelectionTimer++;
				if (menuSelectionTimer == menuSelectionTimerFrames) {
					menuSelectionTimer = 0;
				}
			}

			if (menu.size() > 0 && menuSelectionTimer == 0 && !transition.ongoing) {
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
					menuSelection.y--;
					menuSelectionTimer++;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
					menuSelection.y++;
					menuSelectionTimer++;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
					menuSelection.x--;
					menuSelectionTimer++;
				}
				else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
					menuSelection.x++;
					menuSelectionTimer++;
				}

				if (menuSelection.x < 0) {
					menuSelection.x = 0;
				} else if (menuSelection.x > menu.size() - 1) {
					menuSelection.x = menu.size() - 1;
				}
				if (menuSelection.y < 0) {
					menuSelection.y = 0;
				} else if (menuSelection.y > menu.at(menuSelection.x).size() - 1) {
					menuSelection.y = menu.at(menuSelection.x).size() - 1;
				}
			}

			if (transition.inwardComplete) {
				state = transition.destination;
			}

			switch (state) {
				case startScreenState:
					startScreen(window, view, smallFont, largeFont, state, menu, menuSelection, enter, escape);
					break;
				case levelSelectState:
					levelSelect(window, view, smallFont, largeFont, state, menu, menuSelection, enter, escape, transition, map, player);
					break;
				case levelClearState:
					levelClear (window, view, smallFont, largeFont, state, menu, menuSelection, enter, escape, transition, map, player);
					break;
				case gameState:
					game       (window, view, smallFont, largeFont, state, menu, menuSelection, enter, escape, transition, map, player, playerSprite, playerDeathSprite, offscreenCircleSprite, tilesetSprite, paused);
					break;
			}

			std::stringstream fpsString;
			fpsString << "FPS: " << fpsCounter;

			window.setView(view);
			textBox(window, view, smallFont, smallFontSize, fpsString.str(), viewWidth - 10, 10, false, true, true, false, 0, 0, 5, textColor, sf::Color::Transparent, sf::Color::Transparent);
			window.display();

			fpsUpdateTimer++;

			currentTime = fpsClock.restart().asSeconds();
			if (fpsUpdateTimer == fpsUpdateFrames) {
				fpsUpdateTimer = 0;
				fpsCounter = std::ceil(1 / currentTime);
			}
			lastTime = currentTime;
		}
	}

	return 0;
}
