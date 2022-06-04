#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

const std::string Title = "Slimey";
const int WindowWidth 	= 16 * 16;
const int WindowHeight = 16 * 16;

const float Gravity = 0.3;

const float FPS = 60;

std::vector<std::vector<std::string>> mapCollision;

sf::Texture tileTexture;
sf::Sprite tileSprite;

sf::Image mapSketch;
sf::Vector2u mapSize;

sf::Color pixel;
sf::Color topPixel;

void populateMapCollision() {
	for (int x = 0; x < mapSize.x; x++) {
		std::vector<std::string> column;
		for (int y = 0; y < mapSize.y; y++) {
			pixel = mapSketch.getPixel(x, y);
			if (pixel.r == 255 && pixel.g == 0 && pixel.b == 0) {
				column.push_back("x");
			} else {
				column.push_back(" ");
			}
		}
		mapCollision.push_back(column);
	}
}

void drawMap(sf::RenderWindow& window) {
	for (int x = 0; x < mapSize.x; x++) {
		for (int y = 0; y < mapSize.y; y++) {
			pixel = mapSketch.getPixel(x, y);
			if (pixel.r == 255 && pixel.g == 0 && pixel.b == 0) {
				topPixel = mapSketch.getPixel(x, y - 1);
				if (topPixel.r != 255 || topPixel.g != 0 || topPixel.b != 0) {
					tileTexture.loadFromFile("resources/textures/grass.png");
				} else {
					tileTexture.loadFromFile("resources/textures/ground.png");
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
		float xVelocity = 0;
		float yVelocity = 0;
		float maxVelocity = 2;
		float acceleration = 0.2;
		float deceleration = 0.3;

		bool up = false;
		bool down = false;
		bool left = false;
		bool right = false;

		int currentJumpFrame = 0;
		int jumpFrames = 8;
		float jumpForce = 0.5;
		bool jump = false;
		bool jumped = false;
		bool jumping = false;
		bool onground = false;

		sf::Texture texture;
		sf::Sprite sprite;

		// cells used for collision
		int cell_0_x, cell_0_y;
		std::string cell_0;

		int cell_1_x, cell_1_y;
		std::string cell_1;

		int cell_2_x, cell_2_y;
		std::string cell_2;

		int cell_3_x, cell_3_y;
		std::string cell_3;

		Player(float x, float y) {
			this->x = x;
			this->y = y;
			this->texture.loadFromFile("resources/textures/slimey.png", sf::IntRect(0, 0, this->width, this->height));
			this->texture.setSmooth(true);
			this->sprite.setTexture(this->texture);
		}

		void grounded() {
			this->onground = true;
			this->jumping = false;
			this->currentJumpFrame = 0;
		}

		void collision() {
			this->onground = false;

			for (int x = 0; x < mapSize.x; x++) {
				for (int y = 0; y < mapSize.y; y++) {
					if (mapCollision.at(x).at(y) == "x") {
						// top
						if (this->x > x * 16 - this->width &&
							this->x < x * 16 + 16 &&
							this->y + this->yVelocity > y * 16 &&
							this->y + this->yVelocity < y * 16 + 16) {
							this->y = y * 16 + 16;
							this->yVelocity = 0;
						}
						// bottom
						if (this->x > x * 16 - this->width &&
							this->x < x * 16 + 16 &&
							this->y + this->yVelocity > y * 16 - this->height &&
							this->y + this->yVelocity < y * 16) {
							this->y = y * 16 - this->height;
							this->yVelocity = 0;
							this->grounded();
						}
						// left
						if (this->y > y * 16 - this->height &&
							this->y < y * 16 + 16 &&
							this->x + this->xVelocity > x * 16 &&
							this->x + this->xVelocity < x * 16 + 16 ) {
							this->x = x * 16 + 16;
							this->xVelocity = 0;
						}
						// right
						else if (this->y > y * 16 - this->height &&
							this->y < y * 16 + 16 &&
							this->x + this->xVelocity > x * 16 - this->width &&
							this->x + this->xVelocity < x * 16) {
							this->x = x * 16 - this->width;
							this->xVelocity = 0;
						}
					}
				}
			}
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
				this->xVelocity = 0;
				this->yVelocity = 0;
				this->jumping = false;
			}

			if (this->left && !this->right) {
				this->xVelocity -= this->acceleration;
			} else if (this->right && !this->left) {
				this->xVelocity += this->acceleration;
			} else if (this->onground) {
				if (this->xVelocity > 0) {
					this->xVelocity -= this->deceleration;
					if (this->xVelocity < 0) {
						this->xVelocity = 0;
					}
				} else if (this->xVelocity < 0) {
					this->xVelocity += this->deceleration;
					if (this->xVelocity > 0) {
						this->xVelocity = 0;
					}
				}
			}

			if (this->xVelocity > this->maxVelocity) {
				this->xVelocity = this->maxVelocity;
			} else if (this->xVelocity < -this->maxVelocity) {
				this->xVelocity = -this->maxVelocity;
			}

			if (this->onground && !this->jump) {
				this->jumped = false;
			}

			if (this->onground && this->jump && !this->jumped) {
				this->jumped = true;
				this->jumping = true;
				this->onground = false;
			}

			if (this->jumping) {
				this->yVelocity -= this->jumpForce;
				this->currentJumpFrame++;
				if (this->currentJumpFrame == this->jumpFrames) {
					this->jumping = false;
					this->currentJumpFrame = 0;
				}
			}

			if (!this->onground && !this->jumping) {
				this->yVelocity += Gravity;
			}

			if ((this->x + this->xVelocity) / 16 >= 0 && (this->x + this->xVelocity) / 16 <= mapSize.x && (this->y + this->yVelocity) / 16 >= 0 && (this->y + this->yVelocity) / 16 <= mapSize.y) {
				this->collision();
			}

			this->x += this->xVelocity;
			this->y += this->yVelocity;
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

	sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), Title, sf::Style::Close);

	mapSketch.loadFromFile("resources/textures/map.png");
	mapSize = mapSketch.getSize();
	populateMapCollision();

	// prints map collision
	// for (int y = 0; y < mapSize.y; y++) {
	// 	for (int x = 0; x < mapSize.x; x++) {
	// 		std::cout << "[" << mapCollision.at(x).at(y) << "]";
	// 		if (x == 15) {
	// 			std::cout << "\n";
	// 		}
	// 	}
	// }

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
