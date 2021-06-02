#include "Player.h"
#include <vector>

enum PLAYER_ANIMATION_STATES
{
	IDLE = 0,
	MOVING_LEFT,
	MOVING_RIGHT,
	JUMPING,
	FALLING,
	SHOOTING,
	STATIONARY_SHOOTING
};

Player::Player()
{
	initTexture();
	initSprite();
	isMoving = false;
	initAnimations();
	playerSprite.setOrigin(playerSprite.getGlobalBounds().width / 2.f, playerSprite.getGlobalBounds().height / 2);
	playerSprite.setPosition(100.f, 100.f);
	initBullet();
	initStats();
	initText();
	initPhysics();
}

void Player::initStats()
{
	health = 20;
	maxHealth = 20;
	armor = 0;
	maxArmor = 10;
	speed = 1;
	playerUI.healthString = "HEALTH: " + std::to_string(health) + '/' + std::to_string(maxHealth);
	playerUI.armorString = "ARMOR: " + std::to_string(armor) + '/' + std::to_string(maxArmor);
	playerUI.currentPointsString = "POINTS: " + std::to_string(currentPoints);
	playerUI.healthBar.setFillColor(Color::Red);
	playerUI.armorBar.setFillColor(Color::Green);
	playerUI.healthBar.setSize(Vector2f(200.f, 25.f));
	playerUI.healthBar.setOutlineColor(Color::Black);
	playerUI.healthBar.setOutlineThickness(1.5f);
	playerUI.armorBar.setSize(Vector2f(200.f, 25.f));
	playerUI.armorBar.setOutlineColor(Color::Black);
	playerUI.armorBar.setOutlineThickness(1.5f);
	playerUI.healthBar.setPosition(0, 2.f);
	playerUI.armorBar.setPosition(200.f, 2.f);
}

void Player::initText()
{
	playerUI.theFont.loadFromFile("Fonts/Chocolate Covered Raindrops.ttf");
	playerUI.healthText.setFont(playerUI.theFont);
	playerUI.healthText.setFillColor(Color::Red);
	playerUI.healthText.setOutlineColor(Color::Black);
	playerUI.healthText.setOutlineThickness(.75f);
	playerUI.healthText.setCharacterSize(35);
	playerUI.armorText.setFont(playerUI.theFont);
	playerUI.armorText.setFillColor(Color::Green);
	playerUI.armorText.setOutlineColor(Color::Black);
	playerUI.armorText.setOutlineThickness(.75f);
	playerUI.armorText.setCharacterSize(35);
	playerUI.healthText.setString(playerUI.healthString);
	playerUI.armorText.setString(playerUI.armorString);
	playerUI.healthText.setOrigin(playerUI.healthText.getGlobalBounds().width / 2, playerUI.healthText.getGlobalBounds().height / 2);
	playerUI.armorText.setOrigin(playerUI.healthText.getGlobalBounds().width / 2, playerUI.healthText.getGlobalBounds().height / 2);
	playerUI.healthText.setPosition(100.f, 30.f);
	playerUI.armorText.setPosition(300.f, 30.f);
	playerUI.currentPointsText.setFont(playerUI.theFont);
	playerUI.currentPointsText.setFillColor(Color::Cyan);
	playerUI.currentPointsText.setOutlineColor(Color::Black);
	playerUI.currentPointsText.setOutlineThickness(.75f);
	playerUI.currentPointsText.setCharacterSize(35);
	playerUI.currentPointsText.setString(playerUI.currentPointsString);
	playerUI.currentPointsText.setOrigin(playerUI.currentPointsText.getGlobalBounds().width / 2, playerUI.currentPointsText.getGlobalBounds().height / 2);
	playerUI.currentPointsText.setPosition(playerUI.armorText.getGlobalBounds().left + playerUI.armorText.getGlobalBounds().width * 2, 30.f);
}

void Player::update()
{
	movePlayer();
	updateAnimations();
	moveBullets();
	deleteBullets();
	moveNetBullets();
	deleteNetBullets();
	updateStats();
	updatePhysics();
	updateNextPos();
}

void Player::updateStats()
{
	if (armor == 0)
		playerUI.armorBar.setSize(sf::Vector2f(0.f, 0.f));
	else
		playerUI.armorBar.setSize(sf::Vector2f(armor * 20.f, 25.f));
	playerUI.healthBar.setSize(Vector2f(health * 10.f, 25.f));
	if (health > maxHealth)
		health = maxHealth;
	if (armor > maxArmor)
		armor = maxArmor;
	else if (armor < 0)
		armor = 0;
	playerUI.healthString = "HEALTH: " + std::to_string(health) + '/' + std::to_string(maxHealth);
	playerUI.armorString = "ARMOR: " + std::to_string(armor) + '/' + std::to_string(maxArmor);
	playerUI.healthText.setString(playerUI.healthString);
	playerUI.armorText.setString(playerUI.armorString);
	playerUI.currentPointsString = "POINTS: " + std::to_string(currentPoints);
	playerUI.currentPointsText.setString(playerUI.currentPointsString);
}

void Player::initSprite()
{
	playerSprite.setTexture(playerTextureSheet);
	currentFrame = IntRect(10, 16, 68, 70);
	playerSprite.setTextureRect(currentFrame);
}

void Player::initPhysics()
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

void Player::updatePhysics()
{
	//gravity
	currentVelocity.y += 15.f * gravity * deltaTime * multiplier;
	if (std::abs(currentVelocity.y) > maxGravity)
	{
		currentVelocity.y = maxGravity * ((currentVelocity.y < 0.f) ? -1.f : 1.f);
	}

	if (isJumping)
	{
		moveP(0.f, -50.f);
		if (jumpTimerClock.getElapsedTime().asSeconds() >= .3f)
		{
			isJumping = false;
		}
	}

	//deceleration
	currentVelocity *= drag * deltaTime * multiplier;
	if (std::abs(currentVelocity.x) < minVelocity)
		currentVelocity.x = 0.f;
	if (std::abs(currentVelocity.y) < minGravity)
		currentVelocity.y = 0.f;
	playerSprite.move(currentVelocity.x, currentVelocity.y);
}

void Player::initTexture()
{
	Collision::CreateTextureAndBitmask(playerTextureSheet, "Textures/spritesheet.png");
	//playerTextureSheet.loadFromFile("Textures/spritesheet.png");
}

void Player::initBullet()
{
	Collision::CreateTextureAndBitmask(normalBullet.bulletTexture, "Textures/bullet.png");
	//normalBullet.bulletTexture.loadFromFile("Textures/bullet.png");
	normalBullet.theBullet.setTexture(normalBullet.bulletTexture);
	normalBullet.theBullet.setScale(.05f, .05f);
	Collision::CreateTextureAndBitmask(netBullet.bulletTexture, "Textures/net.png");
	//netBullet.bulletTexture.loadFromFile("Textures/net.png");
	netBullet.theBullet.setTexture(netBullet.bulletTexture);
	netBullet.theBullet.setScale(.2f, .2f);
	netBullet.setMaxSpeed(7.f);
}

void Player::render(RenderWindow& targetWindow)
{
	targetWindow.draw(playerSprite);
}

void Player::renderUI(RenderWindow& targetWindow)
{
	targetWindow.draw(playerUI.healthText);
	targetWindow.draw(playerUI.armorText);
	targetWindow.draw(playerUI.healthBar);
	targetWindow.draw(playerUI.armorBar);
	targetWindow.draw(playerUI.currentPointsText);
}

Sprite& Player::returnPlayer()
{
	return playerSprite;
}

void Player::setDeltaTime(float time)
{
	deltaTime = time;
}

void Player::moveP(const float dir_x, const float dir_y)
{
	currentVelocity.x += dir_x * acceleration * deltaTime * multiplier;
	currentVelocity.y += dir_y * acceleration * deltaTime * multiplier;
	if (std::abs(currentVelocity.x) > maxVelocity)
	{
		currentVelocity.x = maxVelocity * ((currentVelocity.x < 0.f) ? -1.f : 1.f);
	}
	playerSprite.move(currentVelocity.x * deltaTime * multiplier, currentVelocity.y * deltaTime * multiplier);
}

void Player::movePlayer()
{
	direction = Vector2f(0.f, 0.f);
	if (!isJumping)
		animationState = IDLE;
	if (Keyboard::isKeyPressed(Keyboard::A) && playerSprite.getPosition().x > playerSprite.getGlobalBounds().width / 2 && canMoveLeft)
	{
		--currentVelocity.x;
		playerSprite.setScale(-1.f, 1.f);
		moveP(currentVelocity.x, 0.f);
		if (!isJumping)
			animationState = MOVING_LEFT;
		if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
		{
			animationState = SHOOTING;
		}
	}
	else if (Keyboard::isKeyPressed(Keyboard::D) && playerSprite.getPosition().x < windowSizeX - playerSprite.getGlobalBounds().width / 2 && canMoveRight)
	{
		++currentVelocity.x;
		playerSprite.setScale(1.f, 1.f);
		moveP(currentVelocity.x, 0.f);
		if (!isJumping)
			animationState = MOVING_RIGHT;
		if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
		{
			animationState = SHOOTING;
		}
	}
	else if (Mouse::isButtonPressed(Mouse::Left) || Mouse::isButtonPressed(Mouse::Right))
	{
		animationState = STATIONARY_SHOOTING;
	}

	if (Keyboard::isKeyPressed(Keyboard::Space) && jumpTimerClock.getElapsedTime().asSeconds() >= .6f && isGrounded)
	{
		isJumping = true;
		playerJumpSound.play();
		isGrounded = false;
		//moveP(0.f, -25);
		animationState = JUMPING;
		jumpTimerClock.restart();
	}
}

void Player::setWindowSize(float x, float y)
{
	windowSizeX = x;
	windowSizeY = y;
}

void Player::initAnimations()
{
	animationClock.restart();
	animationState = IDLE;
}

void Player::updateAnimations()
{

	if (animationState == IDLE)
	{
		if (animationClock.getElapsedTime().asSeconds() >= 0.3f)
		{
			if (currentFrame.top >= 18)
				currentFrame.top = 18.f;
			currentFrame.left += 80.f;
			if (currentFrame.left >= 570.f)
				currentFrame.left = 10.f;
			animationClock.restart();
			playerSprite.setTextureRect(currentFrame);
		}
	}
	else if (animationState == SHOOTING)
	{
		if (animationClock.getElapsedTime().asSeconds() >= .15f)
		{
			currentFrame.top += 205.f;
			if (currentFrame.top >= 205.f)
				currentFrame.top = 205.f;
			currentFrame.left += 80.f;
			if (currentFrame.left >= 370.f)
				currentFrame.left = 10.f;
			animationClock.restart();
			playerSprite.setTextureRect(currentFrame);
		}
	}
	else if (animationState == MOVING_LEFT || animationState == MOVING_RIGHT)
	{
		if (animationClock.getElapsedTime().asSeconds() >= 0.15f)
		{
			currentFrame.top += 112.f;
			currentFrame.left += 80.f;
			if (currentFrame.left >= 370.f)
				currentFrame.left = 10.f;
			if (currentFrame.top >= 112.f)
				currentFrame.top = 112.f;
			animationClock.restart();
			playerSprite.setTextureRect(currentFrame);
		}
	}
	else if (animationState == STATIONARY_SHOOTING)
	{
		if (animationClock.getElapsedTime().asSeconds() >= .15f)
		{
			currentFrame.top = 390.f;
			currentFrame.left = 10.f;
			animationClock.restart();
			playerSprite.setTextureRect(currentFrame);
		}
	}
	else if (animationState == JUMPING)
	{
		if (animationClock.getElapsedTime().asSeconds() >= .15f)
		{
			currentFrame.top = 300.f;
			currentFrame.left = 90.f;
			animationClock.restart();
			playerSprite.setTextureRect(currentFrame);
		}
	}
	else if (animationState == FALLING)
	{
		currentFrame.top = 300.f;
		currentFrame.left = 90.f;
		animationClock.restart();
		playerSprite.setTextureRect(currentFrame);
	}
	
}

void Player::setStartingPosition()
{
	playerSprite.setPosition(windowSizeX / 2, windowSizeY - (playerSprite.getGlobalBounds().height / 2));
}

void Player::shootPistol(RenderWindow& window, View& view)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if (shotTimerClock.getElapsedTime().asSeconds() >= .1f)
		{
			gunShotSound.play();
			playerCenter = Vector2f(playerSprite.getPosition().x, playerSprite.getPosition().y);
			mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
			aimDirection = mousePosition - playerCenter;
			aimDirectionNormal = aimDirection / sqrt(pow(aimDirection.x, 2) + pow(aimDirection.y, 2));
			normalBullet.theBullet.setOrigin(normalBullet.theBullet.getGlobalBounds().width / 2, normalBullet.theBullet.getGlobalBounds().height / 2);
			normalBullet.theBullet.setPosition(playerCenter);
			normalBullet.currentVelocity = aimDirectionNormal * normalBullet.maxSpeed;
			normalBulletVector.push_back(BulletClass(normalBullet));
			shotTimerClock.restart();
		}
	}
}

void Player::moveBullets()
{
	for (size_t i = 0; i < normalBulletVector.size(); ++i)
	{
		normalBulletVector[i].moveBullet(deltaTime, multiplier);
	}
}

std::vector<BulletClass>& Player::returnNormBulletVector()
{
	return normalBulletVector;
}

BulletClass& Player::returnNormBulletVector(int i)
{
	return normalBulletVector[i];
}

void Player::deleteBullets()
{
	for (size_t i = 0; i < normalBulletVector.size(); ++i)
	{
		if (normalBulletVector[i].theBullet.getPosition().x < 0 || normalBulletVector[i].theBullet.getPosition().x > windowSizeX || normalBulletVector[i].theBullet.getPosition().y < 0 || normalBulletVector[i].theBullet.getPosition().y > windowSizeY)
			normalBulletVector.erase(normalBulletVector.begin() + i);
	}
}

void Player::updateInfo(RenderWindow& window)
{
	playerCenter = sf::Vector2f(playerSprite.getPosition().x, playerSprite.getPosition().y);
	mousePosition = (sf::Vector2f)sf::Mouse::getPosition(window);
	aimDirection = mousePosition - playerCenter;
	aimDirectionNormal = aimDirection / sqrt(pow(aimDirection.x, 2) + pow(aimDirection.y, 2));
}

void Player::deleteNetBullets()
{
	for (size_t i = 0; i < netBulletVector.size(); ++i)
	{
		if (netBulletVector[i].theBullet.getPosition().x < 0 || netBulletVector[i].theBullet.getPosition().x > windowSizeX || netBulletVector[i].theBullet.getPosition().y < 0 || netBulletVector[i].theBullet.getPosition().y > windowSizeY)
			netBulletVector.erase(netBulletVector.begin() + i);
	}
}
void Player::shootNetGun(RenderWindow& window, View& view)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if (netShotTimerClock.getElapsedTime().asSeconds() >= .3f)
		{
			playerCenter = sf::Vector2f(playerSprite.getPosition().x, playerSprite.getPosition().y);
			mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
			aimDirection = mousePosition - playerCenter;
			aimDirectionNormal = aimDirection / sqrt(pow(aimDirection.x, 2) + pow(aimDirection.y, 2));
			netBullet.theBullet.setOrigin(netBullet.theBullet.getGlobalBounds().width / 2, netBullet.theBullet.getGlobalBounds().height / 2);
			netBullet.theBullet.setPosition(playerCenter);
			netBullet.currentVelocity = aimDirectionNormal * netBullet.maxSpeed;
			netBulletVector.push_back(BulletClass(netBullet)); 
			netShotTimerClock.restart();
		}
	}
}
void Player::moveNetBullets()
{
	for (size_t i = 0; i < netBulletVector.size(); ++i)
	{
		netBulletVector[i].moveBullet(deltaTime, multiplier);
	}
}

BulletClass& Player::returnNetBulletVector(int i)
{
	return netBulletVector[i];
}

std::vector<BulletClass>& Player::returnNetBulletVector()
{
	return netBulletVector;
}

void Player::resetVelocity()
{
	currentVelocity = Vector2f(0.f, 0.f);
}

void Player::updateNextPos()
{
	nextPos = playerSprite.getGlobalBounds();
	nextPos.left += currentVelocity.x;
	nextPos.top += currentVelocity.y;
}

FloatRect Player::returnNextPos()
{
	return nextPos;
}
