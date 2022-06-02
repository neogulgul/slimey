#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH 	= 800;
const int WINDOW_HEIGHT = 800;

float horizontal_vel 	= 0.0;
float vertical_vel 		= 0.0;
const float max_vel 	= 0.5;
const float acc			= 0.000010;
const float dec			= 0.000001;
float x 			= 0.0;
float y 			= 0.0;
const int WIDTH 	= 14;
const int HEIGHT 	= 12;
bool up 	= false;
bool down 	= false;
bool left 	= false;
bool right 	= false;

float calc_velocity(float vel, bool neg_dir, bool pos_dir) {
	if (neg_dir && !pos_dir) {
		vel -= acc;
	} else if (pos_dir && !neg_dir) {
		vel += acc;
	} else {
		if (vel > 0) {
			vel -= dec;
			if (vel < 0) {
				vel = 0;
			}
		} else if (vel < 0) {
			vel += dec;
			if (vel > 0) {
				vel = 0;
			}
		}
	}

	if (vel > max_vel) {
		vel = max_vel;
	} else if (vel < -max_vel) {
		vel = -max_vel;
	}

	return vel;
}

void game(sf::RenderWindow& window) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		up = true;
	} else {
		up = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		down = true;
	} else {
		down = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		left = true;
	} else {
		left = false;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		right = true;
	} else {
		right = false;
	}

	horizontal_vel = calc_velocity(horizontal_vel, left, right);
	vertical_vel = calc_velocity(vertical_vel, up, down);

	x += horizontal_vel;
	y += vertical_vel;

	// pacman style edges
	/*if (x < 0) {
		x = WINDOW_WIDTH;
	} else if (x > WINDOW_WIDTH - WIDTH) {
		x = 0;
	}
	if (y < 0) {
		y = WINDOW_HEIGHT;
	} else if (y > WINDOW_HEIGHT - HEIGHT) {
		y = 0;
	}*/

	// bounce
	if (x < 0 || x > WINDOW_WIDTH - WIDTH) {
		horizontal_vel = -horizontal_vel;
	}
	if (y < 0 || y > WINDOW_HEIGHT - HEIGHT) {
		vertical_vel = -vertical_vel;
	}

	sf::Texture texture;
	texture.loadFromFile("src/resources/textures/slimey.png", sf::IntRect(0, 0, WIDTH, HEIGHT));
	texture.setSmooth(true);
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setPosition(sf::Vector2f(x, y));

	window.draw(sprite);
}

int main() {
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Slimey: The Five Crystals", sf::Style::Close);

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}
		window.clear();

		game(window);

		window.display();
	}

	return 0;
}
