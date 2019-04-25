#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	float mSpeed;
	int mTileSize;
	int mHealth;
	int mMaxHealth;
	bool mUpPressed;
	bool mDownPressed;
	bool mLeftPressed;
	bool mRightPressed;

	Vector2f mPosition;
	Sprite mSprite;
	Texture mTexture;
	Vector2f mResolution;
	IntRect mArena;
	Time mLastHit;

public:
	Player();
	~Player();
	void Spawn(IntRect arena, Vector2f resolution, int tileSize);
	void ResetPlayerStats();
	bool Hit(Time timeHit);
	Time GetLastHitTime();
	FloatRect GetPosition();
	Vector2f GetCenter();
	float GetRotation();
	Sprite GetSprite();
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	void StopLeft();
	void StopRight();
	void StopUp();
	void StopDown();
	void Update(float elapsedTime, Vector2i mousePosition);
	void UpgradeSpeed();
	void UpgradeHealth();
	void IncreaseHealthLevel(int amount);
	int getHealth();
};