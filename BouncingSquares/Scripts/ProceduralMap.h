#pragma once
#include <SFML\Graphics.hpp>
#include <iostream>

std::vector< std::vector<int> > buildMapTemplate(float x, float y);
void randomizeWalls(std::vector< std::vector<int> > &map, int wallWeight);
std::vector< std::vector<int> > smoothMap(std::vector< std::vector<int> > &map);
int wallCount(std::vector< std::vector<int> > &map, int row, int column);
void printArray(std::vector<std::vector<int>> map);
