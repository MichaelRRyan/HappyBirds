#pragma once

// Author: Dr Noel O'Hara
// SFML Starter for an awesome game you can make!


#include <SFML/Graphics.hpp>

// I use a couple of h files from thor library.
//https://github.com/Bromeon/Thor
#include "VectorAlgebra2D.h"
#include <iostream>
#include <stdlib.h> 
#include <time.h> 
#include <list>

enum PlayerState { ready, drawingLine, Jump };

class Game
{
public:
	Game();
	void init();
	void run();

private:
	void restart();

	//create Window
	sf::RenderWindow window;
	sf::View view;
	float randomNum = 0;

	const sf::Vector2f LAUNCH_POSITION{ 200.0f, 362.0f };
	const float GROUND_POSITION{ 550 };

	static const int NUM_OBSTACLES{ 50 };

	// Shapes
	sf::RectangleShape m_ground;
	sf::RectangleShape m_obstacle;

	int m_obstaclePositions[NUM_OBSTACLES];

	// Sprites
	sf::Texture m_slingTexture;
	sf::Texture m_birdTexture;

	sf::Sprite m_slingSprite;
	sf::Sprite m_birdSprite;

	sf::CircleShape m_birdBounds;

	sf::View m_currentViewport;

	//sf::Vector2f moveForce;
	PlayerState playerState = ready;
	float gravityScale = 20;
	float playerMaxSpeed = 500.0f;
	float m_rotationForce{ 0.0f };

	sf::Vector2f velocity = { 0,0 };
	sf::Vector2f gravity{ 0.0f, 0.0f };

	bool m_debugging{ false };

	sf::Vector2f normalised;

	int m_score;

	sf::Font m_font;
	sf::Text m_scoreText;
};