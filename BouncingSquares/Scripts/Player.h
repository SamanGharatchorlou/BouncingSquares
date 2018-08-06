#pragma once
#include "Entity.h"

#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>

// player controlled entity
class Player
{
private:
	sf::CircleShape circleShape;
	
	sf::Vector2f position;
	sf::Vector2f previousPosition;
	sf::Vector2f hitPosition;

	sf::Vector2f velocity;
	sf::Vector2f direction = sf::Vector2f(0, 0);

	float speed = 3.5f;

	int lives;

	bool vunerability = true;

public:
	Player(sf::Vector2f startingPosition, float radius, float points);

	sf::CircleShape getShape();

	// get player position in pixels or tile (i.e. blueprint index)
	void setPosition(sf::Vector2f newPosition);
	sf::Vector2f getPosition();
	sf::Vector2f getTilePosition(float tileSize);

	// log and reset the previous frames player position
	void logPosition();
	void resetPosition();

	void logHitPosition();
	sf::Vector2f getHitPosition();

	void setDirection(sf::Vector2f newDirection);

	void changeColour(sf::Color newColour);

	bool getVunerability();
	void setVunerability(bool isVunerable);

	// get/set lives
	void setLives(int lives);
	int getLives();

	// player directions
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();

	void Update();
};
