#include "stdafx.h"
#include "Collectables.h"
#include "Environment.h"

// add new pick up to map and list
void Collectables::addItems(Environment &map, int numberOfItems, float itemSize)
{
	items.clear();

	for (int i = 0; i < numberOfItems; i++)
	{
		Item pickUp(map.findEmptyTile(), itemSize, 3.0f);

		items.push_back(pickUp);
	}
}

// add new life pick up to map and list
void Collectables::addLife(Environment &map, float points, float itemSize)
{
	Item life(map.findEmptyTile(), itemSize, points);
	life.getShape().setFillColor(sf::Color::Magenta);

	lives.push_back(life);
}

Item& Collectables::item(int index)
{
	if(items.size() > index)
		return items[index];
}

int Collectables::itemCount()
{
	return items.size();
}

int Collectables::countActiveItems()
{
	int activeItems = 0;

	for (int i = 0; i < items.size(); i++)
	{
		// only count items that have not been collected
		if (!items[i].isCollected())
			activeItems++;
	}
	return activeItems;
}

void Collectables::resetPositions(Environment &map)
{
	for (int i = 0; i < items.size(); i++)
	{
		items[i].setPosition(map.findEmptyTile());
	}
}

Item& Collectables::getLife(int index)
{
	if(lives.size() > index)
		return lives[index];
}

int Collectables::lifeCount()
{
	return lives.size();
}

int Collectables::countActiveLives()
{
	int activeLives = 0;

	for (int i = 0; i < lives.size(); i++)
	{
		if (!lives[0].isCollected())
			activeLives++;
	}
	return activeLives;
}

void Collectables::resetItems()
{
	items.clear();
	lives.clear();
}