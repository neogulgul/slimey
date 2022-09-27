#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "headers/Game.hpp"

#include <cmath>
#include <sstream>

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Slimey", sf::Style::Default);
	sf::View view(sf::Vector2f(viewWidth * 0.5, viewHeight * 0.5), sf::Vector2f(viewWidth, viewHeight));
	sf::Event event;

	window.setFramerateLimit(FPS);

	sf::Music music;
	music.openFromFile("assets/audio/rain.ogg");
	music.setVolume(50);
	music.play();

	sf::Clock clock;
	std::stringstream FPS;

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

		FPS.str("");
		FPS << "FPS: " << std::floor(1 / clock.restart().asSeconds());

		window.setView(view);

		game.draw();

		game.text.draw(FPS.str(), Start, Start, {0, 0});

		window.display();
	}

	return 0;
}
