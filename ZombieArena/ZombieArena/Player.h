#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	const float START_SPEED = 200;
	const float START_HEALTH = 100;

	Vector2f mPosition;
	Sprite mSprite;
	Texture mTexture;
	Vector2f mResolution;
	IntRect mArena;
	int mTileSize;
	bool mUpPressed;
	bool mDownPressed;
	bool mLeftPressed;
	bool mRightPressed;
	int mHealth;
	int mMaxHealth;
	Time mLastHit;
	float mSpeed;

public:
	Player();
	void Spawn(IntRect arena, Vector2f resolution, int tileSize);
	bool Hit(Time timeHit);
	Time GetLastHitTime();
	FloatRect GetPosition();
	Vector2f GetCenter();
	float GetRotation();
	Sprite GetSprite();
	int GetHealth();
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
};