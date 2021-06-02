#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <memory>
#include "BulletClass.h"
#include "SoundFX.h"
#include "Collision.h"

using namespace sf;

struct UIStruct
{
	Font theFont;
	std::string healthString;
	std::string armorString;
	Text healthText;
	Text armorText;
	RectangleShape healthBar;
	RectangleShape armorBar;
	std::string currentPointsString;
	Text currentPointsText;
};

class Entity : public SoundFX
{
protected:
	int healthCurrent, healthMax;
	bool isDead = false;
	Sprite entitySprite;
	Texture entityTextureSheet;
	float speedMax;
	RectangleShape healthBar;
	std::string healthString;
	Clock animationClock;
	///window info
	float windowSizeX;
	float windowSizeY;
	//physics
	Vector2f currentVelocity;
	float maxVelocity, minVelocity, acceleration, drag, gravity, maxGravity, minGravity;
	float multiplier, deltaTime;
	bool isMoving;
	FloatRect nextPos;
	///animations
	IntRect currentFrame;
	short animationState;
	///STATS TEXT
	UIStruct entityUI;
	void initSprite();
	void initPhysics();
	void updatePhysics();
public:
	void initEntity();
	void setTexture(Texture incTexture);
	void setDeltaTime(float time);
	void setWindowSize(float x, float y);
	Sprite& returnEntitySprite() { return entitySprite; }
	void updateEntity();
};