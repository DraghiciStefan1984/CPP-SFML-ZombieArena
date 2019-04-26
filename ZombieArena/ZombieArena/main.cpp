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

		}// End updating the scene

		//draw scene
		if (state == State::PLAYING)
		{
			window.clear();
			window.draw(background, &textureBackground);
			window.setView(mainView);

			for (int i = 0; i < numZombies; i++)
			{
				window.draw(zombies[i].GetSprite());
			}

			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].IsInFlight()) window.draw(bullets[i].GetShape());
			}

			if (healthPickup.IsSapwned()) window.draw(healthPickup.GetSprite());
			if (ammoPickup.IsSapwned()) window.draw(ammoPickup.GetSprite());

			window.draw(player.GetSprite());
			window.draw(spriteCrosshair);
		}

		if (state == State::LEVELING_UP)
		{
		}

		if (state == State::PAUSED)
		{
		}

		if (state == State::GAME_OVER)
		{
		}

		window.display();
	}// End game loop
	delete[] zombies;
	return 0;
}