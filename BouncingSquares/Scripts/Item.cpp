#include "stdafx.h"
#include "Item.h"
#include "Utility.h"

#include <iostream>

Item::Item(sf::Vector2f itemPosition, float radius, float points)
{
	position.x = itemPosition.x;
	position.y = itemPosition.y;

	circleShape.setPosition(position);

	circleShape.setRadius(radius);
	circleShape.setPointCount(points);

	circleShape.setFillColor(sf::Color::Green);
}


sf::CircleShape& Item::getShape()
{
	return circleShape;
}


sf::Vector2f Item::getPosition()
{
	return position;
}


// get the players position in the map blueprint array rather than in pixels
sf::Vector2f Item::getTilePosition(float tileSize)
{
	return position / tileSize;
}

void Item::setPosition(sf::Vector2f newPosition)
{
	position = newPosition;
	circleShape.setPosition(position);
}


bool Item::pickUpItem(sf::Vector2f playerPosition)
{
	// apply offset as center doesnt seem to be in the middle
	sf::Vector2f itemPosition = position + sf::Vector2f(5, 4);

	float distance = PointPointDistance(itemPosition, playerPosition);

	if (distance < 14)
	{
		collectItem();
		return true;
	}
	else
		return false;
}


void Item::collectItem()
{
	collected = true;
}

bool Item::isCollected()
{
	return collected;
}