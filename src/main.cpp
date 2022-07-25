#include <cmath>
#include <sstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/Player.hpp"
#include "headers/SettingsManager.hpp"
#include "headers/SoundManager.hpp"
#include "headers/SpriteManager.hpp"
#include "headers/State.hpp"
#include "headers/Transition.hpp"

int main() {
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Slimey", sf::Style::Default);
	window.setFramerateLimit(targetFPS);
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

	State state = STATE_splashScreen;
	State lastState = state;

	std::vector<std::vector<MenuBox>> menu;
	sf::Vector2f menuSelection;
	int menuSelectionTimer       = 0;
	int menuSelectionTimerFrames = 6;

	bool enter   = false;
	bool entered = true;
	bool escape  = false;
	bool escaped = true;

	bool paused  = false;

	SettingsManager settingsManager;
	SoundManager soundManager;
	SpriteManager spriteManager;

	Player player;
	Map map;

	Transition transition(Transition::Type::fade);

	sf::Clock beginningOfTime;

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

		window.clear(backgroundColor);

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

		enter  = false;
		escape = false;

		if (window.hasFocus()) {
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
		}

		if (transition.inwardComplete) {
			state = transition.destination;
		}

		switch (state) {
			case STATE_splashScreen:
				splashScreen(window, view, smallFont, largeFont, state, transition, enter, beginningOfTime);
				break;
			case STATE_startScreen:
				startScreen (window, view, smallFont, largeFont, state, transition, menu, menuSelection, enter, escape                 , soundManager);
				break;
			case STATE_optionsMenu:
				optionsMenu (window, view, smallFont, largeFont, state, transition, menu, menuSelection, enter, escape, settingsManager, soundManager);
				break;
			case STATE_levelSelect:
				levelSelect (window, view, smallFont, largeFont, state, transition, menu, menuSelection, enter, escape                 , soundManager               , map, player);
				break;
			case STATE_levelClear:
				levelClear  (window, view, smallFont, largeFont, state, transition, menu, menuSelection, enter, escape                 , soundManager               , map, player);
				break;
			case STATE_game:
				game        (window, view, smallFont, largeFont, state, transition, menu, menuSelection, enter, escape, settingsManager, soundManager, spriteManager, map, player, paused);
				break;
			case STATE_exit:
				exit        (window);
				break;
		}
		window.setView(view);

		if (settingsManager.music.active != soundManager.playMusic) {
			if (settingsManager.music.active) {
				soundManager.playMusic = true;
			} else {
				soundManager.playMusic = false;
			}
		}
		if (settingsManager.SFX.active != soundManager.playSFX) {
			if (settingsManager.SFX.active) {
				soundManager.playSFX = true;
			} else {
				soundManager.playSFX = false;
			}
		}

		if (state != STATE_splashScreen) {
			if (soundManager.playMusic && soundManager.music.getStatus() != sf::SoundSource::Playing) {
				soundManager.music.play();
			} else if (!soundManager.playMusic && soundManager.music.getStatus() != sf::SoundSource::Paused) {
				soundManager.music.pause();
			}
		}

		if (settingsManager.FPS.active) { // show FPS
			std::stringstream fpsString;
			fpsString << "FPS: " << fpsCounter;
			textBox(window, view, smallFont, smallFontSize, fpsString.str(), viewWidth - 10, 10, false, true, true, false, 0, 0, 5, textColor, sf::Color::Transparent, sf::Color::Transparent);
		}

		if (settingsManager.crosshair.active) { // show crosshair
			static const sf::Color color(255, 0, 0, 127);
			static const int length    = 16;
			static const int thickness = 2;
			sf::RectangleShape hor(sf::Vector2f(length, thickness));
			sf::RectangleShape ver(sf::Vector2f(thickness, length));
			hor.setFillColor(color);
			ver.setFillColor(color);
			hor.setPosition(view.getCenter().x - length / 2, view.getCenter().y - thickness / 2);
			ver.setPosition(view.getCenter().x - thickness / 2, view.getCenter().y - length / 2);
			window.draw(hor);
			window.draw(ver);
		}

		window.display();

		fpsUpdateTimer++;
		currentTime = fpsClock.restart().asSeconds();
		if (fpsUpdateTimer == fpsUpdateFrames) {
			fpsUpdateTimer = 0;
			fpsCounter = std::ceil(1 / currentTime);
		}
		lastTime = currentTime;
	}

	return 0;
}
