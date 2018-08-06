#pragma once
#include <SFML\/Graphics.hpp>

// class to be inherited by any game object such as player, enemy, items etc.
class Entity
{
protected:
	sf::Vector2f position;

	sf::Vector2f direction;
	float speed;

public:
	// get position in pixels or tile (i.e. blueprint index)
	sf::Vector2f getPosition();
	sf::Vector2f getTilePosition(float tileSize);

	void setDirection(sf::Vector2f newDirection);

	// also include update function, which is to be overridden

};