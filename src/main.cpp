#include <SFML/Graphics.hpp>

void game(sf::RenderWindow& window, float x, float y) {
	sf::Texture texture;
	texture.loadFromFile("src/resources/textures/slimey.png");
	texture.setSmooth(true);
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(sf::Vector2f(x, y));

	//sf::CircleShape shape(100);
	//shape.setPosition(sf::Vector2f(x, y));

	window.draw(sprite);
}

int main() {
	sf::RenderWindow window(sf::VideoMode(800, 800), "Slimey: The Five Crystals", sf::Style::Close);

	float x = 400.0f;
	float y = 400.0f;

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear();

		// up
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
			y -= 0.05;
		}
		// down
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
			y += 0.05;
		}
		// left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
			x -= 0.05;
		}
		// right
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
			x += 0.05;
		}

		game(window, x, y);

		window.display();
	}

	return 0;
}
