#include <SFML/Graphics.hpp>

#include "headers/Game.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Slimey", sf::Style::Default);
	sf::View view(sf::Vector2f(viewWidth * 0.5, viewHeight * 0.5), sf::Vector2f(viewWidth, viewHeight));
	sf::Event event;

	window.setFramerateLimit(targetFPS);

	Game game(&window, &view);

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		window.clear(background);

		game.update();

		window.setView(view);

		game.draw();

		window.display();
	}

	return 0;
}