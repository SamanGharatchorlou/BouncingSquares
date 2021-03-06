// PlayingWithShapes.cpp : Defines the entry point for the console application.
//

// include scripts
#include "stdafx.h"
#include "ProceduralMap.h"
#include "Environment.h"
#include "GameController.h"
#include "Collectables.h"
#include "Item.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyController.h"
#include "CollisionController.h"
#include "Utility.h"

// include libraries
#include <SFML\Audio.hpp>
#include <SFML\Graphics.hpp>
#include <iostream>
#include <cstdlib>
#include <sstream>


int main()
{
	// ----- Configuration -----

	// map dimentions
	int windowWidth = 1000;
	int windowHeight = 800;

	// max frame rate
	int frameRateLimit = 75;

	// player attributes
	float playerSize = 5;
	float playerPoints = 20;

	// smallest dimention
	float tileSize = playerSize * 4;

	// reset game counter
	int holdHCounter = 0;

	// prevent multiple space bar presses
	bool selected = false;

	// ----- HUD -----

	sf::Font font;
	font.loadFromFile("GameFiles/game_over.ttf");


	// -- main player display --
	sf::Text hud;
	hud.setFont(font);
	hud.setCharacterSize(90);
	hud.setFillColor(sf::Color::Green);
	hud.setPosition(sf::Vector2f(0, -55));


	// -- Opening Display--

	// box positions
	sf::Vector2f center = sf::Vector2f(windowWidth, windowHeight * 1.07) * 0.5f;
	sf::Vector2f outerBoxSize = sf::Vector2f(windowWidth * 0.95f, windowHeight * 0.5f);
	sf::Vector2f innerBoxSize = outerBoxSize - sf::Vector2f(20, 20);

	// outer box
	sf::RectangleShape textBoxOuter;
	textBoxOuter.setPosition(center - outerBoxSize / 2.0f);
	textBoxOuter.setSize(outerBoxSize);
	textBoxOuter.setFillColor(sf::Color::White);

	// inner box
	sf::RectangleShape textBoxInner;
	textBoxInner.setPosition(center - innerBoxSize / 2.0f);
	textBoxInner.setSize(innerBoxSize);
	textBoxInner.setFillColor(sf::Color::Black);

	// display text
	std::stringstream gameText;
	gameText << "\n\n\n\n\n"
		<< "	   1. Use the arrows to move around\n"
		<< "	   2. Dodge these guys\n"
		<< "	   3. Collect       to increase your score and move onto the next level\n"
		<< "	   4. Gain a life after each level (up to your max)\n"
		<< "	   5. Pick up       to gain a life and permenantly increase your max lives\n"
		<< "	   6. Press P to reset item positions (once per level)"
		<< "\n\n											   GOT IT, LET'S GO! (enter)";

	// format text
	sf::Text gameInstructions;
	gameInstructions.setFont(font);
	gameInstructions.setCharacterSize(75);
	gameInstructions.setString(gameText.str());
	gameInstructions.setFillColor(sf::Color::Green);

	// demo square/enemy
	sf::RectangleShape demoSquare;
	demoSquare.setPosition(textBoxOuter.getPosition() + sf::Vector2f(300, 75));
	demoSquare.setSize(sf::Vector2f(27, 27));
	demoSquare.setFillColor(sf::Color::Red);

	// demo triangle/pick up
	sf::CircleShape demoTriangle;
	demoTriangle.setPosition(textBoxOuter.getPosition() + sf::Vector2f(148, 113));
	demoTriangle.setPointCount(3);
	demoTriangle.setRadius(18);
	demoTriangle.setFillColor(sf::Color::Green);

	// demo pentagon/extra life
	sf::CircleShape demoPenta;
	demoPenta.setPosition(textBoxOuter.getPosition() + sf::Vector2f(157, 191));
	demoPenta.setPointCount(5);
	demoPenta.setRadius(18);
	demoPenta.setFillColor(sf::Color::Magenta);


	// game over / continue game displays
	sf::Text gameStateDisplay;
	gameStateDisplay.setFont(font);
	gameStateDisplay.setFillColor(sf::Color::Green);


	// -- High scores --
	sf::Text highScoreText;
	highScoreText.setFont(font);
	highScoreText.setFillColor(sf::Color::Green);


	// ----- AUDIO -----

	// background
	sf::Music music; 
	if (!music.openFromFile("GameFiles/background.wav"))
		return -1;

	music.play();
	music.setLoop(true);
	music.setVolume(60);


	// next level
	sf::SoundBuffer nextLevelBuffer;
	if (!nextLevelBuffer.loadFromFile("GameFiles/nextLevel.wav"))
		return -1;

	sf::Sound nextLevel;
	nextLevel.setBuffer(nextLevelBuffer);


	// wall bump
	sf::SoundBuffer wallBump;
	if (!wallBump.loadFromFile("GameFiles/bump.wav"))
		return -1;

	sf::Sound bump;
	bump.setBuffer(wallBump);


	// item pick up
	sf::SoundBuffer ItemPickUp;
	if (!ItemPickUp.loadFromFile("GameFiles/ping.wav"))
		return -1;
	
	sf::Sound bing;
	bing.setBuffer(ItemPickUp);
	bing.setVolume(25);


	// extra life
	sf::SoundBuffer extraLifeBuffer;
	if (!extraLifeBuffer.loadFromFile("GameFiles/extraLife.wav"))
		return -1;

	sf::Sound extraLife;
	extraLife.setBuffer(extraLifeBuffer);


	// lost life
	sf::SoundBuffer lostLifeBuffer;
	if (!lostLifeBuffer.loadFromFile("GameFiles/lostLife.wav"))
		return -1;

	sf::Sound lostLife;
	lostLife.setBuffer(lostLifeBuffer);


	// game over
	sf::SoundBuffer gameOverBuffer;
	if (!gameOverBuffer.loadFromFile("GameFiles/gameOver.wav"))
		return -1;

	sf::Sound gameOver;
	gameOver.setBuffer(gameOverBuffer);
	
	// ----- Create Game Objects -----

	sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Playing With Shapes");
	window.setFramerateLimit(frameRateLimit);

	// -- environment --
	Environment environment;
	environment.loadTexture("GameFiles/wall.png");

	environment.setMapDimentions(windowWidth, windowHeight, tileSize);

	// build random cave blueprint
	std::vector< std::vector<int> > caveBlueprint = buildMapTemplate(windowWidth / tileSize, windowHeight / tileSize);
	randomizeWalls(caveBlueprint, 27);

	for (int i = 0; i < 5; i++)
		caveBlueprint = smoothMap(caveBlueprint);

	// configure blueprints
	environment.buildMapBlueprints(caveBlueprint);

	// turn blueprint into drawable map
	sf::VertexArray map = environment.tileMap();


	// -- game variables --
	GameController gameController;
	gameController.setDefaultValues();


	// -- Items --
	Collectables items;


	// -- player --
	Player player(environment.findEmptyTile(), playerSize, playerPoints);
	player.changeColour(sf::Color::Cyan);


	// -- enemies --
	EnemyController enemyController;

	// create first enemy
	sf::Vector2f enemySize = sf::Vector2f(10, 10);

	// start at 100 to distinguise between space and walls (1s & 0s)
	int enemyIndex = 100;

	Enemy enemy(environment.findEmptyTile(), enemySize, enemyIndex);
	enemyController.addNewEnemy(enemy);


	// -- collisions --
	CollisionController collision;
	collision.copyMapBlueprints(caveBlueprint);
	collision.setTileSize(tileSize);


	// ----- Game loop -----
	while (window.isOpen()) {

		// ----- Inputs -----
		sf::Event event;

		// log position in case of collision
		player.logPosition();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// enter (start game)
		if (!gameController.gameStartedStatus() && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			gameController.setGameStarted(true);
			gameController.setResettable(true);
			player.setVunerability(true);

			nextLevel.play();
		}

		// esc (end game)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();

		// UP
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			player.moveUp();
				
		// RIGHT
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			player.moveRight();
		
		// DOWN
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			player.moveDown();

		// LEFT
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			player.moveLeft();

		// P (reset all item positions)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::P) && gameController.resettableItems())
		{
			items.resetPositions(environment);
			gameController.setResettable(false);
		}

		// SPACE BAR (continue game)
		if (!player.getVunerability() && sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !selected)
		{
			player.setVunerability(true);
			selected = true;
		}

		// HOLD H (restart game)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
		{
			holdHCounter++;

			// hold for 2 seconds to reset game (75 frames /s)
			if (holdHCounter == 150)
			{
				holdHCounter = 0;

				// reset all game variables
				gameController.setDefaultValues();

				// randomise player position
				player.setPosition(environment.findEmptyTile());

				// reset enemies
				enemyController.resetEnemies();

				// reset items
				items.resetItems();
			}
		}
		else
			holdHCounter = 0;

		// ----- Next Level -----

		// if no more items to pick up, increase level
		if (items.countActiveItems() == 0)
		{
			// play audio
			if (gameController.getLevel() > 1)
				nextLevel.play();

			// increase number of pick ups, enemies and level
			gameController.nextLevel();
			gameController.increaseMaxEnemies();
			gameController.increasePickUps();

			// add more items
			items.addItems(environment, gameController.getPickUpCount(), playerSize * 2);

			// increase additional enemies and place life pick up every 3 levels
			if (gameController.getLevel() % 3 == 0)
			{
				items.addLife(environment, 4, 7);
				gameController.increaseAdditionalEnemies();
			}

			// if lives < max increase life by one
			if (gameController.getLives() < gameController.getMaxLives())
			{
				gameController.addLife();
			}

			gameController.setResettable(true);
		}


		// ----- Position Updates -----

		// only allow movement once game has started
		if (gameController.gameStartedStatus())
		{
			// update all enemy positions
			enemyController.Updates();

			// update player position
			player.Update();
		}


		// ----- Collision Detections -----

		// mark all enemy positions
		collision.markPositions(enemyController.getAllEnemies());

		// ENEMY-ENEMY
		for (int i = 0; i < enemyController.enemyCount(); i++)
		{
			collision.bodyCollision(enemyController.getEnemy(i), enemyController.getAllEnemies());
		}

		// PLAYER-ENEMY
		for (int i = 0; i < enemyController.enemyCount(); i++)
		{	
			if (collision.hitPlayer(player, enemyController.getAllEnemies()))
			{
				// give player chance to reset
				if (player.getVunerability() && gameController.getLives() > 0)
				{
					lostLife.play();

					gameController.reduceLife();

					player.logHitPosition();
					player.setVunerability(false);

					// allow player to press space bar again
					selected = false;
				}
			}
		}

		// restict player motion if hit (until space bar is pressed)
		if (!player.getVunerability())
		{
			if (PointPointDistance(player.getHitPosition(), player.getPosition()) > 40)
			{
				player.resetPosition();
			}
		}


		// PLAYER-WALL
		if (collision.wallCollision(player))
		{
			player.resetPosition();

			// play bump sound
			if (!bump.getStatus())
				bump.play();
		}

		// ENEMY-WALL
		for (int i = 0; i < enemyController.enemyCount(); i++)
		{
			if (collision.wallCollision(enemyController.getEnemy(i)))
			{
				collision.reboundWall(enemyController.getEnemy(i));

				// add new enemy per wall bounce
				if (enemyController.enemyCount() < gameController.getMaxEnemies())
				{
					Enemy enemy(environment.findEmptyTile(), enemySize, 100 + enemyController.enemyCount());
					enemyController.addNewEnemy(enemy);
				}
			}
		}

		// player can move around but not interact with items when game is over
		if (!gameController.gameOverStatus())
		{
			// PLAYER-ITEM
			for (int i = 0; i < items.itemCount(); i++)
			{
				// check item hasnt been collected yet and player is close enough to collect it
				if (!items.item(i).isCollected() && items.item(i).pickUpItem(player.getPosition()))
				{
					bing.play();
					gameController.increaseScore();
				}
			}

			// PLAYER-EXTRALIFE
			for (int i = 0; i < items.lifeCount(); i++)
			{
				// check item hasnt been collected yet and player is close enough to collect it
				if (!items.getLife(i).isCollected() && items.getLife(i).pickUpItem(player.getPosition()))
				{
					extraLife.play();

					gameController.increaseMaxLives();
					gameController.addLife();
				}
			}
		}


		// ----- HUD -----

		// main game stats
		std::stringstream gameStats;

		gameStats <<
			"Score " << gameController.getScore() << std::string(35, ' ') <<
			"LEVEL " << gameController.getLevel() << std::endl <<
			"Lives " << gameController.getLives();

		hud.setString(gameStats.str());


		// continue game
		if (!player.getVunerability() && gameController.getLives() > 0)
		{
			std::stringstream continueGame;

			continueGame << "Press the space bar to continue";

			gameStateDisplay.setCharacterSize(100);
			gameStateDisplay.setPosition(230, 300);
			gameStateDisplay.setString(continueGame.str());
		}


		// game over
		if (gameController.getLives() == 0 && !gameController.gameOverStatus())
		{
			std::stringstream endGame;

			endGame << "        GAME OVER :(" <<
				"\n\n        Final Score " << gameController.getScore() <<
				"\n\n\n\n    Hold G to play again";

			gameOver.play();
			music.setVolume(30);

			gameController.addHighScore();

			gameStateDisplay.setCharacterSize(130);
			gameStateDisplay.setPosition(230, 200);
			gameStateDisplay.setString(endGame.str());

			gameController.setGameOver(true);
		}


		// high scores
		if (gameController.gamesPlayed() > 0)
		{
			std::stringstream highScore;

			// during game only display top score
			if (!gameController.gameOverStatus())
			{
				highScoreText.setPosition(sf::Vector2f(windowWidth - 200, -45));
				highScoreText.setCharacterSize(70);
				highScore << "High Score: " << gameController.getHighScore(0) << std::endl;
			}
			// during game over screen display all scores
			else
			{
				highScoreText.setPosition(sf::Vector2f(windowWidth - 250, -45));
				highScoreText.setCharacterSize(100);

				highScore << "High Scores" << std::endl;

				// print all high score
				for (int i = 0; i < gameController.gamesPlayed(); i++)
				{
					highScore << i + 1 << ":   " << gameController.getHighScore(i) << std::endl;
				}
			}

			highScoreText.setString(highScore.str());
		}


		// ----- DRAW GAME OBJECTS -----
		window.clear();

		// map
		window.draw(map, &environment.getTexture());

		// enemies
		for (int i = 0; i < enemyController.enemyCount(); i++)
		{
			window.draw(enemyController.getEnemy(i).getShape());
		}

		// player
		window.draw(player.getShape());

		// puck up items
		for (int i = 0; i < items.itemCount(); i++)
		{
			if (!items.item(i).isCollected())
			{
				window.draw(items.item(i).getShape());
			}
		}

		// extra life
		for (int i = 0; i < items.lifeCount(); i++)
		{
			if (!items.getLife(i).isCollected())
			{
				window.draw(items.getLife(i).getShape());
			}
		}

		// main stats
		window.draw(hud);

		// high scores
		window.draw(highScoreText);

		// continue game / game over
		if(gameController.gameOverStatus() || !player.getVunerability())
			window.draw(gameStateDisplay);
		
		// display starting game instructions
		if (!gameController.gameStartedStatus())
		{
			window.draw(textBoxOuter);
			window.draw(textBoxInner);
			window.draw(gameInstructions);
			window.draw(demoSquare);
			window.draw(demoTriangle);
			window.draw(demoPenta);
		}
		
		window.display();
	}

	return 0;
}