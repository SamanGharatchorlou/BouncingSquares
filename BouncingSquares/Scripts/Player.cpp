#include "stdafx.h"
#include "Player.h"
#include "Utility.h"


// player constructor
Player::Player(sf::Vector2f startingPosition, float radius, float points)
{
	position.x = startingPosition.x;
	position.y = startingPosition.y;

	circleShape.setRadius(radius);
	circleShape.setPointCount(points);

	circleShape.setFillColor(sf::Color::Green);
}


sf::CircleShape Player::getShape()
{
	return circleShape;
}


// change player colour
void Player::changeColour(sf::Color newColour)
{
	circleShape.setFillColor(newColour);
}


sf::Vector2f Player::getPosition() 
{
	return position;
}

void Player::setPosition(sf::Vector2f newPosition)
{
	position = newPosition;
}


// get the players position in the map blueprint array rather than in pixels
sf::Vector2f Player::getTilePosition(float tileSize)
{
	return position / tileSize;
}


// set a new direction vector
void Player::setDirection(sf::Vector2f newDirection)
{
	direction.x = newDirection.x;
	direction.y = newDirection.y;

	direction = normalise(direction);
}


void Player::setLives(int setLives)
{
	lives = setLives;
}

int Player::getLives()
{
	return lives;
}

// 4 possible directions only adjust direction vector not velocity
void Player::moveRight()
{

	direction.x = 1.0f;
}

void Player::moveLeft()
{
	direction.x = -1.0f;
}

void Player::moveUp()
{
	direction.y = -1.0f;
}

void Player::moveDown()
{
	direction.y = 1.0f;
}


// log the previous frames position in case of colision
void Player::logPosition()
{
	previousPosition = position;
}


// if collision with wall reset player position to previous location, prevent moving through walls
void Player::resetPosition()
{
	position = previousPosition;
}

void Player::logHitPosition()
{
	hitPosition = position;
}

sf::Vector2f Player::getHitPosition()
{
	return hitPosition;
}


// applies any changes in position each frame
void Player::Update()
{
	velocity = speed * normalise(direction);

	position += velocity;
	circleShape.setPosition(position);

	// reset direction vector, prevent player flying off
	direction = sf::Vector2f(0, 0);
}


bool Player::getVunerability()
{
	return vunerability;
}

void Player::setVunerability(bool state)
{
	vunerability = state;
}