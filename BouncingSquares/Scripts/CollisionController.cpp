#include "stdafx.h"
#include "CollisionController.h"
#include "EnemyController.h"
#include "Player.h"
#include "Enemy.h"
#include "Utility.h"

#include <cmath>
#include <iostream>

// NOTE: a bunch of this is plain wrong... positions are not taken from the center but from the
// top left point. FYI.

//NOTE: collisions must happen after update as the position needs to have been
// changed to detect the collision, this is then adjusted by the collision function
// and is corrected before the entity is drawn

// copy map blueprint for wall and body interations
// convert the pixel positions to a 'tile' position and check array indexes rather than pixels
// wall = 0 and space = 1
void CollisionController::copyMapBlueprints(std::vector< std::vector<int> > mapBlueprint)
{
	// direct copy, not to be changed - make this const?
	caveBlueprint = mapBlueprint;

	// map blueprint for wall collisions - make this const?
	wallInterationMap = mapBlueprint;

	// map blueprint for body collisions
	bodyInteractionMap = mapBlueprint;
}


// check whether player position on mapblueprint is a wall or space
bool CollisionController::wallCollision(Player &player)
{
	// convert player postion in pixels to blueprint index
	sf::Vector2f playerPosition = player.getTilePosition(tileSize);

	float x = playerPosition.x;
	float y = playerPosition.y;

	// lose of information int -> float we need to check all four corners of tile
	int topLeft = wallInterationMap[round(y)][round(x)];
	int topRight = wallInterationMap[round(y)][x];
	int botLeft = wallInterationMap[y][round(x)];
	int botRight = wallInterationMap[y][x];

	// check 4 corners of tile for collisions
	if ((topLeft == 0) || (topRight == 0) || (botLeft == 0) || (botRight == 0))
		return true;

	// if no collisions
	return false;
}


// same function as above for enemy
bool CollisionController::wallCollision(Enemy &enemy)
{
	// convert player enemy in pixels to blueprint index
	sf::Vector2f enemyPosition = enemy.getTilePosition(tileSize);

	float x = enemyPosition.x;
	float y = enemyPosition.y;

	int topLeft = wallInterationMap[round(y)][round(x)];
	int topRight = wallInterationMap[round(y)][x];
	int botLeft = wallInterationMap[y][round(x)];
	int botRight = wallInterationMap[y][x];

	// check for wall collisions
	if ((topLeft == 0) || (topRight == 0) || (botLeft == 0) || (botRight == 0))
		return true;

	// if no collisions
	return false;
}


// for wall collision calculate rebound direction
void CollisionController::reboundWall(Enemy &enemy)
{
	// convert pixel position to map index
	sf::Vector2f pos = enemy.getTilePosition(tileSize);
	sf::Vector2f direction;

	// is the enemy surrunded by a wall (0) or space (1)
	int topLeft = wallInterationMap[pos.y - 1][pos.x - 1];
	int top = wallInterationMap[pos.y - 1][pos.x];

	int topRight = wallInterationMap[pos.y - 1][pos.x + 1];
	int right = wallInterationMap[pos.y][pos.x + 1];

	int botRight = wallInterationMap[pos.y + 1][pos.x + 1];
	int bot = wallInterationMap[pos.y + 1][pos.x];

	int botLeft = wallInterationMap[pos.y + 1][pos.x - 1];
	int left = wallInterationMap[pos.y][pos.x - 1];

	// sum all walls and/or spaces around enemy, used for deciding which below condition to run
	int sum = topLeft + top + topRight + right + botRight + bot + botLeft + left;

	// --- corner rebound ---

	// for rebounding of a single corner only
	if (sum == 7)
	{
		// top left
		if (topLeft == 0)
			direction = sf::Vector2f(1, 1);

		// top right
		else if (topRight == 0)
			direction = sf::Vector2f(-1, 1);

		// bot right
		else if (botRight == 0)
			direction = sf::Vector2f(-1, -1);

		// bot left
		else if (botLeft)
			direction = sf::Vector2f(1, -1);
	}

	// every other situation other than a signle corner
	else {

		// --- vertical component ---

		// space above and below
		if (top == 1 && bot == 1)
			direction.y = direction.y;

		// wall above and below
		else if (top == 0 && bot == 0)
			direction.y = 0;

		// only wall above
		else if (top == 0)
			direction.y = 1;

		// only wall below
		else if (bot == 0)
			direction.y = -1;


		// --- horizontal component ---

		// space right and left
		if (right == 1 && left == 1)
			direction.x = direction.x;

		// wall right and left
		else if (right == 0 && left == 0)
			direction.x = 0;

		// only wall right
		else if (right == 0)
			direction.x = -1;

		// only wall left
		else if (left == 0)
			direction.x = 1;
	}

	// prevent enemies sometimes getting stuck in corners
	if (direction == sf::Vector2f(0, 0))
		enemy.randomiseDirection();

	else
		enemy.setDirection(direction);
}



// ENEMY - ENEMY
// begin tracking collisions when another enemy is near by and rebound if collision detected
void CollisionController::bodyCollision(Enemy &enemy, std::vector<Enemy> &enemies)
{
	// get and convert xy position to ints
	sf::Vector2f enemyPosition = enemy.getTilePosition(tileSize);

	int x = enemyPosition.x;
	int y = enemyPosition.y;

	// get enemy id
	int enemyID = enemy.getIndex();

	// sum of 5 points topLeft + top + topRight + right + center
	// only these points need to be checked for each enemy
	int sum = bodyInteractionMap[y - 1][x - 1] +
		bodyInteractionMap[y - 1][x] +
		bodyInteractionMap[y - 1][x + 1] +
		bodyInteractionMap[y][x + 1] +
		bodyInteractionMap[y][x];

	// check for enemies surrounding this enemy
	if (sum > 5 + enemyID)
	{
		// place all nearby enemies into list
		std::vector<int> nearbyEnemies = proximityEnemies(sf::Vector2i(x, y), enemyID, enemies);

		for (int i = 0; i < nearbyEnemies.size(); i++)
		{
			// if enemies intersect collision detected
			if (enemy.getBoundary().intersects(enemies[nearbyEnemies[i]].getBoundary()))
			{
				// move in opposite directionsEn
				rebound(enemy, enemies[nearbyEnemies[i]]);
			}
		}
	}
}


 // PLAYER - ENEMY
 // begin tracking collisions when an enemy is near by, return true if collision detected
bool CollisionController::hitPlayer(Player &player, std::vector<Enemy> &enemies)
{
	// get and convert xy position to ints
	sf::Vector2f playerPosition = player.getPosition();
	sf::Vector2f playerTilePosition = player.getTilePosition(tileSize);

	int x = playerTilePosition.x;
	int y = playerTilePosition.y;

	// minimun distance between player and enemy before collision
	float minDistance;
	// distance between player and enemy
	float enemyDistance;

	// sum of 9 surrounding points:
	// top Left + top + top right + right + bot right + bot + bot left + left + center
	int sum = bodyInteractionMap[y - 1][x - 1] + bodyInteractionMap[y - 1][x] +
		bodyInteractionMap[y - 1][x + 1] + bodyInteractionMap[y][x + 1] +
		bodyInteractionMap[y + 1][x + 1] + bodyInteractionMap[y + 1][x] +
		bodyInteractionMap[y + 1][x - 1] + bodyInteractionMap[y][x - 1] +
		bodyInteractionMap[y][x];

	// check for enemies surrounding player
	if (sum > 9)
	{
		// place all nearby enemies into list
		std::vector<int> nearbyEnemies = proximityEnemies(sf::Vector2i(x, y), enemies);

		for (int i = 0; i < nearbyEnemies.size(); i++)
		{	
			sf::Vector2f enemyPos = enemies[nearbyEnemies[i]].getPosition();

			// get current distance between player and near by enemy
			enemyDistance = PointPointDistance(playerPosition, enemyPos);

			// minimun possible distance calculation depends on enemy position relative to player
			if (typeAB(playerPosition, enemyPos))
			{
				// type A (i.e. above or below player)
				minDistance = getMinDistanceA(player, enemies[nearbyEnemies[i]]);
			}
			else
			{
				// type B (i.e. left or right of player
				minDistance = getMinDistanceB(player, enemies[nearbyEnemies[i]]);
			}


			// collision if current distance < min distance
			if (enemyDistance <= minDistance)
				return true;
		}
	}
	// no collision
	return false;
}


// gets minimun distance when enemy above or below player
float CollisionController::getMinDistanceA(Player &player, Enemy &enemy)
{
	// direction vector player -> enemy
	sf::Vector2f PEbar = normalise(enemy.getPosition() - player.getPosition());

	// angle between y = -1 and PEbar
	float phi = atan(PEbar.x / PEbar.y);

	float playerRadius = player.getShape().getRadius();

	// distance from center to a point on the perimiter of the square
	float enemyRadius = enemy.getShape().getSize().x / (2 * sin(phi));

	// the distance if the player and enemy were touching with direction vector PEbar
	return playerRadius + abs(enemyRadius);
}


// gets minimun distance when enemy to the right or left of player
float CollisionController::getMinDistanceB(Player &player, Enemy &enemy)
{
	sf::Vector2f PEbar = normalise(enemy.getPosition() - player.getPosition());

	float phi = atan(PEbar.x / PEbar.y);

	float playerRadius = player.getShape().getRadius();

	// this function uses cos rather than sin
	float enemyRadius = enemy.getShape().getSize().x / (2 * cos(phi));

	return playerRadius + abs(enemyRadius);
}


// returns true for type A collision and false for type B
bool CollisionController::typeAB(sf::Vector2f playerPosition, sf::Vector2f enemyPosition)
{
	// player to enemy vector
	sf::Vector2f PEbar = normalise(enemyPosition - playerPosition);

	// type A for |x| > |y|
	if (abs(PEbar.x) >= abs(PEbar.y))
		return true;

	// type B for |x| < |y|
	else
		return false;
}


// ENEMY - ENEMY
// check top left, top, top right, right an center for enemies when an enemy is near by
// enemy is near by if it occupies a surround tile
std::vector<int> CollisionController::proximityEnemies(sf::Vector2i enemyPosition, int enemyIndex, std::vector<Enemy> &enemies)
{
	// get and convert xy position to ints
	int xPos = enemyPosition.x;
	int yPos = enemyPosition.y;

	// list to hold all neay by enemies
	std::vector<int> nearByEnemies;

	// check center tile
	// cannot use addEnemiesAtTile due to extra condition check (enemies[i].getIndex() != enemyIndex)
	if (bodyInteractionMap[yPos][xPos] > enemyIndex + 1)
	{
		// if there is an enemy (other than this one) in the center, add it to nearByEnemies
		for (int i = 0; i < enemies.size(); i++)
		{
			// is the position of any of the enemies in the top left square
			if (floor(enemies[i].getTilePosition(tileSize).x) == xPos &&
				floor(enemies[i].getTilePosition(tileSize).y) == yPos &&
				enemies[i].getIndex() != enemyIndex)
			{
				nearByEnemies.push_back(enemies[i].getIndex() - 100);
			}
		}
	}

	// check 4 surrounding tiles for enemies and add them to nearByEnemies list
	// only check tiles which hold an enemy

	// top left
	if (bodyInteractionMap[yPos - 1][xPos - 1] > 1)
		addEnemiesAtTile(enemies, xPos - 1, yPos - 1, nearByEnemies);

	// top
	if (bodyInteractionMap[yPos - 1][xPos] > 1)
		addEnemiesAtTile(enemies, xPos, yPos - 1, nearByEnemies);

	// top right
	if (bodyInteractionMap[yPos - 1][xPos + 1] > 1)
		addEnemiesAtTile(enemies, xPos + 1, yPos - 1, nearByEnemies);

	// right
	if (bodyInteractionMap[yPos][xPos + 1] > 1)
		addEnemiesAtTile(enemies, xPos + 1, yPos, nearByEnemies);

	return nearByEnemies;
}


// PLAYER - ENEMY
// similar to function above, however all surrounding tiles (incl. center) must be checked for near by enemies
std::vector<int> CollisionController::proximityEnemies(sf::Vector2i playerPosition, std::vector<Enemy> &enemies)
{
	// get and convert xy position to ints
	int xPos = playerPosition.x;
	int yPos = playerPosition.y;

	// list to hold all enemies nearby the enemy at position enemyPosition
	std::vector<int> nearByEnemies;

	// check all surrounding tiles for enemies and add them to nearByEnemies list
	// only check tiles which hold an enemy

	// NOTE: is there a smarter/better way of doing this ?????

	// center
	if (bodyInteractionMap[yPos][xPos] > 1)
		addEnemiesAtTile(enemies, xPos, yPos, nearByEnemies);

	// top left
	if (bodyInteractionMap[yPos - 1][xPos - 1] > 1)
		addEnemiesAtTile(enemies, xPos - 1, yPos - 1, nearByEnemies);

	// top
	if (bodyInteractionMap[yPos - 1][xPos] > 1)
		addEnemiesAtTile(enemies, xPos, yPos - 1, nearByEnemies);

	// top right
	if (bodyInteractionMap[yPos - 1][xPos + 1] > 1)
		addEnemiesAtTile(enemies, xPos + 1, yPos - 1, nearByEnemies);

	// right
	if (bodyInteractionMap[yPos][xPos + 1] > 1)
		addEnemiesAtTile(enemies, xPos + 1, yPos, nearByEnemies);

	// bot right
	if (bodyInteractionMap[yPos + 1][xPos + 1] > 1)
		addEnemiesAtTile(enemies, xPos + 1, yPos + 1, nearByEnemies);

	// bot
	if (bodyInteractionMap[yPos + 1][xPos] > 1)
		addEnemiesAtTile(enemies, xPos, yPos + 1, nearByEnemies);

	// bot left
	if (bodyInteractionMap[yPos + 1][xPos - 1] > 1)
		addEnemiesAtTile(enemies, xPos -1, yPos + 1, nearByEnemies);

	// left
	if (bodyInteractionMap[yPos][xPos - 1] > 1)
		addEnemiesAtTile(enemies, xPos - 1, yPos, nearByEnemies);

	return nearByEnemies;
}


// add all enemies on tile x,y to nearByEnemies list
void CollisionController::addEnemiesAtTile(std::vector<Enemy> &enemies, int x, int y, std::vector<int> &nearByEnemies)
{
	for (int i = 0; i < enemies.size(); i++)
	{
		// add enemy if position at given x & y
		if (floor(enemies[i].getTilePosition(tileSize).x) == x &&
			floor(enemies[i].getTilePosition(tileSize).y) == y)
		{
			nearByEnemies.push_back(enemies[i].getIndex() - 100);
		}
	}
}


// marks the position of every enemy by index on the bodyInteractionMap
void CollisionController::markPositions(std::vector<Enemy> &enemies)
{
	sf::Vector2f enemyPos;

	// reset map
	bodyInteractionMap = caveBlueprint;

	for (int i = 0; i < enemies.size(); i++)
	{
		enemyPos = enemies[i].getTilePosition(tileSize);

		// mark interaction map with the enemies position
		bodyInteractionMap[enemyPos.y][enemyPos.x] += enemies[i].getIndex();
	}
}


// simple rebound function, enemies move in opposite directions
void CollisionController::rebound(Enemy &enemy1, Enemy &enemy2)
{
	//vector enemy 1 -> enemy 2
	sf::Vector2f e1Toe2 = enemy2.getPosition() - enemy1.getPosition();

	enemy1.setDirection(-e1Toe2);
	enemy2.setDirection(e1Toe2);
}


// tile size required for various functions
void CollisionController::setTileSize(float sizeOfTile)
{
	tileSize = sizeOfTile;
}