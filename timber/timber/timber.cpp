// helloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <sstream>
// include SFML properties
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>

using namespace sf;

//Function Declaration

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
//where is the player /branch
enum class side{LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];
int main()
{
    std::cout << "Hello World!\n";

	//Create a Video Mode Object
	VideoMode vm(1920, 1080);
	//create and open a window
	RenderWindow window(vm, "Timber!!!!", Style::Fullscreen);

	// Create a texture to hole graphic
	Texture textureBackground;

	//load graphics into texture.
	textureBackground.loadFromFile("graphics/background.png");

	// create sprite;

	Sprite spriteBackground;
	//Attach texture
	spriteBackground.setTexture(textureBackground);

	spriteBackground.setPosition(0, 0);

	// Make Trees
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree1;
	spriteTree1.setTexture(textureTree);
	spriteTree1.setPosition(810, 0);
	
	// to add to background
	Texture textureTree2;
	textureTree2.loadFromFile("graphics/tree2.png");

	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
	spriteTree2.setPosition(0, 0);

	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree2);
	spriteTree3.setPosition(0, 200);

	Sprite spriteTree4;
	spriteTree4.setTexture(textureTree2);
	spriteTree4.setPosition(1500, -100);

	Sprite spriteTree5;
	spriteTree5.setTexture(textureTree2);
	spriteTree5.setPosition(810, 0);


	// Prepare Bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);

	//is bee moving
	bool beeActive = false;

	float beeSpeed = 0.0f;

	// clouds
	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;

	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);

	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(0, 250);
	spriteCloud3.setPosition(0, 500);

	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	//variables to control time
	Clock clock;

	//Time bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Green);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining =6.0f;
	float timeBArWidhtPerSecond = timeBarStartWidth / timeRemaining;


	bool paused = true;

	//DRAW text
	int score = 0;
	sf::Text messageText;
	sf::Text scoreText;
	
	//choose font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	// set font to message
	messageText.setFont(font);
	scoreText.setFont(font);

	// Assign actual message
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score = 0 ");
	
	//MAke it BIG
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(100);

	//Choose Color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	
	//Position the Text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);


	//Prepare Branches
	Texture textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	//set Texture to each branch sprite
	for (int i = 0; i < NUM_BRANCHES; i++) {
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -20000);
		//set the sprites. origin to dead center
		// we can teh spin it around without changing its position
		branches[i].setOrigin(220, 20);
	}

	// Prepare the player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(580,720);
	
	side playerSide = side::LEFT;
	
	// Prepare Gravestone
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip1.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(600, 800);

	//Prepare Axe

	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 810);

	// line up the axe with the tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	// Prepare flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);


	//other usefull variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	//comntrol input
	bool acceptInput = true;

	// Prepare sound;

	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);

	while (window.isOpen()){
		/*************HANDLE INPUT******************************/

		// Detect key Release
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::KeyReleased && !paused) {
				// accept inputs
				acceptInput = true;
					// hide axe
					spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		if (Keyboard::isKeyPressed(Keyboard::Return) ){
			paused=!paused;
			// reset time and socre
			score = 0;
			timeRemaining = 5;

			// make all branches disappear
			for (int i = 1; i < NUM_BRANCHES; i++) {
				branchPositions[i] = side::NONE;
			}
			
			// hide gravestone
			spriteRIP.setPosition(675, 2000);

			// move the player into position
			spritePlayer.setPosition(580, 720);
			acceptInput = true;


		}

		// wrap the player control to make 
		//sure we are acceptind input
		if (acceptInput) {
			// handle right key press
			if (Keyboard::isKeyPressed(Keyboard::Right)) {
				// player goes to right
				playerSide = side::RIGHT;
				score++;

				// add to the amount of time remaining
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_RIGHT,
					spriteAxe.getPosition().y);
				spritePlayer.setPosition(1200, 720);
				// update Branches
				updateBranches(score);

				//send the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;

				acceptInput = false;

				//play sound
				chop.play();
			}
			// handle left key press
			if (Keyboard::isKeyPressed(Keyboard::Left)) {
				// player goes to right
				playerSide = side::LEFT;
				score++;

				// add to the amount of time remaining
				timeRemaining += (2 / score) + .15;
				spriteAxe.setPosition(AXE_POSITION_LEFT,
					spriteAxe.getPosition().y);
				spritePlayer.setPosition(580, 720);
				// update Branches
				updateBranches(score);

				//send the log flying to the left
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;

				acceptInput = false;
				//play sound
				chop.play();
			}

		}

		/*************UPDATE SCENE***************************/
		if (!paused) {
			//Measure time
			Time dt = clock.restart();
			//subtract amount of time remaining
			timeRemaining -= dt.asSeconds();
			timeBar.setSize(Vector2f(timeBArWidhtPerSecond * timeRemaining, timeBarHeight));

			if (timeRemaining <= 2.0f)
				timeBar.setFillColor(Color::Red);
			else
				timeBar.setFillColor(Color::Green);

			if (timeRemaining <= 0.0f) {
				paused = true;//pause game
				//change message shown to the player
				messageText.setString("Out of Time!!");
				//Reposition the text based on new size
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				timeBar.setFillColor(Color::Green);
				//play sound
				outOfTime.play();
			}
				//setup bee;

				if (!beeActive) {
					//speed
					srand((int)time(0) * 10);
					beeSpeed = (rand() % 250) + 200;
					//height
					srand((int)time(0) * 10);
					float height = (rand() % 500) + 500;
					spriteBee.setPosition(2000, height);
					beeActive = true;
				}
				else {
					//move the bee
					spriteBee.setPosition(spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),spriteBee.getPosition().y + (rand()% 11 - 5)  );///HEREEEEEEEEEEEEEEEEEEEEEEE
					if (spriteBee.getPosition().x < -100 || spriteBee.getPosition().y< -100) {
						beeActive = false;
					}
				}

			//MANAGE CLOUDS
			// cloud 1
			if (!cloud1Active) {
				// how fast is the cloud
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);
				//height
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(-200, height);
				cloud1Active = true;
			}
			else {
				spriteCloud1.setPosition(spriteCloud1.getPosition().x +
					(cloud1Speed * dt.asSeconds()), spriteCloud1.getPosition().y);
				if (spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false;
				}
			}
			// cloud 2
			if (!cloud2Active) {
				// how fast is the cloud
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 200);
				//height
				srand((int)time(0) * 20);
				float height = (rand() % 300) - 150;
				spriteCloud2.setPosition(-200, height);
				cloud2Active = true;
			}
			else {
				spriteCloud2.setPosition(spriteCloud2.getPosition().x +
					(cloud2Speed * dt.asSeconds()), spriteCloud2.getPosition().y);
				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}

			// cloud 3
			if (!cloud3Active) {
				// how fast is the cloud
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 200);
				//height
				srand((int)time(0) * 30);
				float height = (rand() % 450) - 150;
				spriteCloud3.setPosition(-200, height);
				cloud3Active = true;
			}
			else {
				spriteCloud3.setPosition(spriteCloud3.getPosition().x +
					(cloud3Speed * dt.asSeconds()), spriteCloud3.getPosition().y);
				if (spriteCloud3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}
			//update score text
			std::stringstream ss;
			ss << "Score = " << score;
			scoreText.setString(ss.str());

			//update the branches
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPositions[i] == side::LEFT) {
					//move sprite to left
					branches[i].setPosition(610, height);
					// flip the sprite
					branches[i].setRotation(180);
				}
				else if (branchPositions[i] == side::RIGHT) {
					// move the sprite to the right side
					branches[i].setPosition(1330, height);
					//set sprite to normal
					branches[i].setRotation(0);
				}
				else {
					//hide branch
					branches[i].setPosition(3000, height);
				}
			}

			// Handle the Flying log
			if (logActive) {
				spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
					spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));
				// has the log reached  edge
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().x >2000) {
					logActive = false;
					spriteLog.setPosition(810, 720);
				}


			}
            // has hte player been squished by a branch    
			if (branchPositions[5] == playerSide) {
				// death
				paused = true;
				acceptInput = false;
				
				// draw the graveStone
				spriteRIP.setPosition(525, 760);
				
				//hide the player
				spritePlayer.setPosition(2000, 660);
				
				// hide axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
				//change the text of the message
				messageText.setString("Squished!!");
				//center it on the screen
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f, 
					textRect.top + textRect.height / 2.0f);
				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

				//play sound
				death.play();
			}

		}//END IF

		/*
		************DRAW SCENE*****************************
		*/
		//clear everything from the last frame
		window.clear();

		// DRAW scene

		window.draw(spriteBackground);//must be drawn vefore everything
		
		
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);
		
		// draw branches
		for (int i = 0; i < NUM_BRANCHES; i++) {
			window.draw(branches[i]);
		}
		
		window.draw(spriteTree1);

		//window.draw(spriteTree2);

//		window.draw(spriteTree3);

	//	window.draw(spriteTree4);

	//	window.draw(spriteTree5);



		window.draw(spritePlayer);
		window.draw(spriteAxe);
		
		window.draw(spriteLog);
		
		window.draw(spriteRIP);
		
		window.draw(timeBar);
		


		window.draw(spriteBee);

		// draw score
		window.draw(scoreText);
		if (paused) {
			window.draw(messageText);
		}

		//show everything just drawn
		window.display();

	}


	return 0;
}


void updateBranches(int seed) {
	// move all one place down
	for (int i = NUM_BRANCHES - 1; i >0; i--) {
		branchPositions[i] = branchPositions[i - 1];
	}
	//spawn a new branch at positon 0
	srand((int)time(0) + seed);
	int r=(rand() % 5);
	switch (r) {
	case 0:
		branchPositions[0] = side::LEFT;
		break;
	case 1:
		branchPositions[0] = side::RIGHT;
		break;
	default:
		branchPositions[0] = side::NONE;
		break;
	}
}
// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
