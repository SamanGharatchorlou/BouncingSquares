#pragma once
#include <SFML/Graphics.hpp>
#include "Item.h"
#include "Environment.h"

class Collectables
{
private:
	std::vector<Item> items;
	std::vector<Item> lives;

public:
	//int extraLife = 0;

	void addItems(Environment &map, int numberOfItems, float itemSize);
	void addLife(Environment &map, float points, float itemSize);

	Item& item(int index);
	Item& getLife(int index);

	int itemCount();
	int countActiveItems();

	int lifeCount();
	int countActiveLives();

	void resetPositions(Environment &map);

	void resetItems();
};