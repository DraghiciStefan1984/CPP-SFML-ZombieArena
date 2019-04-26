// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "ZombieArena.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"
#include <sstream>

using namespace sf;
using namespace std;

int main()
{
	TextureHolder textureHolder;
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	State state = State::GAME_OVER;
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen);
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));
	Clock clock;
	Time gameTimeTotal;
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;
	Player player;
	IntRect arena;
	VertexArray background;
	Texture textureBackground=TextureHolder::GetTexture("graphics/background_sheet.png");
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = nullptr;
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int clipSize = 6;
	float fireRate = 1;
	Time lastPressed;

	window.setMouseCursorVisible(false);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(25, 25);

	Pickup healthPickup(1);
	Pickup ammoPickup(2);

	int score = 0;
	int hiScore = 0;

	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/background.jpg");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(20, 980);

	Font font;
	font.loadFromFile("fonts/zombiecontrol.ttf");

	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press Enter \nto continue");

	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Enter to play");

	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250);
	std::stringstream levelUpStream;
	levelUpStream <<
		"1- Increased rate of fire" <<
		"\n2- Increased clip size(next reload)" <<
		"\n3- Increased max health" <<
		"\n4- Increased run speed" <<
		"\n5- More and better health pickups" <<
		"\n6- More and better ammo pickups";
	levelUpText.setString(levelUpStream.str());

	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(55);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(200, 980);

	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(20, 0);

	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::White);
	hiScoreText.setPosition(1400, 0);
	std::stringstream s;
	s << "Hi Score:" << hiScore;
	hiScoreText.setString(s.str());

	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(1500, 980);
	zombiesRemainingText.setString("Zombies: 100");

	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(1250, 980);
	waveNumberText.setString("Wave: 0");

	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setPosition(450, 980);

	int framesSinceLastHUDUpdate = 0;
	int fpsMeasurementFrameInterval = 1000;

	// The main game loop
	while (window.isOpen())
	{
		// Handle events
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				// Pause a game while playing
				if (event.key.code == Keyboard::Return && state == State::PLAYING) state = State::PAUSED;

				// Restart while paused
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					clock.restart();
				}
				// Start a new game while in GAME_OVER state
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) state = State::LEVELING_UP;

				if (state == State::PLAYING)
				{
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= clipSize)
						{
							bulletsInClip = clipSize;
							bulletsSpare -= clipSize;
						}
						else if (bulletsSpare > 0)
						{
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
						}
						else
						{

						}
					}
				}
			}
		}// End event polling

		// Handle the player quitting
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}
		// Handle controls while playing
		if (state == State::PLAYING)
		{
			// Handle the pressing and releasing of the WASD keys
			if (Keyboard::isKeyPressed(Keyboard::W)) player.MoveUp();
			else player.StopUp();

			if (Keyboard::isKeyPressed(Keyboard::S)) player.MoveDown();
			else player.StopDown();

			if (Keyboard::isKeyPressed(Keyboard::A)) player.MoveLeft();
			else player.StopLeft();

			if (Keyboard::isKeyPressed(Keyboard::D)) player.MoveRight();
			else player.StopRight();

			//fire bullet
			if (Mouse::isButtonPressed(Mouse::Left))
			{
				if (gameTimeTotal.asMicroseconds() - lastPressed.asMicroseconds() > 1000 / fireRate && bulletsInClip > 0)
				{
					bullets[currentBullet].Shoot(player.GetCenter().x, player.GetCenter().y, mouseWorldPosition.x, mouseWorldPosition.y);
					currentBullet++;
					if (currentBullet > 99) currentBullet = 0;
					lastPressed = gameTimeTotal;
					bulletsInClip--;
				}
			}//end fire bullet
		}// End WASD while playing

		// Handle the levelling up state
		if (state == State::LEVELING_UP)
		{
			// Handle the player levelling up
			if (event.key.code == Keyboard::Num1) state = State::PLAYING;
			if (event.key.code == Keyboard::Num2) state = State::PLAYING;
			if (event.key.code == Keyboard::Num3) state = State::PLAYING;
			if (event.key.code == Keyboard::Num4) state = State::PLAYING;
			if (event.key.code == Keyboard::Num5) state = State::PLAYING;
			if (event.key.code == Keyboard::Num6) state = State::PLAYING;

			if (state == State::PLAYING)
			{
				arena.width = 500;
				arena.height = 500;
				arena.left = 0;
				arena.top = 0;
				int tileSize = CreateBackground(background, arena);
				player.Spawn(arena, resolution, tileSize);

				healthPickup.SetArena(arena);
				ammoPickup.SetArena(arena);

				numZombies = 10;
				delete[] zombies;
				zombies = CreateHorde(numZombies, arena);
				numZombiesAlive = numZombies;
				clock.restart();
			}
		}// End levelling up

		//update the frame
		if (state == State::PLAYING)
		{
			Time deltaTime = clock.restart();
			gameTimeTotal += deltaTime;
			float deltaTimeAsSeconds = deltaTime.asSeconds();
			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
			spriteCrosshair.setPosition(mouseWorldPosition);
			player.Update(deltaTimeAsSeconds, Mouse::getPosition());
			Vector2f playerPosition(player.GetCenter());
			mainView.setCenter(player.GetCenter());
			
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].IsAlive())
				{
					zombies[i].Update(deltaTime.asSeconds(), playerPosition);
				}
			}

			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].IsInFlight()) bullets[i].Update(deltaTimeAsSeconds);
			}

			healthPickup.Update(deltaTimeAsSeconds);
			ammoPickup.Update(deltaTimeAsSeconds);

			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].IsInFlight() && zombies[j].IsAlive())
					{
						if (bullets[i].GetPosition().intersects(zombies[j].GetPosition()))
						{
							bullets[i].Stop();

							if (zombies[j].Hit())
							{
								score += 10;
								if (score > hiScore) hiScore = score;
								numZombiesAlive--;
								if (numZombiesAlive == 0) state = State::LEVELING_UP;
							}
						}
					}
				}
			}

			for (int i = 0; i < numZombies; i++)
			{
				if (player.GetPosition().intersects(zombies[i].GetPosition()) && zombies[i].IsAlive())
				{
					if (player.Hit(gameTimeTotal))
					{

					}

					if (player.GetHealth() <= 0) state = State::GAME_OVER;
				}
			}

			if (player.GetPosition().intersects(healthPickup.GetPosition()) && healthPickup.IsSapwned()) player.IncreaseHealthLevel(healthPickup.GotIt());
			if (player.GetPosition().intersects(ammoPickup.GetPosition()) && ammoPickup.IsSapwned()) bulletsSpare += ammoPickup.GotIt();

			healthBar.setSize(Vector2f(player.GetHealth() * 3, 70));
			framesSinceLastHUDUpdate++;

			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval)
			{
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;

				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				ssHiScore << "Hi Score:" << hiScore;
				hiScoreText.setString(ssHiScore.str());

				ssWave << "Wave:" << wave;
				waveNumberText.setString(ssWave.str());

				ssZombiesAlive << "Zombies:" << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				framesSinceLastHUDUpdate = 0;
			}
		}// End updating the scene

		//draw scene
		if (state == State::PLAYING)
		{
			window.clear();

			// set the mainView to be displayed in the window
			// And draw everything related to it
			window.setView(mainView);

			// Draw the background
			window.draw(background, &textureBackground);

			// Draw the zombies
			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].GetSprite());
			}

			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].IsInFlight())
				{
					window.draw(bullets[i].GetShape());
				}
			}

			// Draw the player
			window.draw(player.GetSprite());

			// Draw the pickups is currently spawned
			if (ammoPickup.IsSapwned())
			{
				window.draw(ammoPickup.GetSprite());
			}
			if (healthPickup.IsSapwned())
			{
				window.draw(healthPickup.GetSprite());
			}

			//Draw the crosshair
			window.draw(spriteCrosshair);

			// Switch to the HUD view
			window.setView(hudView);

			// Draw all the HUD elements
			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}

		if (state == State::LEVELING_UP)
		{
			window.draw(spriteGameOver);
			window.draw(levelUpText);
		}

		if (state == State::PAUSED)
		{
			window.draw(pausedText);
		}

		if (state == State::GAME_OVER)
		{
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
		}

		window.display();
	}// End game loop
	delete[] zombies;
	return 0;
}