#pragma once

class Bullet {
	public:
		sf::Vector2f position;
		sf::Vector2f velocity;

		int index;

		int explosionTimer          = 0;
		int explosionFrames         = 4;
		int explosionFramesDuration = 4;
		bool exploding = false;
		bool destroyed = false;

		Bullet(sf::Vector2f position, sf::Vector2f velocity, int index);
		void destroy(std::vector<Bullet> &bulletVector);
		bool checkCollision(sf::Image &mapImage, sf::Vector2u &mapSize, sf::FloatRect playerRect, bool &playerDead);
		void update(sf::Image &mapImage, sf::Vector2u &mapSize, sf::FloatRect playerRect, bool &playerDead);
		void draw(sf::RenderWindow &window, sf::View &view, sf::Sprite &bulletSprite, sf::Sprite &bulletExplosionSprite);
};
