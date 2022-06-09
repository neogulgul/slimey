#include <iostream>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "headers/Player.hpp"
#include "headers/CollisionObject.hpp"

int FPS = 60;

int viewWidth = 256;
int viewHeight = 256;
std::string TITLE = "Slimey: The Five Crystals";

std::vector<CollisionObject> CollisionObjects;

Player player;
CollisionObject test(0, 200, 100, 56);

void setup() {
	CollisionObjects.push_back(test);
}

void game(sf::RenderWindow &window) {
	for (int i = 0; i < CollisionObjects.size(); i++) {
		CollisionObjects.at(i).draw(window);
	}

	player.draw(window);
}

int main() {
	sf::Color background = sf::Color(77, 120, 204);
	sf::RenderWindow window(sf::VideoMode(viewWidth * 2, viewHeight * 2), TITLE, sf::Style::Default);
	window.setFramerateLimit(FPS);
	sf::Event event;
	sf::View view(sf::Vector2f(viewWidth / 2, viewHeight / 2), sf::Vector2f(viewWidth, viewHeight));

	setup();

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(background);

		game(window);

		window.setView(view);
		window.display();
	}

	return 0;
}
