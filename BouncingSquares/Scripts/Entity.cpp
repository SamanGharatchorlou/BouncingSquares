#include "stdafx.h"
#include "Entity.h"
#include "Utility.h"


sf::Vector2f Entity::getPosition()
{
	return position;
}


// get the players position in the map blueprint array rather than in pixels
sf::Vector2f Entity::getTilePosition(float tileSize)
{
	return position / tileSize;
}


// set a new direction vector
void Entity::setDirection(sf::Vector2f newDirection)
{
	direction.x = newDirection.x;
	direction.y = newDirection.y;

	direction = normalise(direction);
}