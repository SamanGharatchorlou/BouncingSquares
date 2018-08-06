#pragma once
#include <SFML/Graphics.hpp>

class Item
{
private:
	sf::CircleShape circleShape;

	sf::Vector2f position;

	bool collected = false;

public:
	Item(sf::Vector2f itemPosition, float radius, float points);

	sf::CircleShape& getShape();

	sf::Vector2f getPosition();
	sf::Vector2f getTilePosition(float tileSize);

	void setPosition(sf::Vector2f newPosition);

	bool pickUpItem(sf::Vector2f playerPosition);

	void collectItem();
	bool isCollected();
};