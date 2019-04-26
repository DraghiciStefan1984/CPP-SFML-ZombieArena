#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Pickup
{
private:
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	Sprite mSprite;
	IntRect mArena;
	int mValue;
	int mType;
	bool mSpawned;
	float mSecondsSinceSapwn;
	float mSecondsSinceDeSapwn;
	float mSecondsToLive;
	float mSecondsToWait;

public:
	Pickup(int type);
	~Pickup();

	void SetArena(IntRect arena);
	void Spawn();
	FloatRect GetPosition();
	Sprite GetSprite();
	void Update(float elapsedTime);
	bool IsSapwned();
	int GotIt();
	void Upgrade();
};