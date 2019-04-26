#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet
{
private:
	Vector2f mPosition;
	RectangleShape mBulletShape;
	bool mInFlight = false;
	float mBulletSpeed = 1000;
	float mBulletDistanceX;
	float mBulletDistanceY;
	float mMinX;
	float mMaxX;
	float mMinY;
	float mMaxY;

public:
	Bullet();
	~Bullet();

	void Stop();
	bool IsInFlight();
	void Shoot(float startX, float startY, float targetX, float targetY);
	FloatRect GetPosition();
	RectangleShape GetShape();
	void Update(float elapsedTime);
};