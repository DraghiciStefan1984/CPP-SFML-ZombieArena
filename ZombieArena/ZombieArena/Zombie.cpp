#include "pch.h"
#include "Zombie.h"
#include "TextureHolder.h"
#include <cstdlib>
#include <ctime>

using namespace std;

Zombie::Zombie(){}

Zombie::~Zombie(){}

bool Zombie::Hit()
{
	mHealth--;
	if (mHealth < 0)
	{
		mAlive = false;
		mSprite.setTexture(TextureHolder::GetTexture("graphics/blood.png"));
		return true;
	}
	return false;
}

bool Zombie::IsAlive()
{
	return mAlive;
}

void Zombie::Spawn(float startX, float startY, int type, int seed)
{
	switch (type)
	{
	case 0:
		mSprite = Sprite(TextureHolder::GetTexture("graphics/bloater.png"));
		mSpeed = 40;
		mHealth = 5;
		break;

	case 1:
		mSprite = Sprite(TextureHolder::GetTexture("graphics/chaser.png"));
		mSpeed = 70;
		mHealth = 1;
		break;

	case 2:
		mSprite = Sprite(TextureHolder::GetTexture("graphics/crawler.png"));
		mSpeed = 20;
		mHealth = 3;
		break;
	}

	srand((int)time(0)*seed);
	float modifier = (rand() % MAX_VARRIANCE) + OFFSET;
	modifier /= 100;
	mSpeed *= modifier;
	mPosition.x = startX;
	mPosition.y = startY;
	mSprite.setOrigin(25, 25);
	mSprite.setPosition(mPosition);
}

FloatRect Zombie::GetPosition()
{
	return mSprite.getGlobalBounds();
}

Sprite Zombie::GetSprite()
{
	return mSprite;
}

void Zombie::Update(float elapsedTime, Vector2f playerLocation)
{
	float playerX = playerLocation.x;
	float playerY = playerLocation.y;

	if (playerX > mPosition.x)
	{
		mPosition.x += (mSpeed*elapsedTime);
	}
	if (playerY > mPosition.y)
	{
		mPosition.y += (mSpeed*elapsedTime);
	}

	mSprite.setPosition(mPosition);
	float angle = (atan2(playerY - mPosition.y, playerX - mPosition.x) * 180) / 3.141;
	mSprite.setRotation(angle);
}