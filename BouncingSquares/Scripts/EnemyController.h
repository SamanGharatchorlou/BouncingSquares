#pragma once
#include <SFML\Graphics.hpp>
#include "Enemy.h"

// allows for the control of multiple enemies
class EnemyController
{
private:
	// hold all enemies
	std::vector<Enemy> enemies;

public:
	int enemyCount();

	void addNewEnemy(Enemy &enemy);

	std::vector<Enemy>& getAllEnemies();
	Enemy& getEnemy(int index);
	Enemy& getEnemyByIndex(int enemyIndex);

	// remove all enemies except first enemy
	void resetEnemies();

	// apply all enenmy updates
	void Updates();
};
