#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

class Enemy
{
private:
	sf::RectangleShape rectShape;

	sf::Vector2f position;
	sf::Vector2f direction;

	sf::Vector2f previousPosition;

	float speed = 2.5f;

	int enemyIndex;

public:
	Enemy(sf::Vector2f startingPosition, sf::Vector2f size, int enemyIndex);

	sf::Vector2f getTilePosition(float tileSize);
	sf::Vector2f getPosition();

	sf::FloatRect getBoundary();
	sf::RectangleShape getShape();

	void changeColor(sf::Color newColour);

	int getIndex();

	void randomiseDirection();

	void setDirection(sf::Vector2f newDirection);

	void Update();

};
