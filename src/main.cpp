#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

const std::string TITLE = "Slimey";
const int WINDOW_WIDTH 	= 16 * 16;
const int WINDOW_HEIGHT = 16 * 16;

const float GRAVITY = 0.5;

const float FPS = 60;

sf::Texture tileTexture;
sf::Sprite tileSprite;

sf::Image mapSketch;
sf::Color pixel;
sf::Color topPixel;

void drawMap(sf::RenderWindow& window) {
	for (int x = 0; x < 16; x++) {
		for (int y = 0; y < 16; y++) {
			pixel = mapSketch.getPixel(x, y);
			if (pixel.r == 255 && pixel.g == 0 && pixel.b == 0) {
				topPixel = mapSketch.getPixel(x, y - 1);
				if (topPixel.r != 255 || topPixel.g != 0 || topPixel.b != 0) {
					tileTexture.loadFromFile("src/resources/textures/grass.png");
				} else {
					tileTexture.loadFromFile("src/resources/textures/ground.png");
				}
				tileTexture.setSmooth(true);
				tileSprite.setTexture(tileTexture);
				tileSprite.setPosition(x * 16, y * 16);
				window.draw(tileSprite);
			}
		}
	}
}

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
		float max_vel = 4;
		float acceleration = 0.4;
		float deceleration = 0.2;

		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;

		float jumpforce = 6;
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

Player player(0, 0);

void game(sf::RenderWindow& window) {
	drawMap(window);
	player.draw(window);
}

int main() {
	sf::Clock clock;

	sf::Color background = sf::Color(77, 120, 204);

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), TITLE, sf::Style::Close);

	mapSketch.loadFromFile("src/resources/textures/map.png");

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
