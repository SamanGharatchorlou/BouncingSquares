#include "stdafx.h"
#include "Environment.h"
#include "EnemyController.h"

#include <iostream>

Environment::Environment()
{
	verticies.setPrimitiveType(sf::Quads);
}

// load texture from file
bool Environment::loadTexture(std::string imgName)
{
	if (tileImg.loadFromFile(imgName))
		return true;
	else
		return false;
}


sf::Texture Environment::getTexture()
{
	return tileImg;
}


// copy map blueprint for wall and body interations
void Environment::buildMapBlueprints(std::vector< std::vector<int> > mapBlueprint)
{
	// direct copy, not to be changed - make this a const?
	caveBlueprint = mapBlueprint;
}


// set map boundaries and tile size
void Environment::setMapDimentions(float mapWidth, float mapHeight, float mapTileSize)
{
	// size of each map tile
	tileSize = mapTileSize;

	pixelWidth = mapWidth;
	pixelHeight = mapHeight;

	tileWidth = mapWidth / tileSize;
	tileHeight = mapWidth / tileSize;

	// change to fit map size
	// note: many of these will be empty as only walls will be assigned position and texture coords
	verticies.resize(tileWidth * tileHeight * 4);
}


// Build a map from a given 2d vector of 1s and 0s (place a square on every 0)
sf::VertexArray Environment::tileMap()
{
	sf::Vector2u dimentions = tileImg.getSize();

	int caveHeight = caveBlueprint.size();
	int caveWidth = caveBlueprint[0].size();

	int tileNumber;

	for (int row = 0; row < caveHeight; row++)
	{
		for (int column = 0; column < caveWidth; column++)
		{
			// if space, do no assign values and skip index
			if (caveBlueprint[row][column] == 1)
				continue;

			// get index of tile in blueprint if it were a 1d list
			tileNumber = column + (row * caveWidth);

			// create quad object and set its memory address to a tile in the vertex array
			// *quad points to the tiles first vertex
			sf::Vertex *quad = &verticies[tileNumber * 4];

			// 4 verticies of quad
			quad[0].position = sf::Vector2f(column, row) * tileSize;
			quad[1].position = sf::Vector2f(column + 1, row) * tileSize;
			quad[2].position = sf::Vector2f(column + 1, row + 1) * tileSize;
			quad[3].position = sf::Vector2f(column, row + 1) * tileSize;

			// texture fully covers square
			quad[0].texCoords = sf::Vector2f(0, 0);
			quad[1].texCoords = sf::Vector2f(dimentions.x, 0);
			quad[2].texCoords = sf::Vector2f(dimentions.x, dimentions.y);
			quad[3].texCoords = sf::Vector2f(0, dimentions.y);
		}
	}
	return verticies;
}


// find a random empty tile
sf::Vector2f Environment::findEmptyTile()
{
	int mapHeight = caveBlueprint.size();
	int mapWidth = caveBlueprint[0].size();

	// caveBlueprint[0][0] always a wall
	int rowIndex = 0;
	int columnIndex = 0;

	// keep trying random positions until an empty one is found
	while (caveBlueprint[rowIndex][columnIndex] == 0)
	{
		rowIndex = rand() % mapHeight;
		columnIndex = rand() % mapWidth;
	}

	return sf::Vector2f(columnIndex, rowIndex) * tileSize;
}