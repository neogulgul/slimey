#include <SFML/Graphics.hpp>

#include "headers/Game.hpp"

int main()
{
	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Slimey", sf::Style::Default);
	sf::View view({viewWidth * 0.5, viewHeight * 0.5}, {viewWidth, viewHeight});
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
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				if (game.state == LevelEditor)
				{
					game.editor.handleZoom(event);
				}
				else if (game.state == CustomLevels)
				{
					// todo: this
				}
			}
			else if (event.type == sf::Event::TextEntered)
			{
				if (game.state == LevelEditor)
				{
					game.editor.handleTextEntered(event);
				}
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
