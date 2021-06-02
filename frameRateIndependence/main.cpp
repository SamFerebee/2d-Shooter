#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "Game.h"

using namespace sf;


int main()
{
	const std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
	Game theGame;
	while (theGame.returnWindow().isOpen())
	{
		theGame.update();
		theGame.render();
		theGame.playMusic();
	}
	return 0;
}










/*
int main()
{
	Sprite theSprite;
	Texture theTexture;
	theTexture.loadFromFile("Textures/enemy1.png");
	theSprite.setTexture(theTexture);
	theSprite.setOrigin(theSprite.getGlobalBounds().width / 2, theSprite.getGlobalBounds().height / 2);
	theSprite.setPosition(150.f, 150.f);
	Clock rngClock;

	sf::RenderWindow window(sf::VideoMode(800, 600), "Frame Independent");
	//window.setFramerateLimit(60);
	sf::Clock clock;
	float deltaTime;
	float multiplier = 153.f;

	sf::RectangleShape shape;
	shape.setSize(sf::Vector2f(80.f, 80.f));
	shape.setFillColor(sf::Color::Red);
	Vector2f currentVelocity;
	Vector2f direction;
	float maxVelocity = 8.f;
	float acceleration = .7f;
	float drag = 0.2f;

	shape.setOrigin(40.f, 40.f);
	shape.setPosition(window.getSize().x / 2.f, window.getSize().y / 2.f);

	sf::CircleShape triangle(80, 3);
	triangle.setFillColor(Color::Green);
	//triangle.setPosition(window.getSize().x / 2, window.getSize().y / 2);

	sf::Vector2f spritePosition;
	sf::Vector2f playerPosition;
	sf::Vector2f aimDirection;
	sf::Vector2f aimDirectionNormal;
	BulletClass bullet1(45.f);
	std::vector<BulletClass> incomingBulletVector;
	sf::Texture bulletTexture;
	bulletTexture.loadFromFile("Textures/heart.png");
	bullet1.theBullet.setTexture(bulletTexture);
	bullet1.theBullet.setScale(.2f, .2f);

	bool bRotate = false;
	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();

			if (event.type == Event::KeyPressed && event.key.code == Keyboard::R)
				bRotate = !bRotate;
			if (event.type == Event::KeyPressed && event.key.code == Keyboard::C)
				theSprite.setScale(-1.f, 1.f);
		}
		deltaTime = clock.restart().asSeconds();
	//	theSprite.setOrigin(theSprite.getGlobalBounds().width / 2, theSprite.getGlobalBounds().height / 2);
		playerPosition = sf::Vector2f(shape.getPosition().x, shape.getPosition().y);
		spritePosition = sf::Vector2f(theSprite.getPosition().x, theSprite.getPosition().y);
		aimDirection = playerPosition - spritePosition;
		aimDirectionNormal = aimDirection / sqrt(pow(aimDirection.x, 2) + pow(aimDirection.y, 2));

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			bullet1.theBullet.setPosition(spritePosition);
			bullet1.currentVelocity = aimDirectionNormal * bullet1.maxSpeed;
			incomingBulletVector.push_back(BulletClass(bullet1));

		}

		for (int i = 0; i < incomingBulletVector.size(); ++i)
		{
			incomingBulletVector[i].theBullet.move(incomingBulletVector[i].currentVelocity);
		}

		for (int i = 0; i < incomingBulletVector.size(); ++i)
		{
			if (incomingBulletVector[i].theBullet.getPosition().x < 0 || incomingBulletVector[i].theBullet.getPosition().x > window.getSize().x || incomingBulletVector[i].theBullet.getPosition().y < 0 || incomingBulletVector[i].theBullet.getPosition().y > window.getSize().y)
				incomingBulletVector.erase(incomingBulletVector.begin() + i);
		}

		if (bRotate)
			shape.rotate(10.f);
		////UPDATE
		///ACCELERATION

		direction = Vector2f(0.f, 0.f);
		if (Keyboard::isKeyPressed(Keyboard::A))
		{
			direction.x = -1.f;
			if (currentVelocity.x > -maxVelocity)
				currentVelocity.x += acceleration * direction.x * deltaTime * multiplier;
		}
		if (Keyboard::isKeyPressed(Keyboard::D))
		{
			direction.x = 1.f;
			if (currentVelocity.x < maxVelocity)
				currentVelocity.x += acceleration * direction.x * deltaTime * multiplier;
		}
		if (Keyboard::isKeyPressed(Keyboard::W))
		{
			direction.y = -1.f;
			if (currentVelocity.y > -maxVelocity)
				currentVelocity.y += acceleration * direction.y * deltaTime * multiplier;
		}
		if (Keyboard::isKeyPressed(Keyboard::S))
		{
			direction.y = 1.f;
			if (currentVelocity.y < maxVelocity)
				currentVelocity.y += acceleration * direction.y * deltaTime * multiplier;
		}

		////DRAG
		if (currentVelocity.x > 0.f)
		{
			currentVelocity.x -= drag * deltaTime * multiplier;
			if (currentVelocity.x < 0.f)
				currentVelocity.x = 0.f;
		}
		else if (currentVelocity.x < 0.f)
		{
			currentVelocity.x += drag * deltaTime * multiplier;
			if (currentVelocity.x > 0.f)
				currentVelocity.x = 0.f;
		}

		if (currentVelocity.y > 0.f)
		{
			currentVelocity.y -= drag * deltaTime * multiplier;
			if (currentVelocity.y < 0.f)
				currentVelocity.y = 0.f;
		}
		else if (currentVelocity.y < 0.f)
		{
			currentVelocity.y += drag * deltaTime * multiplier;
			if (currentVelocity.y > 0.f)
				currentVelocity.y = 0.f;
		}

		///FINAL MOVE
		shape.move(currentVelocity.x * deltaTime * multiplier, currentVelocity.y * deltaTime * multiplier);
		playerPosition = sf::Vector2f(shape.getPosition().x, shape.getPosition().y);
		spritePosition = sf::Vector2f(theSprite.getPosition().x, theSprite.getPosition().y);
		aimDirection = playerPosition - spritePosition;
		aimDirectionNormal = aimDirection / sqrt(pow(aimDirection.x, 2) + pow(aimDirection.y, 2));

		//////RNG STUFF
		//if (rngClock.getElapsedTime().asSeconds() >= 2)


		////DRAW
		window.clear(sf::Color::White);
		window.draw(shape);
		window.draw(triangle);
		window.draw(theSprite);
		for (int i = 0; i < incomingBulletVector.size(); ++i) { window.draw(incomingBulletVector[i].theBullet); }

		window.display();

		std::cout << deltaTime << '\n';
	}

	return 0;
}








1. set a frame rate and move it the amount you like in that frame rate, this is the base case, how much it will always move
2. get delta time by restart clock, cout the delta time
3. get multiplier value. multiplier = 10 * delta time 
	-to get the multiplier, get how much you need to multiply delta time by to get to your desired movement (10 in this case) at your fps picked in step 1

	if (Keyboard::isKeyPressed(Keyboard::A))
	{
		shape.move(-10.f * deltaTime * multiplier, 0.f);
	}
	if (Keyboard::isKeyPressed(Keyboard::D))
	{
		shape.move(10.f * deltaTime * multiplier, 0.f);
	}
	if (Keyboard::isKeyPressed(Keyboard::W))
	{
		shape.move(0.f, -10.f * deltaTime * multiplier);
	}
	if (Keyboard::isKeyPressed(Keyboard::S))
	{
		shape.move(0.f, 10.f * deltaTime * multiplier);
	}

*/