// HelloSFML.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"

using namespace sf;

int main()
{
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };

	State state = State::GAME_OVER;
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width;
	resolution.y = VideoMode::getDesktopMode().height;
	RenderWindow window(VideoMode(resolution.x, resolution.y), "Zmobie Arena!!!", Style::Fullscreen);
	View mainView(FloatRect(0, 0, resolution.x, resolution.y));
	Clock clock;
	Time gameTimeTotal;
	Vector2f mouseWorldPosition;
	Vector2i mouseScreenPosition;
	Player player;
	IntRect arena;

	//main game loop
	while (window.isOpen())
	{
		Event event;

		//poll event
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyPressed)
			{
				if (event.key.code == Keyboard::Return && state == State::PLAYING)
				{
					state = State::PAUSED;
				}
				else if (event.key.code == Keyboard::Return && state == State::PAUSED)
				{
					state = State::PLAYING;
					clock.restart();
				}
				else if (event.key.code == Keyboard::Return && state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
				}
				if (state == State::PLAYING)
				{

				}
			}
		}//end poll event

		//player quit
		if (Keyboard::isKeyPressed(Keyboard::Escape)) window.close();

		//control while play
		if (state == State::PLAYING)
		{
			if (Keyboard::isKeyPressed(Keyboard::W)) player.MoveUp();
			else player.StopUp();

			if (Keyboard::isKeyPressed(Keyboard::S)) player.MoveDown();
			else player.StopDown();

			if (Keyboard::isKeyPressed(Keyboard::A)) player.MoveLeft();
			else player.StopLeft();

			if (Keyboard::isKeyPressed(Keyboard::D)) player.MoveRight();
			else player.StopRight();
		}//end control while play

		//leveling up
		if (state == State::LEVELING_UP)
		{
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
				int tileSize = 50;
				player.Spawn(arena, resolution, tileSize);
				clock.restart();
			}
		}//end leveling up

		//update frame
		if (state == State::PLAYING)
		{
			Time deltaTime = clock.restart();
			gameTimeTotal += deltaTime;
			float deltaTimeAsSeconds = deltaTime.asSeconds();
			mouseScreenPosition = Mouse::getPosition();
			mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);
			player.Update(deltaTimeAsSeconds, Mouse::getPosition());
			Vector2f playerPosition(player.GetCenter());
			mainView.setCenter(player.GetCenter());
		}//end update frame

		//draw scene
		if (state == State::PLAYING)
		{
			window.clear();
			window.setView(mainView);
			window.draw(player.GetSprite());
		}// end draw scene

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
	}//end main game loop
	
	return 0;
}
