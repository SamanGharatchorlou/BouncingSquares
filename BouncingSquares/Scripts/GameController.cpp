#include "stdafx.h"
#include "GameController.h"

#include <vector>

// reset game value to defaults
void GameController::setDefaultValues()
{
	lives = d_lives;
	maxLives = d_maxLives;
	maxEnemies = d_maxEnemies;
	additionalEnemies = d_additionalEnemies;
	score = 0;
	pickUps = 0;
	level = 0;

	startGame = false;
	gameOver = false;
	itemsResettable = true;
}

void GameController::addHighScore()
{
	highScores.push_back(score);

	// reverse sort scores
	std::sort(highScores.rbegin(), highScores.rend());
}

int GameController::getHighScore(int index)
{
	if (index < highScores.size())
		return highScores[index];
	else
		return -1;
}

int GameController::gamesPlayed()
{
	return highScores.size();
}

int GameController::getLevel()
{
	return level;
}

void GameController::nextLevel()
{
	level++;
}

int GameController::getMaxLives()
{
	return maxLives;
}

void GameController::increaseMaxLives()
{
	maxLives++;
}

int GameController::getLives()
{
	return lives;
}

void GameController::addLife()
{
	lives++;
}

void GameController::reduceLife()
{
	lives--;
}

// number of enemies to increase by next level
void GameController::increaseAdditionalEnemies()
{
	additionalEnemies++;
}

void GameController::increaseMaxEnemies()
{
	maxEnemies += additionalEnemies;
}

int GameController::getMaxEnemies()
{
	return maxEnemies;
}

void GameController::increaseScore()
{
	score += level;
}

int GameController::getScore()
{
	return score;
}

void GameController::increasePickUps()
{
	pickUps++;
}

int GameController::getPickUpCount()
{
	return pickUps;
}

void GameController::setGameOver(bool state)
{
	gameOver = state;
}

bool GameController::gameOverStatus()
{
	return gameOver;
}

void GameController::setGameStarted(bool status)
{
	startGame = status;
}

bool GameController::gameStartedStatus()
{
	return startGame;
}

void GameController::setResettable(bool state)
{
	itemsResettable = state;
}

bool GameController::resettableItems()
{
	return itemsResettable;
}