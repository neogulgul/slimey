#include <iostream>
#include <SFML/Graphics.hpp>

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
		float x = 0;
		float y = 0;
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

		//sf::Texture texture;
		//sf::Sprite sprite;
		// texture.loadFromFile("src/resources/textures/slimey.png", sf::IntRect(0, 0, this->width, this->height));
		// texture.setSmooth(true);
		// sprite.setTexture(texture);

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
			sf::Texture texture;
			sf::Sprite sprite;
			texture.loadFromFile("src/resources/textures/slimey.png", sf::IntRect(0, 0, this->width, this->height));
			texture.setSmooth(true);
			sprite.setTexture(texture);
			//sprite.setScale(10, 10);
			sprite.setPosition(sf::Vector2f(this->x, this->y));
			window.draw(sprite);
		}
};

Player player;

void game(sf::RenderWindow& window) {
	player.draw(window);
}

int main() {
	sf::Clock clock;

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Slimey: The Five Crystals", sf::Style::Close);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		if (frame(clock)) {
			window.clear();

			game(window);

			window.display();
		}
	}

	return 0;
}
