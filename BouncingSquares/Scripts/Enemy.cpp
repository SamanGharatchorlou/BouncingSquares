#include "stdafx.h"
#include "Enemy.h"
#include "Utility.h"

#include <iostream>


// enemy constructor
Enemy::Enemy(sf::Vector2f startingPosition, sf::Vector2f size, int index)
{
	position = startingPosition;
	randomiseDirection();

	rectShape.setPosition(position);
	rectShape.setSize(size);

	rectShape.setFillColor(sf::Color::Red);

	// effectively the enemy ID
	enemyIndex = index;
}


// get enemy by index
int Enemy::getIndex()
{
	return enemyIndex;
}


sf::Vector2f Enemy::getPosition()
{
	return position;
}


// get the players position in the map blueprint array rather than in pixels
sf::Vector2f Enemy::getTilePosition(float tileSize)
{
	return position / tileSize;
}


// return boundary as a FloatRect (required for Intersect function)
sf::FloatRect Enemy::getBoundary()
{
	return rectShape.getGlobalBounds();
}


sf::RectangleShape Enemy::getShape()
{
	return rectShape;
}


// change enemy colour
void Enemy::changeColor(sf::Color newColour)
{
	rectShape.setFillColor(newColour);
}


//  spawn enemy with a random direction vector
void Enemy::randomiseDirection()
{
	// x = -1 or 0 or +1
	direction.x = (rand() % 3) - 1;

	// prevent (0,0) situations creating a stationary enemy
	if (direction.x == 0) 
	{
		// y = -1 or +1
		direction.y = (rand() % 2) * 2 - 1;
	}
	else
	{
		direction.y = (rand() % 3) - 1;
	}

	direction = normalise(direction);
}


// set a new direction vector
void Enemy::setDirection(sf::Vector2f newDirection) 
{
	direction.x = newDirection.x;
	direction.y = newDirection.y;

	direction = normalise(direction);
}


// apply position update
void Enemy::Update()
{
	position += direction * speed;
	rectShape.setPosition(position);
}