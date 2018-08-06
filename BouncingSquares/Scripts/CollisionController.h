#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Item.h"

// object for handling all game collisions
class CollisionController
{
private:

	// direct copy of mapBlueprint
	std::vector< std::vector<int> > caveBlueprint;

	// used to register collisions with walls only
	std::vector< std::vector<int> > wallInterationMap;

	// used to register moving body collisions
	std::vector< std::vector<int> > bodyInteractionMap;

	float tileSize;

	// returns all near by enemies, by index
	std::vector<int> proximityEnemies(sf::Vector2i enemyTilePosition, int enemyIndex, std::vector<Enemy> &enemies);
	std::vector<int> proximityEnemies(sf::Vector2i playerPosition, std::vector<Enemy> &enemies);

	// pushes all enemies on tile x,y to the nearByEnemies list
	void addEnemiesAtTile(std::vector<Enemy> &enemies, int x, int y, std::vector<int> &nearbyEnemies);

	// type A or type B collision
	bool typeAB(sf::Vector2f playerPosition, sf::Vector2f enemyPosition);

	// gets minimun possible distance between player and enemy without collision
	float getMinDistanceA(Player &player, Enemy &enemy);
	float getMinDistanceB(Player &player, Enemy &enemy);

	// enemies move in opposite directions
	void rebound(Enemy &enemy1, Enemy &enemy2);

public:
	// checks for wall collisions
	bool wallCollision(Player &player);
	bool wallCollision(Enemy &enemy);

	// enemy rebounds of wall in a calculated direction
	void reboundWall(Enemy &enemy);

	// marks the position of every enemy on the map blueprint by ID
	void markPositions(std::vector<Enemy> &enemies);

	// enemy - enemy collision detection
	void bodyCollision(Enemy &enemy, std::vector<Enemy> &enemies);

	// player - enemy collision detection
	bool hitPlayer(Player &player, std::vector<Enemy> &enemies);

	// copy map blueprints for collision detections
	void copyMapBlueprints(std::vector< std::vector<int> > mapBlueprint);

	void setTileSize(float sizeOfTile);
};
