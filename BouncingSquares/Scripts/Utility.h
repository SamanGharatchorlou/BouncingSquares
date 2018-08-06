#pragma once
#include <SFML\Graphics.hpp>

// -- General untility functions --

// normalise vector
sf::Vector2f normalise(sf::Vector2f vec);

// distance between two points
float PointPointDistance(sf::Vector2f vectorA, sf::Vector2f vectorB);

// prints vector to console e.g. "x: 12  y: 34"
void printVector(sf::Vector2f vec);

std::string buffer(int number, char character);