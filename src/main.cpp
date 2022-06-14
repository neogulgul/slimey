#include <string>
#include <SFML/Graphics.hpp>
#include "headers/global.hpp"
#include "headers/Map.hpp"
#include "headers/Player.hpp"

void game(sf::RenderWindow &window, sf::View view, Map map, Player &player) {
	map.draw(window, view);

	player.update(map);
	player.draw(window);
}

int main() {
	sf::RenderWindow window(sf::VideoMode(viewWidth * 2, viewHeight * 2), "Slimey", sf::Style::Default);
	window.setFramerateLimit(FPS);
	sf::Event event;
	sf::View view(sf::Vector2f(0, 0), sf::Vector2f(viewWidth, viewHeight));

	Map map("resources/textures/map.png");
	Player player(map.spawn.x, map.spawn.y);

	updateView(view, player.x, player.y, map.size, true);

	while (window.isOpen()) {
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (window.hasFocus()) {
			window.clear(background);

			game(window, view, map, player);

			updateView(view, player.x + player.width / 2, player.y + player.height / 2, map.size, false);
			window.setView(view);
			window.display();
		}
	}

	return 0;
}
