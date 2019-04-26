#include "pch.h"
#include "Bullet.h"

using namespace sf;

Bullet::Bullet()
{
	mBulletShape.setSize(Vector2f(2, 2));
}

Bullet::~Bullet()
{
}

void Bullet::Stop()
{
	mInFlight = false;
}

bool Bullet::IsInFlight()
{
	return mInFlight;
}

void Bullet::Shoot(float startX, float startY, float targetX, float targetY)
{
	mInFlight = true;
	mPosition.x = startX;
	mPosition.y = startY;
	float gradient = (startX - targetX) / (startY - targetY);

	if (gradient < 0) gradient *= -1;

	float ratioXY = mBulletSpeed / (1 + gradient);
	mBulletDistanceX = ratioXY * gradient;
	mBulletDistanceY = ratioXY;
	
	if (targetX < startX) mBulletDistanceX *= -1;
	if (targetY < startY) mBulletDistanceY *= -1;

	float range = 1000;
	mMinX = startX - range;
	mMaxX = startX + range;
	mMinY = startY - range;
	mMaxY = startY + range;
	mBulletShape.setPosition(mPosition);
}

FloatRect Bullet::GetPosition()
{
	return mBulletShape.getGlobalBounds();
}

RectangleShape Bullet::GetShape()
{
	return mBulletShape;
}

void Bullet::Update(float elapsedTime)
{
	mPosition.x += mBulletDistanceX * elapsedTime;
	mPosition.y += mBulletDistanceY * elapsedTime;
	mBulletShape.setPosition(mPosition);

	if (mPosition.x<mMinX || mPosition.x>mMaxX || mPosition.y<mMinY || mPosition.y>mMaxY) mInFlight = false;
}
