#include "Entity.h"

void Entity::initEntity()
{
	//initSprite();
	initPhysics();
}

void Entity::initSprite()
{
	//entitySprite.setTexture(entityTextureSheet);
	currentFrame = IntRect(10, 16, 68, 70);
	entitySprite.setTextureRect(currentFrame);
}

void Entity::initPhysics()
{
	maxVelocity = 4.f;
	acceleration = .4f;
	drag = 0.2f;
	multiplier = 153.f;
	minVelocity = 1.f;
	gravity = 1.f;
	maxGravity = 30.f;
	minGravity = .5f;
}

void Entity::updatePhysics()
{
	//gravity
	currentVelocity.y += 15.f * gravity * deltaTime * multiplier;
	if (std::abs(currentVelocity.y) > maxGravity)
	{
		currentVelocity.y = maxGravity * ((currentVelocity.y < 0.f) ? -1.f : 1.f);
	}

	//deceleration
	currentVelocity *= drag * deltaTime * multiplier;
	if (std::abs(currentVelocity.x) < minVelocity)
		currentVelocity.x = 0.f;
	if (std::abs(currentVelocity.y) < minGravity)
		currentVelocity.y = 0.f;
	entitySprite.move(currentVelocity.x, currentVelocity.y);
}

void Entity::updateEntity()
{
	updatePhysics();
}

void Entity::setTexture(Texture incTexture)
{
	entityTextureSheet = incTexture;

}

void Entity::setDeltaTime(float time)
{
	deltaTime = time;
}

void Entity::setWindowSize(float x, float y)
{
	windowSizeX = x;
	windowSizeY = y;
}