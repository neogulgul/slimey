#include <string>
#include <SFML/Graphics.hpp>
#include "headers/constants.hpp"
#include "headers/functions.hpp"
#include "headers/Map.hpp"
#include "headers/Player.hpp"

std::string TITLE = "Slimey: The Five Crystals";

void game(sf::RenderWindow &window, sf::View view, Map map, Player &player) {
	map.draw(window, view);

	player.update(map);
	player.draw(window);
}

int main() {
	sf::Color background = sf::Color(77, 120, 204);
	sf::RenderWindow window(sf::VideoMode(viewWidth * 2, viewHeight * 2), TITLE, sf::Style::Default);
	window.setFramerateLimit(FPS);
	sf::Event event;
	sf::View view(sf::Vector2f(viewWidth / 2, viewHeight / 2), sf::Vector2f(viewWidth, viewHeight));

	Map map("resources/textures/map.png");
	Player player(0, map.size.y - 3);

	adjustView(view, player.x, player.y, map.size, true);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		window.clear(background);

		game(window, view, map, player);

		adjustView(view, player.x, player.y, map.size, false);
		window.setView(view);
		window.display();
	}

	return 0;
}
