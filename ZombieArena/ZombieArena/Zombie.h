#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Zombie
{
private:
	const float BLOATER_SPEED = 40;
	const float CHASER_SPEED = 80;
	const float CRAWLER_SPEED = 20;
	const float BLOATER_HEALTH = 5;
	const float CHASER_HEALTH = 1;
	const float CRAWLER_HEALTH = 3;
	const int MAX_VARRIANCE = 30;
	const int OFFSET = 101 - MAX_VARRIANCE;

	Vector2f mPosition;
	Sprite mSprite;
	float mSpeed;
	float mHealth;
	bool mAlive;

public:
	Zombie();
	~Zombie();

	bool Hit();
	bool IsAlive();
	void Spawn(float startX, float startY, int type, int seed);
	FloatRect GetPosition();
	Sprite GetSprite();
	void Update(float elapsedTime, Vector2f playerLocation);
};

