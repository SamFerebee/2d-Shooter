#pragma once
#include "Entity.h"

using namespace sf;


class Enemy : public Entity
{
private:
	int pointValue;
	Vector2f thePlayerPosition;
	Clock attackTimer;
public:

};