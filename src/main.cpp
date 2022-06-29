#include <vector>
#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/Player.hpp"
#include "headers/State.hpp"

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

	State state = startMenuState;
	State lastState = state;
	std::vector<std::vector<MenuBox>> menu;
	sf::Vector2f menuSelection;
	int menuSelectionTimer = 0;
	int menuSelectionTimerFrames = 6;
	bool entered = true;
	bool escaped = true;

	// loading textures
	sf::Texture playerTexture;
	sf::Texture playerDeathTexture;
	sf::Texture offscreenCircleTexture;
	sf::Sprite playerSprite;
	sf::Sprite playerDeathSprite;
	sf::Sprite offscreenCircleSprite;
	playerTexture.loadFromFile("resources/textures/slimey.png");
	playerDeathTexture.loadFromFile("resources/textures/death.png");
	offscreenCircleTexture.loadFromFile("resources/textures/offscreen-circle.png");
	playerSprite.setTexture(playerTexture);
	playerDeathSprite.setTexture(playerDeathTexture);
	offscreenCircleSprite.setTexture(offscreenCircleTexture);

	Player player;
	Map map;

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (window.hasFocus()) {
			window.clear(background);

			if (state != lastState) {
				lastState = state;
				menu.clear();
				menuSelection = sf::Vector2f(0, 0);
			}

			if (menuSelectionTimer > 0) {
				menuSelectionTimer++;
				if (menuSelectionTimer == menuSelectionTimerFrames) {
					menuSelectionTimer = 0;
				}
			}

			if (menu.size() > 0 && menuSelectionTimer == 0) {
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

				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
					entered = false;
				}
				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
					escaped = false;
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

			switch (state) {
				case startMenuState:
					startMenu(window, view, smallFont, largeFont, state, menu, menuSelection, entered, escaped);
					break;
				case levelSelectState:
					levelSelect(window, view, smallFont, largeFont, state, menu, menuSelection, entered, escaped, map, player);
					break;
				case gameState:
					game(window, view, smallFont, largeFont, state, menu, menuSelection, entered, escaped, map, player, playerSprite, playerDeathSprite, offscreenCircleSprite);
					break;
			}

			window.setView(view);
			window.display();
		}
	}

	return 0;
}
