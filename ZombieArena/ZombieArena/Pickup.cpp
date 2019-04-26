#include "pch.h"
#include "Pickup.h"
#include "TextureHolder.h"

Pickup::Pickup(int type)
{
	mType = type;

	if (mType == 1)
	{
		mSprite = Sprite(TextureHolder::GetTexture("graphics/health_pickup.png"));
		mValue = HEALTH_START_VALUE;
	}
	else
	{
		mSprite = Sprite(TextureHolder::GetTexture("graphics/ammo_pickup.png"));
		mValue = AMMO_START_VALUE;
	}

	mSprite.setOrigin(25, 25);
	mSecondsToLive = START_SECONDS_TO_LIVE;
	mSecondsToWait = START_WAIT_TIME;
}

Pickup::~Pickup()
{
}

void Pickup::SetArena(IntRect arena)
{
	mArena.left = arena.left + 50;
	mArena.width = arena.width - 50;
	mArena.top = arena.top + 50;
	mArena.height = arena.height - 50;
	Spawn();
}

void Pickup::Spawn()
{
	srand((int)time(0) / mType);
	int x = (rand() % mArena.width);
	srand((int)time(0) * mType);
	int y = (rand() % mArena.height);
	mSecondsSinceSapwn = 0;
	mSpawned = true;
	mSprite.setPosition(x, y);
}

FloatRect Pickup::GetPosition()
{
	return mSprite.getGlobalBounds();
}

Sprite Pickup::GetSprite()
{
	return mSprite;
}

void Pickup::Update(float elapsedTime)
{
	if (mSpawned) mSecondsSinceSapwn += elapsedTime;
	else mSecondsSinceDeSapwn += elapsedTime;

	if (mSecondsSinceSapwn > mSecondsToLive && mSpawned)
	{
		mSpawned = false;
		mSecondsSinceDeSapwn = 0;
	}

	if (mSecondsSinceDeSapwn > mSecondsToWait && !mSpawned) Spawn();
}

bool Pickup::IsSapwned()
{
	return mSpawned;
}

int Pickup::GotIt()
{
	mSpawned = false;
	mSecondsSinceDeSapwn = 0;
	return mValue;
}

void Pickup::Upgrade()
{
	if (mType == 1) mValue += (HEALTH_START_VALUE * .5);
	else mValue += (AMMO_START_VALUE * .5);

	mSecondsToLive += (START_SECONDS_TO_LIVE / 10);
	mSecondsToLive -= (START_WAIT_TIME / 10);
}
