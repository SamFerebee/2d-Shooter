#include "BulletClass.h"

void BulletClass::moveBullet(float deltaTime, float multiplier)
{
	theBullet.move(currentVelocity * deltaTime * multiplier);
}

void BulletClass::setMaxSpeed(float speed)
{
	maxSpeed = speed;
}