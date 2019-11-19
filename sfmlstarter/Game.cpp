// Author: Dr Noel O'Hara
// SFML Starter for an awesome game you can make!
#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#endif 

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
	sf::Vector2f velocity = { 0,0 };
	float playerMaxSpeed = 500.0f;
	sf::Vector2f gravity{ 0.0f, 0.0f };
	float m_rotationForce{ 0.0f };
	bool m_debugging{ false };

	sf::Vector2f normalised;
	

	Game()
	{
		window.create(sf::VideoMode(800, 600), "Untitled Physics Games");
	}
	
	void init()
	{
		m_ground.setSize({ 800.0f, 600.0f - GROUND_POSITION });
		m_ground.setPosition(0.0f, GROUND_POSITION);
		m_ground.setFillColor(sf::Color::Green);

		// Sprites
		if (!m_slingTexture.loadFromFile("images/sling.png"))
		{
			std::cout << "Error loading slingshot sprite" << std::endl;
		}

		if (!m_birdTexture.loadFromFile("images/birds.png"))
		{
			std::cout << "Error loading bird sprite" << std::endl;
		}

		m_slingSprite.setTexture(m_slingTexture);
		m_slingSprite.setPosition(LAUNCH_POSITION.x - m_slingSprite.getGlobalBounds().width / 2.0f, LAUNCH_POSITION.y - 20.0f);

		m_birdSprite.setTexture(m_birdTexture);
		m_birdSprite.setTextureRect({ 62, 0, 62, 61 });
		m_birdSprite.setPosition(LAUNCH_POSITION);
		m_birdSprite.setOrigin({ m_birdSprite.getGlobalBounds().width / 2.0f + 5.0f, m_birdSprite.getGlobalBounds().height / 2.0f + 5.0f });

		m_birdBounds.setRadius(m_birdSprite.getGlobalBounds().width / 2.0f - 5.0f);
		m_birdBounds.setOrigin(m_birdBounds.getRadius(), m_birdBounds.getRadius());
		m_birdBounds.setFillColor(sf::Color{ 0, 255, 0, 100 });

		view = window.getDefaultView();

		sf::Vector2f normalised = thor::unitVector(m_birdSprite.getPosition());
		float l = thor::length(gravity);
		float r = thor::toDegree(10.0);
		
		thor::setLength(normalised, 100.0f);
		thor::rotate(normalised, 10.0f);
		playerState = ready;

		m_currentViewport = window.getDefaultView();

		m_obstacle.setSize({ 50.0f,50.0f });
		m_obstacle.setFillColor(sf::Color{ 100, 100, 50 });

		for (int i = 0; i < NUM_OBSTACLES; i++)
		{
			m_obstaclePositions[i] = rand() % (200 * NUM_OBSTACLES) + static_cast<int>(LAUNCH_POSITION.x);
		}
	}

	void run()
	{
	
		sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);


		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		
		sf::Clock clock;

		clock.restart();

		while (window.isOpen())
		{
			
			sf::Event event;
			while (window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window.close();
				if (event.type == sf::Event::KeyPressed)
				{
					if (sf::Keyboard::D == event.key.code)
					{
						m_debugging = !m_debugging;
					}
				}
			}

			
			timeSinceLastUpdate += clock.restart();

			if (timeSinceLastUpdate > timePerFrame)
			{

				if (playerState == ready)
				{
					if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						playerState = drawingLine;
					}
				}
				if (playerState == drawingLine)
				{
					if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
					{
						sf::Vector2f localPosition = (sf::Vector2f)sf::Mouse::getPosition(window);
						sf::Vector2f newVelocity;
						newVelocity =  (LAUNCH_POSITION - localPosition) * 3.0f;

						if (thor::length(newVelocity) < playerMaxSpeed)
						{
							velocity = newVelocity;
							std::cout << "Speed: " << thor::length(newVelocity) << std::endl;
						}
						else {
							thor::setLength(newVelocity, playerMaxSpeed);
							velocity = newVelocity;
							std::cout << "Max Speed: " << thor::length(newVelocity) << std::endl;
						}

						playerState = Jump;
						gravity.y = 9.8 * gravityScale;
						m_birdSprite.setTextureRect({ 0, 0, 62, 61 });
					}
					else
					{
						sf::Vector2f localPosition = (sf::Vector2f)sf::Mouse::getPosition(window);
						m_birdSprite.setPosition(localPosition);
						gravity = { 0.0f, 0.0f };

						sf::Vector2f newVelocity = (LAUNCH_POSITION - localPosition);

						if (thor::length(newVelocity) > playerMaxSpeed / 3.0f)
						{
							
							sf::Vector2f newPosition = LAUNCH_POSITION - (thor::unitVector(newVelocity) * (playerMaxSpeed / 3.0f));
							m_birdSprite.setPosition(newPosition);
						}
					}
				}
				if (playerState == Jump)
				{
					if (m_birdSprite.getPosition().x > 400.0f)
					{
						m_currentViewport.setCenter({ m_birdSprite.getPosition().x, 300.0f });
						window.setView(m_currentViewport);
					}

					if (m_birdBounds.getPosition().y > GROUND_POSITION - m_birdBounds.getRadius())
					{
						m_rotationForce = velocity.x;
						m_birdSprite.setPosition(m_birdSprite.getPosition().x, GROUND_POSITION - m_birdBounds.getRadius());
						velocity.y = -velocity.y / 2.0f;

						velocity.x *= 0.98f;

						if (velocity.x < 1.0f)
						{
							gravity.y = 0;
							velocity = { 0.0f, 0.0f };
							m_rotationForce = 0.0f;
							playerState = ready;
							m_birdSprite.setPosition(LAUNCH_POSITION);
							m_birdSprite.setRotation(0.0f);
							window.setView(window.getDefaultView());
							m_birdSprite.setTextureRect({ 62, 0, 62, 61 });
						}
					}
				}
				
				velocity = velocity + (gravity * timeSinceLastUpdate.asSeconds());
				
				m_birdSprite.move(velocity.x * timeSinceLastUpdate.asSeconds(), velocity.y * timeSinceLastUpdate.asSeconds());
				m_birdSprite.rotate(m_rotationForce * timeSinceLastUpdate.asSeconds());

				m_birdBounds.setPosition(m_birdSprite.getPosition());

				window.clear(sf::Color{100,100,255});

				if (playerState == drawingLine)
				{
					sf::Vector2i localPosition = sf::Mouse::getPosition(window);
					sf::Vertex line[] =
					{
						sf::Vertex(sf::Vector2f(m_birdSprite.getPosition())),
						sf::Vertex(sf::Vector2f(LAUNCH_POSITION))
					};

					window.draw(line, 2, sf::Lines);
				}
				
				m_ground.setPosition(window.getView().getCenter().x - m_ground.getGlobalBounds().width / 2.0f, GROUND_POSITION);
				window.draw(m_ground);

				for (int i = 0; i < NUM_OBSTACLES; i++)
				{
					m_obstacle.setPosition(static_cast<float>(m_obstaclePositions[i]), GROUND_POSITION - 50.0f);
					window.draw(m_obstacle);
				}

				window.draw(m_slingSprite);

				window.draw(m_birdSprite);

				if (m_debugging)
				{
					window.draw(m_birdBounds);
				}
				
				window.display();

				
				timeSinceLastUpdate = sf::Time::Zero;
			}
		}
	}
};


int main()
{
	srand(static_cast<unsigned>(time(nullptr)));

	Game game;
	

	game.init();

	game.run();



	return 0;
}