#include <iostream>
#include <vector>
#include <cstdlib>

#include "stdafx.h"
#include "ProceduralMap.h"

// creates a 2d vector for map blueprint
std::vector< std::vector<int> > buildMapTemplate(float x, float y)
{
	std::vector< std::vector<int> > aMap(y, std::vector<int>(x));

	return aMap;
}


// randomize 2d vector with a 1 or 0 (space or wall)
void randomizeWalls(std::vector< std::vector<int> > &map, int wallWeight)
{
	srand(time(NULL));

	int rows = map.size();
	int columns = map[0].size();

	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			// edge is always a wall
			if (row == 0 || row == rows - 1 || column == 0 || column == columns - 1)
			{
				map[row][column] == 0;
			}
			else
			{
				// user can choose how much random wall will appear
				map[row][column] = (rand() % 101 > wallWeight) ? 1 : 0;
			}
		}
	}
}


// counts the number of walls surrounding (incl.) a given index
int wallCount(std::vector< std::vector<int> > &map, int row, int column)
{
	int wallCounter = 0;

	for (int y = row - 1; y <= row + 1; y++)
	{
		for (int x = column - 1; x <= column + 1; x++)
		{
			if (map[y][x] == 0)
				wallCounter++;
		}
	}

	return wallCounter;
}


// index surrounded by walls becomes a wall and visa versa with spaces
// causes grouping of walls and space as you would expect, run this multiple times
std::vector< std::vector<int> > smoothMap(std::vector< std::vector<int> > &map)
{
	int rows = map.size();
	int columns = map[0].size();
	int wallCounter;

	// new vector so smoothing doesn't affect wall count of later indexes
	std::vector< std::vector<int> > smoothedMap(rows, std::vector<int>(columns));

	// change index based on a wall count
	for (int row = 1; row < rows - 2; row++)
	{
		for (int column = 1; column <= columns - 2; column++)
		{
			wallCounter = wallCount(map, row, column);

			// for many walls change index to wall
			if (wallCounter > 4)
				smoothedMap[row][column] = 0;

			// for many space change index to space
			else if (wallCounter < 4)
				smoothedMap[row][column] = 1;
		}
	}

	return smoothedMap;
}


// utility for printing 2d vector
void printArray(std::vector<std::vector<int>> map)
{
	int rows = map.size();
	int columns = map[0].size();

	for (int row = 0; row < rows; row++)
	{
		for (int column = 0; column < columns; column++)
		{
			std::cout << map[row][column] << " ";
		}
		std::cout << std::endl;
	}
}