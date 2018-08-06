#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "ProceduralMap.h"

class Environment
{
private:
	sf::VertexArray verticies;
	sf::Texture tileImg;

	// direct copy of mapBlueprint
	std::vector< std::vector<int> > caveBlueprint;

	// map blue print dimentions
	int pixelWidth;
	int pixelHeight;

	int tileWidth;
	int tileHeight;

	// number of pixels in one index i.e. pixels 0-20 = index 0 in map blueprint
	float tileSize;

public:
	Environment();

	sf::Texture getTexture();
	bool loadTexture(std::string imgName);

	// copy blueprint to object variables
	void buildMapBlueprints(std::vector< std::vector<int> > mapBlueprint);

	// configure variables
	void setMapDimentions(float width, float height, float tileSize);

	// places a wall on every 0 and skips every space (1)
	sf::VertexArray tileMap();

	// finds a random empty tile in map blueprint
	sf::Vector2f findEmptyTile();
};
