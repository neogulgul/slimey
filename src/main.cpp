#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

const std::string TITLE = "Slimey";
const int WINDOW_WIDTH 	= 800;
const int WINDOW_HEIGHT = 800;

const float GRAVITY = 0.5;

const float FPS = 60;

bool frame(sf::Clock& clock) {
	if (clock.getElapsedTime().asSeconds() > 1 / FPS) {
		clock.restart();
		return true;
	}

	return false;
}

class Player {
	public:
		int width = 14;
		int height = 12;
		float x, y;
		float x_vel = 0;
		float y_vel = 0;
		float max_vel = 5;
		float acceleration = 0.6;
		float deceleration = 0.2;

		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;

		float jumpforce = 9;
		bool jump = false;
		bool jumped = false;
		bool onground = false;

		sf::Texture texture;
		sf::Sprite sprite;

		Player(float x, float y) {
			this->x = x;
			this->y = y;
			this->texture.loadFromFile("src/resources/textures/slimey.png", sf::IntRect(0, 0, this->width, this->height));
			this->texture.setSmooth(true);
			this->sprite.setTexture(this->texture);
		}

		void update() {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				this->up = true;
			} else {
				this->up = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				this->down = true;
			} else {
				this->down = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				this->left = true;
			} else {
				this->left = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
				this->right = true;
			} else {
				this->right = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
				this->jump = true;
			} else {
				this->jump = false;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
				this->x = 0;
				this->y = 0;
				this->y_vel = 0;
			}

			if (this->left && !this->right) {
				this->x_vel -= this->acceleration;
			} else if (this->right && !this->left) {
				this->x_vel += this->acceleration;
			} else if (this->onground) {
				if (this->x_vel > 0) {
					this->x_vel -= this->deceleration;
					if (this->x_vel < 0) {
						this->x_vel = 0;
					}
				} else if (this->x_vel < 0) {
					this->x_vel += this->deceleration;
					if (this->x_vel > 0) {
						this->x_vel = 0;
					}
				}
			}

			if (this->x_vel > this->max_vel) {
				this->x_vel = this->max_vel;
			} else if (this->x_vel < -this->max_vel) {
				this->x_vel = -this->max_vel;
			}

			if (this->onground && !this->jump) {
				this->jumped = false;
			}

			if (this->onground && this->jump && !this->jumped) {
				this->y_vel -= this->jumpforce;
				this->jumped = true;
				this->onground = false;
			}

			this->y_vel += GRAVITY;

			this->x += this->x_vel;
			this->y += this->y_vel;

			if (this->y > WINDOW_HEIGHT - this->height) {
				this->y = WINDOW_HEIGHT - this->height;
				this->y_vel = 0;
				this->onground = true;
			}
		}

		void draw(sf::RenderWindow& window) {
			this->update();
			this->sprite.setPosition(sf::Vector2f(this->x, this->y));
			window.draw(this->sprite);
		}
};

class Tile {
	public:
		Tile(int x, int y) {
		}

		void draw(sf::RenderWindow& window) {}
};

Player player(0, 0);

void game(sf::RenderWindow& window) {
	player.draw(window);
}

int main() {
	sf::Clock clock;

	sf::Color background = sf::Color(77, 120, 204);

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE, sf::Style::Close);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (frame(clock)) {
			window.clear(background);

			game(window);

			window.display();
		}
	}

	return 0;
}
