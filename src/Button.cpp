#include "headers/Button.hpp"

#define activeScale 1.2
#define borderSize 2

Button::Button() {}

Button::Button(std::string _string, sf::Vector2f size, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f _position, bool _relativeToView)
{
	string   = _string;
	position = align(size, horizontalAlignment, verticalAlignment, {_position.x + borderSize, _position.y + borderSize});
	relativeToView = _relativeToView;
	shape.setOutlineThickness(borderSize);
	shape.setOutlineColor(sf::Color::Transparent);
	shape.setSize({size.x - borderSize * 2, size.y - borderSize * 2});
}

Button::Button(sf::Sprite* _sprite, Alignment horizontalAlignment, Alignment verticalAlignment, sf::Vector2f _position, bool _relativeToView)
{
	isSprite = true;
	setSprite(_sprite);
	position = align((sf::Vector2f)sprite->getTexture()->getSize(), horizontalAlignment, verticalAlignment, _position);
	relativeToView = _relativeToView;
}

void Button::setSprite(sf::Sprite* _sprite)
{
	sprite = _sprite;
}

void Button::update(sf::Vector2f mousePosition)
{
	active = false;
	if (bounds.contains(mousePosition))
	{
		active = true;
	}
}

void Button::draw(sf::RenderWindow* window, sf::View* view, Text* text)
{
	if (isSprite)
	{
		sprite->setOrigin(sprite->getTexture()->getSize().x / 2, sprite->getTexture()->getSize().y / 2);
		if (relativeToView)
		{
			sprite->setPosition(relativeViewPosition(view, position + sprite->getOrigin()));
		}
		else
		{
			sprite->setPosition(position + sprite->getOrigin());
		}

		bounds = sprite->getGlobalBounds();

		if (active)
		{
			sprite->setScale(0.8, 0.8);
			sprite->setColor(sf::Color(255, 255, 255, 191));
		}
		else
		{
			sprite->setColor(sf::Color::White);
		}

		window->draw(*sprite);

		// resetting sprite in case it is used somewhere else
		sprite->setOrigin(0, 0);
		sprite->setScale(1, 1);
	}
	else
	{
		if (relativeToView)
		{
			shape.setPosition(relativeViewPosition(view, position));
		}
		else
		{
			shape.setPosition(position);
		}
		bounds = shape.getGlobalBounds();

		sf::Vector2f shapeCenter;
		shapeCenter.x = bounds.left + bounds.width  / 2;
		shapeCenter.y = bounds.top  + bounds.height / 2;

		if (locked)
		{
			shape .setFillColor(lockedButtonBackground);
			border.setFillColor(lockedButtonForeground);
		}
		else
		{
			if (active)
			{
				shape .setFillColor(activeButtonBackground);
				border.setFillColor(activeButtonForeground);
			}
			else
			{
				shape .setFillColor(inactiveButtonBackground);
				border.setFillColor(inactiveButtonForeground);
			}
		}

		window->draw(shape);

		if (locked)
		{
			text->draw(string, Center, Center, shapeCenter, lockedButtonForeground);
		}
		else
		{
			if (active)
			{
				text->draw(string, Center, Center, shapeCenter, activeButtonForeground, {activeScale, activeScale});
			}
			else
			{
				text->draw(string, Center, Center, shapeCenter, inactiveButtonForeground);
			}
		}

		// drawing border sides
		// top
		border.setSize({bounds.width - borderSize * 2, borderSize});
		border.setPosition(bounds.left + borderSize, bounds.top);
		window->draw(border);
		// bottom
		border.setSize({bounds.width - borderSize * 2, borderSize});
		border.setPosition(bounds.left + borderSize, bounds.top + bounds.height - borderSize);
		window->draw(border);
		// left
		border.setSize({borderSize, bounds.height - borderSize * 2});
		border.setPosition(bounds.left, bounds.top + borderSize);
		window->draw(border);
		// right
		border.setSize({borderSize, bounds.height - borderSize * 2});
		border.setPosition(bounds.left + bounds.width - borderSize, bounds.top + borderSize);
		window->draw(border);

		// drawing border corners
		border.setSize({borderSize, borderSize});
		// top left
		border.setPosition(bounds.left + borderSize                      - borderSize / 2.f, bounds.top + borderSize                       - borderSize / 2.f);
		window->draw(border);
		// top right
		border.setPosition(bounds.left + bounds.width - borderSize * 2.f + borderSize / 2.f, bounds.top + borderSize                       - borderSize / 2.f);
		window->draw(border);
		// bottom left
		border.setPosition(bounds.left + borderSize                      - borderSize / 2.f, bounds.top + bounds.height - borderSize * 2.f + borderSize / 2.f);
		window->draw(border);
		// bottom right
		border.setPosition(bounds.left + bounds.width - borderSize * 2.f + borderSize / 2.f, bounds.top + bounds.height - borderSize * 2.f + borderSize / 2.f);
		window->draw(border);
	}
}
