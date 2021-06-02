#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>



class BulletClass
{
public:
	BulletClass(float x)
	{
		currentVelocity = sf::Vector2f(0.f, 0.f);
		maxSpeed = x;
	}
	BulletClass() { currentVelocity = sf::Vector2f(0.f, 0.f); maxSpeed = 15.f; }
	void setMaxSpeed(float speed);
	sf::Sprite theBullet;
	sf::Vector2f currentVelocity;
	float maxSpeed;
	void moveBullet(float deltaTime, float multiplier);
	sf::Texture bulletTexture;

};