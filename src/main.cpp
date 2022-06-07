#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>

const std::string Title = "Slimey";
const int TileSize 		= 16;
const int ViewWidth 	= TileSize * 16;
const int ViewHeight 	= TileSize * 16;

const float Gravity = 0.3;

const int FPS = 60;

std::vector<std::vector<std::string>> mapCollision;

sf::Texture tileTexture;
sf::Sprite tileSprite;

sf::Image tileset;
sf::Vector2f tilePos;

sf::Image mapSketch;
sf::Vector2u mapSize;

// tiles
sf::Color ground(255, 0, 0);

sf::Color pixel;
sf::Color TL;
sf::Color TM;
sf::Color TR;

sf::Color ML;
sf::Color MM;
sf::Color MR;

sf::Color BL;
sf::Color BM;
sf::Color BR;

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

		// std::vector<std::vector<sf::Vector2i>> animations = {
		// 	{, },	// idle
		// 	{, },	// walk left
		// 	{, }	// walk right
		// };
		// int animation = 0;
		// int animationFrame = 0;
		// int animationFrameCap;

		sf::Texture texture;
		sf::Sprite sprite;

		Player(float x, float y) {
			this->x = x;
			this->y = y;
			//this->animationFrameCap = this->animations.at(this->animation).size();
			this->texture.loadFromFile("resources/textures/slimey.png", sf::IntRect(0, 0, this->width, this->height));
			this->sprite.setTexture(this->texture);
		}

		sf::Vector2f getPosition() {
			return sf::Vector2f(this->x + this->xVelocity + this->width / 2, this->y + this->yVelocity + this->height / 2);
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
						if (this->x > x * TileSize - this->width &&
							this->x < x * TileSize + TileSize &&
							this->y + this->yVelocity > y * TileSize &&
							this->y + this->yVelocity < y * TileSize + TileSize) {
							this->y = y * TileSize + TileSize;
							this->yVelocity = 0;
						}
						// bottom
						if (this->x > x * TileSize - this->width &&
							this->x < x * TileSize + TileSize &&
							this->y + this->yVelocity > y * TileSize - this->height &&
							this->y + this->yVelocity < y * TileSize) {
							this->y = y * TileSize - this->height;
							this->yVelocity = 0;
							this->grounded();
						}
						// left
						if (this->y > y * TileSize - this->height &&
							this->y < y * TileSize + TileSize &&
							this->x + this->xVelocity > x * TileSize &&
							this->x + this->xVelocity < x * TileSize + TileSize ) {
							this->x = x * TileSize + TileSize;
							this->xVelocity = 0;
						}
						// right
						else if (this->y > y * TileSize - this->height &&
							this->y < y * TileSize + TileSize &&
							this->x + this->xVelocity > x * TileSize - this->width &&
							this->x + this->xVelocity < x * TileSize) {
							this->x = x * TileSize - this->width;
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

			if ((this->x + this->width + this->xVelocity) / 16 >= 0 && (this->x + this->xVelocity) / 16 <= mapSize.x && (this->y + this->height + this->yVelocity) / 16 >= 0 && (this->y + this->yVelocity) / 16 <= mapSize.y) {
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

void populateMapCollision() {
	for (int x = 0; x < mapSize.x; x++) {
		std::vector<std::string> column;
		for (int y = 0; y < mapSize.y; y++) {
			pixel = mapSketch.getPixel(x, y);
			if (pixel == ground) {
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
		if (x > std::floor(player.getPosition().x / 16) - 16 && x < std::floor(player.getPosition().x / 16) + 16) {
			for (int y = 0; y < mapSize.y; y++) {
				if (y > std::floor(player.getPosition().y / 16) - 16 && y < std::floor(player.getPosition().y / 16) + 16) {
					pixel = mapSketch.getPixel(x, y);
					if (pixel == ground) {
						// TL = mapSketch.getPixel(x - 1, y - 1);
						TM = mapSketch.getPixel(x + 0, y - 1);
						// TR = mapSketch.getPixel(x + 1, y - 1);

						// ML = mapSketch.getPixel(x - 1, y + 0);
						// MM = mapSketch.getPixel(x + 0, y + 0);
						// MR = mapSketch.getPixel(x + 1, y + 0);

						// BL = mapSketch.getPixel(x - 1, y + 1);
						// BM = mapSketch.getPixel(x + 0, y + 1);
						// BR = mapSketch.getPixel(x + 1, y + 1);

						if (TM != ground) {
							tilePos.x = 0;
							tilePos.y = 0;
						} else {
							tilePos.x = 0;
							tilePos.y = 1;
						}

						tileTexture.loadFromImage(tileset, sf::IntRect(TileSize * tilePos.x, TileSize * tilePos.y, TileSize, TileSize));
						tileSprite.setTexture(tileTexture);
						tileSprite.setPosition(x * 16, y * 16);
						window.draw(tileSprite);
					}
				}
			}
		}
	}
}

void game(sf::RenderWindow& window) {
	player.draw(window);
	drawMap(window);
}

int main() {
	sf::Color background = sf::Color(77, 120, 204);

	tileset.loadFromFile("resources/textures/ground.png");

	sf::RenderWindow window(sf::VideoMode(ViewWidth * 4, ViewHeight * 4), Title, sf::Style::Default);
	window.setFramerateLimit(FPS);

	sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(ViewWidth, ViewHeight));

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

		window.clear(background);

		game(window);

		view.setCenter(player.getPosition());
		// horizontal
		if (view.getCenter().x < ViewWidth / 2) {
			view.setCenter(ViewWidth / 2, view.getCenter().y);
		} else if (view.getCenter().x > mapSize.x * 14) {
			view.setCenter(mapSize.x * 14, view.getCenter().y);
		}
		// vertical
		if (view.getCenter().y < ViewHeight / 2) {
			view.setCenter(view.getCenter().x, ViewHeight / 2);
		} else if (view.getCenter().y > mapSize.y * 12) {
			view.setCenter(view.getCenter().x, mapSize.y * 12);
		}
		//std::cout << "viex-x: " << view.getCenter().x << " | " << "view-y: " << view.getCenter().y << "\n";

		window.setView(view);
		window.display();
	}

	return 0;
}
