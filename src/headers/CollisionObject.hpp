#pragma once

class CollisionObject {
	public:
		int width;
		int height;
		float x, y;

		sf::RectangleShape rectangle;

		CollisionObject(float x, float y, int width, int height);
		void draw(sf::RenderWindow &window);
};
