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
	void spawn(IntRect arena, Vector2f resolution, int tileSize);
	bool hit(Time timeHit);
	Time getLastHitTime();
	FloatRect getPosition();
	Vector2f getCenter();
	float getRotation();
	Sprite getSprite();
	int getHealth();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopLeft();
	void stopRight();
	void stopUp();
	void stopDown();
	void update(float elapsedTime, Vector2i mousePosition);
	void upgradeSpeed();
	void upgradeHealth();
	void increaseHealthLevel(int amount);
};