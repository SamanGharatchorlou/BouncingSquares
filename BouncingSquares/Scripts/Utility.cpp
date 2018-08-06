#include "stdafx.h"
#include "Utility.h"

#include <iostream>
#include <vector>
#include <cstdlib>


// prevents diagonal movement being faster than intended
sf::Vector2f normalise(sf::Vector2f vec)
{
	if (vec.x == 0 && vec.y == 0)
		return sf::Vector2f(0, 0);

	float magnitude = sqrt((vec.x * vec.x) + (vec.y * vec.y));

	return sf::Vector2f(vec.x, vec.y) / magnitude;
}


// get the distance between two vectors
float PointPointDistance(sf::Vector2f vectorA, sf::Vector2f vectorB)
{
	sf::Vector2f AB = vectorB - vectorA;

	return sqrt((AB.x * AB.x) + (AB.y * AB.y));
}


// short hand for printing vector (primarily for debugging use)
void printVector(sf::Vector2f vec)
{
	std::cout << "x: " << vec.x << "  y: " << vec.y << std::endl;
}

// returns a number of characters as buffers
std::string buffer(int number, char character)
{
	std::string output(number, character);

	return output;
}