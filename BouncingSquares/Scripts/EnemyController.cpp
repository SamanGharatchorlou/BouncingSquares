#include "stdafx.h"
#include "EnemyController.h"


int EnemyController::enemyCount()
{
	return enemies.size();
}


// add enemy to eneny list
void EnemyController::addNewEnemy(Enemy &enemy)
{
	enemies.push_back(enemy);
}


Enemy& EnemyController::getEnemy(int index)
{
	return enemies[index];
}


Enemy& EnemyController::getEnemyByIndex(int enemyIndex)
{
	// eneny index 0 = 100, 1 = 101 etc. (distinguise between spaces and walls)
	return enemies[enemyIndex - 100];
}


std::vector<Enemy>& EnemyController::getAllEnemies()
{
	return enemies;
}


// update all enemy positions
void EnemyController::Updates()
{
	for (int i = 0; i < enemyCount(); i++)
	{
		getEnemy(i).Update();
	}
}

void EnemyController::resetEnemies()
{
	Enemy enemy = enemies[0];

	enemies.clear();

	enemies.push_back(enemy);
}

