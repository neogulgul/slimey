#pragma once

class Bullet {
	public:
		sf::Vector2f position;
		sf::Vector2f velocity;

		Bullet(sf::Vector2f position, sf::Vector2f velocity);
		void update();
		void draw(sf::RenderWindow &window, sf::View &view, sf::Sprite bulletSprite);
};
