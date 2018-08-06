#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

// package up all in game variables like scores, lives etc
class GameController
{
private:
	// top scores
	std::vector<int> highScores;

	// default values
	int d_lives = 2;
	int d_maxLives = 2;
	int d_maxEnemies = 5;
	int d_additionalEnemies = 2;

	// variable ... variables
	int lives;
	int maxLives;
	int maxEnemies;
	int additionalEnemies;
	int score;
	int pickUps;
	int level;

	bool startGame;
	bool gameOver;
	bool itemsResettable;

public:
	void setDefaultValues();

	void addHighScore();
	int getHighScore(int index);
	int gamesPlayed();

	// level
	int getLevel();
	void nextLevel();

	// lives
	int getMaxLives();
	void increaseMaxLives();

	int getLives();
	void addLife();
	void reduceLife();

	// enemies
	void increaseAdditionalEnemies();
	void increaseMaxEnemies();
	int getMaxEnemies();

	// score
	void increaseScore();
	int getScore();

	// items
	void increasePickUps();
	int getPickUpCount();

	// game over
	void setGameOver(bool state);
	bool gameOverStatus();

	// game started
	void setGameStarted(bool state);
	bool gameStartedStatus();

	// items are resettable
	void setResettable(bool state);
	bool resettableItems();
};