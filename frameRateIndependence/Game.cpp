#include "Game.h"

void Game::update()
{
	deltaTime = baseClock.restart().asSeconds();
	thePlayer->setDeltaTime(deltaTime);
	/////POLLING WINDOW EVENTS
	while (window.pollEvent(event))
	{
		if (event.type == Event::Closed)
			window.close();
		else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
			window.close();
		checkForPause();
		controlMusic();
	}
	if (!gamePaused)
	{
		updatePlayer();
		//updateCamera();
		updateItems();
		updateBulletCollision();
		updateEnemies();
	}
}

Game::Game()
{
	initWindow();
	initPlayer();
	initTextures();
	initMusic();
	initText();
	initEnemies();
	playerCamera.setSize(1880, 980);
	playerCamera.setCenter(thePlayer->returnPlayer().getPosition().x, 490.f);
	//createWalls();

}

void Game::createWalls()
{
	RectangleShape wall;
	wall.setFillColor(Color::Black);
	wall.setSize(Vector2f(500.f, 100.f));
	wall.setPosition(-1050.f, 880.f);
	wallVector.push_back(wall);
	RectangleShape wall1;
	wall1.setFillColor(Color::Red);
	wall1.setSize(Vector2f(500.f, 10.f));
	wall1.setPosition(650.f, 850.f);
	wallVector.push_back(wall1);
	RectangleShape wall2;
	wall2.setFillColor(Color::Blue);
	wall2.setSize(Vector2f(300.f, 50.f));
	wall2.setPosition(200.f, 940.f);
	wallVector.push_back(wall2);
	wall.setFillColor(Color::Black);
	wall.setSize(Vector2f(500.f, 100.f));
	wall.setPosition(1050.f, 880.f);
	wallVector.push_back(wall);
}

void Game::initWindow()
{
	window.create(VideoMode(1880,980), "Shooter", Style::Close | Style::Titlebar);
	window.setFramerateLimit(288);
}

void Game::render()
{
	window.clear(Color::White);
	window.draw(backgroundSprite);
	///draw player relative stuff
	window.setView(playerCamera);
	thePlayer->render(window);
	//window.draw(cursorSprite);
	for (size_t i = 0; i < thePlayer->returnNormBulletVector().size(); ++i)
	{
		window.draw(thePlayer->returnNormBulletVector(i).theBullet);
	}
	for (size_t i = 0; i < thePlayer->returnNetBulletVector().size(); ++i)
	{
		window.draw(thePlayer->returnNetBulletVector(i).theBullet);
	}
	for (size_t i = 0; i < bombVector.size(); ++i)
	{
		window.draw(bombVector[i].returnItem());
	}
	for (size_t i = 0; i < healthSmallVector.size(); ++i)
	{
		window.draw(healthSmallVector[i].returnItem());
	}
	for (size_t i = 0; i < armorSmallVector.size(); ++i)
	{
		window.draw(armorSmallVector[i].returnItem());
	}
	for (size_t i = 0; i < redCardVector.size(); ++i)
	{
		window.draw(redCardVector[i].returnItem());
	}
	for (size_t i = 0; i < goldCoinVector.size(); ++i)
	{
		window.draw(goldCoinVector[i].returnItem());
	}
	for (size_t i = 0; i < shooterEnemyVector.size(); ++i)
	{
		window.draw(shooterEnemyVector[i].returnEntitySprite());
	}
	for (size_t i = 0; i < shooterTestVec.size(); ++i)
	{
		window.draw(shooterTestVec[i].returnEntitySprite());
	}
	/*
	for (auto i : wallVector)
	{
		window.draw(i);
	}
	*/
	//draw UI
	window.setView(window.getDefaultView());
	window.draw(cursorSprite);
	thePlayer->renderUI(window);
	if (gamePaused)
		window.draw(pausedText);

	window.display();
}

void Game::updateCamera()
{
	playerCamera.setCenter(thePlayer->returnPlayer().getPosition().x, 490.f);
}

const RenderWindow& Game::returnWindow() const
{
	return window;
}

void Game::initPlayer()
{
	thePlayer =  std::make_unique<Player>();
	thePlayer->setWindowSize(window.getSize().x, window.getSize().y);
	thePlayer->setStartingPosition();
}

void Game::updatePlayer()
{
	checkPlayerCollision();
	thePlayer->update();
	thePlayer->updateInfo(window);
	thePlayer->shootPistol(window, playerCamera);
	thePlayer->shootNetGun(window, playerCamera);

}

void Game::initMusic()
{
	std::unique_ptr<sf::Music> iSeeFirePtr{ std::make_unique<sf::Music>() };
	iSeeFirePtr->openFromFile("Sound/iseefire.wav");
	std::unique_ptr<sf::Music> underBridgePtr{ std::make_unique<sf::Music>() };
	underBridgePtr->openFromFile("Sound/underthebridge.wav");
	std::unique_ptr<sf::Music> whiteIversonPtr{ std::make_unique<sf::Music>() };
	whiteIversonPtr->openFromFile("Sound/whiteiverson.wav");
	std::unique_ptr<sf::Music> betterNowPtr{ std::make_unique<sf::Music>() };
	betterNowPtr->openFromFile("Sound/betternow.wav");
	std::unique_ptr<sf::Music> ballForMePtr{ std::make_unique<sf::Music>() };
	ballForMePtr->openFromFile("Sound/ballforme.wav");
	musicLibrary.push_back(std::move(iSeeFirePtr));
	musicLibrary.push_back(std::move(underBridgePtr));
	musicLibrary.push_back(std::move(whiteIversonPtr));
	musicLibrary.push_back(std::move(betterNowPtr));
	musicLibrary.push_back(std::move(ballForMePtr));
}


void Game::playMusic()
{
	if (musicLibrary[songNumber]->getStatus() == sf::Music::Stopped)
	{
		if (songNumber == musicLibrary.size() - 1)
			songNumber = 0;
		else
			++songNumber;
		musicLibrary[songNumber]->play();
	}
}

void Game::controlMusic()
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::M)
	{
		musicPaused = !musicPaused;
		if (musicPaused)
			musicLibrary[songNumber]->pause();
		else
			musicLibrary[songNumber]->play();
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::N)
	{
		musicLibrary[songNumber]->stop();
		if (songNumber == musicLibrary.size() - 1)
			songNumber = 0;
		else
			++songNumber;
		musicLibrary[songNumber]->play();
	}

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::B)
	{
		musicLibrary[songNumber]->stop();
		if (songNumber == 0)
			songNumber = musicLibrary.size() - 1;
		else
			--songNumber;
		musicLibrary[songNumber]->play();
	}
}

void Game::checkForPause()
{
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P)
		gamePaused = !gamePaused;
}


void Game::checkPlayerCollision()
{
	//screen collision
	if (thePlayer->returnPlayer().getPosition().x < thePlayer->returnPlayer().getGlobalBounds().width / 2)
	{
		thePlayer->returnPlayer().setPosition(thePlayer->returnPlayer().getGlobalBounds().width / 2, thePlayer->returnPlayer().getPosition().y);
		thePlayer->resetVelocity();
	}
	if (thePlayer->returnPlayer().getPosition().x > window.getSize().x - thePlayer->returnPlayer().getGlobalBounds().width / 2)
	{
		thePlayer->returnPlayer().setPosition(window.getSize().x - thePlayer->returnPlayer().getGlobalBounds().width / 2, thePlayer->returnPlayer().getPosition().y);
		thePlayer->resetVelocity();
	}
	if (thePlayer->returnPlayer().getPosition().y <= thePlayer->returnPlayer().getGlobalBounds().height / 2)
	{
		thePlayer->returnPlayer().setPosition(thePlayer->returnPlayer().getPosition().x, 0.f + thePlayer->returnPlayer().getGlobalBounds().height / 2);
		thePlayer->resetVelocity();
	}
	if (thePlayer->returnPlayer().getPosition().y >= window.getSize().y - thePlayer->returnPlayer().getGlobalBounds().height / 2)
	{
		thePlayer->isGrounded = true;
		thePlayer->returnPlayer().setPosition(thePlayer->returnPlayer().getPosition().x, window.getSize().y - thePlayer->returnPlayer().getGlobalBounds().height / 2);
		thePlayer->resetVelocity();
	}

	//wall collision
	/*
	FloatRect nextPos, playerBounds, wallBounds;
	for (auto i : wallVector)
	{
		nextPos = thePlayer->returnNextPos();
		playerBounds = thePlayer->returnPlayer().getGlobalBounds();
		wallBounds = i.getGlobalBounds();
		if (wallBounds.intersects(nextPos))
		{
			//right collision
			if (playerBounds.left < wallBounds.left
				&& playerBounds.left + playerBounds.width < wallBounds.left + wallBounds.width
				&& playerBounds.top < wallBounds.top + wallBounds.height
				&& playerBounds.top + playerBounds.height > wallBounds.top
				)
			{
				thePlayer->canMoveRight = false;
				thePlayer->setVelocityX(0.f);
				//thePlayer->returnPlayer().setPosition(wallBounds.left - (playerBounds.width / 2), thePlayer->returnPlayer().getPosition().y);
			}
			else
				thePlayer->canMoveRight = true;
			//left collision
			if (playerBounds.left > wallBounds.left
				&& playerBounds.left + playerBounds.width > wallBounds.left + wallBounds.width
				&& playerBounds.top < wallBounds.top + wallBounds.height
				&& playerBounds.top + playerBounds.height > wallBounds.top
				)
			{
				thePlayer->canMoveLeft = false;
				thePlayer->setVelocityX(0.f);
				//thePlayer->returnPlayer().setPosition(wallBounds.left + (wallBounds.width), thePlayer->returnPlayer().getPosition().y);
			}
			else
				thePlayer->canMoveLeft = true;
			//bottom collision
			if (thePlayer->returnPlayer().getGlobalBounds().top < i.getGlobalBounds().top
				&& thePlayer->returnPlayer().getGlobalBounds().top + thePlayer->returnPlayer().getGlobalBounds().height < i.getGlobalBounds().top + i.getGlobalBounds().height
				&& thePlayer->returnPlayer().getGlobalBounds().left < i.getGlobalBounds().left + i.getGlobalBounds().width - (thePlayer->returnPlayer().getGlobalBounds().width / 3)
				&& thePlayer->returnPlayer().getGlobalBounds().left + thePlayer->returnPlayer().getGlobalBounds().width > i.getGlobalBounds().left + (thePlayer->returnPlayer().getGlobalBounds().width / 3)
				)
			{
				thePlayer->setVelocityY(0.f);
				thePlayer->returnPlayer().setPosition(thePlayer->returnPlayer().getPosition().x, i.getGlobalBounds().top - thePlayer->returnPlayer().getGlobalBounds().height / 2);
				thePlayer->isGrounded = true;
			}
			//top collision
			if (thePlayer->returnPlayer().getGlobalBounds().top > i.getGlobalBounds().top
				&& thePlayer->returnPlayer().getGlobalBounds().top + thePlayer->returnPlayer().getGlobalBounds().height > i.getGlobalBounds().top + i.getGlobalBounds().height
				&& thePlayer->returnPlayer().getGlobalBounds().left < i.getGlobalBounds().left + i.getGlobalBounds().width - (thePlayer->returnPlayer().getGlobalBounds().width / 3)
				&& thePlayer->returnPlayer().getGlobalBounds().left + thePlayer->returnPlayer().getGlobalBounds().width > i.getGlobalBounds().left + (thePlayer->returnPlayer().getGlobalBounds().width / 3)
				)
			{
				thePlayer->isJumping = false;
				//thePlayer->returnPlayer().setPosition(thePlayer->returnPlayer().getPosition().x, i.getGlobalBounds().top + i.getSize().y + thePlayer->returnPlayer().getGlobalBounds().height);
			}
		}
		else
		{
			thePlayer->canMoveLeft = true;
			thePlayer->canMoveRight = true;
		}
		
	}*/
}

/* 

	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::L)
	{
		isDead = false;
		gamePaused = false;
		//youDiedText.setPosition(-1000.f, -1000.f);
	}
*/


void Game::initText()
{
	theFont.loadFromFile("Fonts/Chocolate Covered Raindrops.ttf");
	pausedText.setFont(theFont);
	pausedText.setFillColor(sf::Color::Red);
	pausedText.setOutlineThickness(1.5f);
	pausedText.setCharacterSize(200);
	std::string pauseString{ "GAME PAUSED " };
	pausedText.setString(pauseString);
	pausedText.setPosition(window.getSize().x / 2 - (pausedText.getGlobalBounds().width / 2), window.getSize().y / 2 - (pausedText.getGlobalBounds().height * 2));
}

void Game::initTextures()
{
	Collision::CreateTextureAndBitmask(bombTexture, "Textures/bomb.png");
	//bombTexture.loadFromFile("Textures/bomb.png");
	goldCoinTexture.loadFromFile("Textures/goldCoin.png");
	keyCardTexture.loadFromFile("Textures/redcard.png");
	smallHeartTexture.loadFromFile("Textures/heart.png");
	smallArmorTexture.loadFromFile("Textures/shield.png");
	crossHairTexture.loadFromFile("Textures/crosshair.png");
	backgroundTexture.loadFromFile("Textures/backgroundsnow.png");
	initItems();

	shooterTexture.loadFromFile("Textures/shooter.png");
	shooterTestTexture.loadFromFile("Textures/shooterSheet.png");
	initEnemies();
}

void Game::initEnemies()
{
	Enemy tempShooterEnemy;
	tempShooterEnemy.setDeltaTime(deltaTime);
	tempShooterEnemy.setWindowSize(window.getSize().x, window.getSize().y);
	tempShooterEnemy.returnEntitySprite().setTexture(shooterTexture);
	tempShooterEnemy.initEntity();
	tempShooterEnemy.returnEntitySprite().setScale(.65f, .65f);
	baseEnemyMap[SHOOTER] = tempShooterEnemy;

	Enemy tempShooterTest;
	tempShooterTest.setDeltaTime(deltaTime);
	tempShooterTest.setWindowSize(window.getSize().x, window.getSize().y);
	tempShooterTest.returnEntitySprite().setTexture(shooterTestTexture);
	tempShooterTest.initEntity();
	tempShooterTest.returnEntitySprite().setScale(.4f, .4f);
	tempShooterTest.returnEntitySprite().setTextureRect(IntRect(680, 250, 140, 160));
	baseEnemyMap[SHOOTER_TEST] = tempShooterTest;
}

void Game::initItems()
{
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setScale(2.2f, 1.7f);
	cursorSprite.setTexture(crossHairTexture);
	cursorSprite.setScale(.15f, .15f);
	cursorSprite.setOrigin(cursorSprite.getGlobalBounds().width * 3, cursorSprite.getGlobalBounds().height * 3);
	window.setMouseCursorVisible(false);

	Item tempBomb;
	tempBomb.returnItem().setTexture(bombTexture);
	tempBomb.returnItem().setScale(.3f, .3f);
	//bombItem = tempBomb;
	baseItemsMap[BOMB_SMALL] = tempBomb;
	//bombItem.returnItem().setTexture(bombTexture);
	//bombItem.returnItem().setScale(.3f, .3f);
	bombSpawnZone = std::uniform_real_distribution { 0.f, window.getSize().x - 24.f };

	Item tempHealthSmall;
	tempHealthSmall.returnItem().setTexture(smallHeartTexture);
	tempHealthSmall.returnItem().setScale(.15f, .15f);
	baseItemsMap[HEART_SMALL] = tempHealthSmall;
	healthSmallSpawnZone = std::uniform_real_distribution{ 0.f, window.getSize().x - 24.f };

	Item tempArmorSmall;
	tempArmorSmall.returnItem().setTexture(smallArmorTexture);
	tempArmorSmall.returnItem().setScale(.17f, .17f);
	baseItemsMap[ARMOR_SMALL] = tempArmorSmall;
	armorSmallSpawnZone = std::uniform_real_distribution{ 0.f, window.getSize().x - 24.f };

	Item tempRedCard;
	tempRedCard.returnItem().setTexture(keyCardTexture);
	tempRedCard.returnItem().setScale(.115f, .115f);
	baseItemsMap[KEYCARD] = tempRedCard;

	Item tempGoldCoin;
	tempGoldCoin.returnItem().setTexture(goldCoinTexture);
	tempGoldCoin.returnItem().setScale(.4f, .4f);
	baseItemsMap[GOLDCOIN] = tempGoldCoin;

	//ITEM SPAWN CHANCE
	itemSpawnChance.healthSmall = std::uniform_int_distribution{ 1, 3 };
	itemSpawnChance.armorSmall = std::uniform_int_distribution{ 1, 3 };
	itemSpawnChance.redCard = std::uniform_int_distribution{ 1, 5 };
	itemSpawnChance.goldCoin = std::uniform_int_distribution{ 1,2 };
}

void Game::updateItems()
{
	cursorSprite.setPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y);
	updateBombs();
	updateHealthItems();
	updateArmorItems();
	updateKeyCardItems();
	updateGoldCoinItems();
}

void Game::updateGoldCoinItems()
{
	//spawn
	if (spawnTimers.goldCoin.getElapsedTime().asSeconds() >= 2)
	{
		itemSpawnDiceRoll.goldCoin =  itemSpawnChance.goldCoin(mersenneTwister);
		if (itemSpawnDiceRoll.goldCoin == 1)
		{
			baseItemsMap[GOLDCOIN].returnItem().setPosition(healthSmallSpawnZone(mersenneTwister), 0.f);
			baseItemsMap[GOLDCOIN].resetAliveTimer();
			goldCoinVector.push_back(baseItemsMap[GOLDCOIN]);
		}
		spawnTimers.goldCoin.restart();
	}

	for (size_t i = 0; i < goldCoinVector.size(); ++i)
	{
		//move
		if (!freezeTime && goldCoinVector[i].returnItem().getGlobalBounds().top < window.getSize().y - goldCoinVector[i].returnItem().getGlobalBounds().height)
			goldCoinVector[i].returnItem().move(0.f, 2.5f * deltaTime * multiplier);
		else
			if (!freezeTime)
				goldCoinVector[i].returnItem().setPosition(goldCoinVector[i].returnItem().getPosition().x, window.getSize().y - goldCoinVector[i].returnItem().getGlobalBounds().height);
		//delete
		if (!freezeTime && goldCoinVector[i].returnTimeAlive() >= 3)
			goldCoinVector.erase(goldCoinVector.begin() + i);

		//collision with player (bullet collision handled in updateBulletCollision())
		else if (goldCoinVector[i].returnItem().getGlobalBounds().intersects(thePlayer->returnPlayer().getGlobalBounds()))
		{
			gotLootSound.play();
			thePlayer->increaseCurrentPoints(3);
			goldCoinVector.erase(goldCoinVector.begin() + i);
			break;
		}
	}
}

void Game::updateKeyCardItems()
{
	//spawn
	if (spawnTimers.redCardSpawnTimer.getElapsedTime().asSeconds() >= 10)
	{
		itemSpawnDiceRoll.redCard = itemSpawnChance.redCard(mersenneTwister);
		if (itemSpawnDiceRoll.redCard == 1)
		{
			baseItemsMap[KEYCARD].returnItem().setPosition(healthSmallSpawnZone(mersenneTwister), 0.f);
			baseItemsMap[KEYCARD].resetAliveTimer();
			redCardVector.push_back(baseItemsMap[KEYCARD]);
		}
		spawnTimers.redCardSpawnTimer.restart();
	}

	for (size_t i = 0; i < redCardVector.size(); ++i)
	{
		//move
		if (!freezeTime && redCardVector[i].returnItem().getGlobalBounds().top < window.getSize().y - redCardVector[i].returnItem().getGlobalBounds().height)
			redCardVector[i].returnItem().move(0.f, 3.5f * deltaTime * multiplier);
		else
			if (!freezeTime)
				redCardVector[i].returnItem().setPosition(redCardVector[i].returnItem().getPosition().x, window.getSize().y - redCardVector[i].returnItem().getGlobalBounds().height);
		//delete
		if (!freezeTime && redCardVector[i].returnTimeAlive() >= 3)
			redCardVector.erase(redCardVector.begin() + i);

		//collision with player (bullet collision handled in updateBulletCollision())
		else if (redCardVector[i].returnItem().getGlobalBounds().intersects(thePlayer->returnPlayer().getGlobalBounds()))
		{
			gotLootSound.play();
			thePlayer->increaseCurrentPoints(20);
			redCardVector.erase(redCardVector.begin() + i);
			break;
		}
	}
}

void Game::updateHealthItems()
{
	//spawn
	if (spawnTimers.healthSmallSpawnTimer.getElapsedTime().asSeconds() >= 10)
	{
		itemSpawnDiceRoll.healthSmall = itemSpawnChance.healthSmall(mersenneTwister);
		if (itemSpawnDiceRoll.healthSmall == 1)
		{
			baseItemsMap[HEART_SMALL].returnItem().setPosition(healthSmallSpawnZone(mersenneTwister), 0.f);
			baseItemsMap[HEART_SMALL].resetAliveTimer();
			healthSmallVector.push_back(baseItemsMap[HEART_SMALL]);
		}
		spawnTimers.healthSmallSpawnTimer.restart();
	}

	for (size_t i = 0; i < healthSmallVector.size(); ++i)
	{
		//move
		if (!freezeTime && healthSmallVector[i].returnItem().getGlobalBounds().top < window.getSize().y - healthSmallVector[i].returnItem().getGlobalBounds().height)
			healthSmallVector[i].returnItem().move(0.f, 2.5f * deltaTime * multiplier);
		else
			if (!freezeTime)
				healthSmallVector[i].returnItem().setPosition(healthSmallVector[i].returnItem().getPosition().x, window.getSize().y - healthSmallVector[i].returnItem().getGlobalBounds().height);
		//delete
		if (!freezeTime && healthSmallVector[i].returnTimeAlive() >= 5)
			healthSmallVector.erase(healthSmallVector.begin() + i);
		
		//collision with player (bullet collision handled in updateBulletCollision())
		else if (healthSmallVector[i].returnItem().getGlobalBounds().intersects(thePlayer->returnPlayer().getGlobalBounds()))
		{
			gotHeartSound.play();
			thePlayer->increaseHealth(2);
			healthSmallVector.erase(healthSmallVector.begin() + i);
			break;
		}
	}
}

void Game::updateArmorItems()
{
	//spawn
	if (spawnTimers.armorSmallSpawnTimer.getElapsedTime().asSeconds() >= 10)
	{
		itemSpawnDiceRoll.armorSmall = itemSpawnChance.armorSmall(mersenneTwister);
		if (itemSpawnDiceRoll.armorSmall == 1)
		{
			baseItemsMap[ARMOR_SMALL].returnItem().setPosition(armorSmallSpawnZone(mersenneTwister), 0.f);
			baseItemsMap[ARMOR_SMALL].resetAliveTimer();
			armorSmallVector.push_back(baseItemsMap[ARMOR_SMALL]);
		}
		spawnTimers.armorSmallSpawnTimer.restart();
	}

	for (size_t i = 0; i < armorSmallVector.size(); ++i)
	{
		//move
		if (!freezeTime && armorSmallVector[i].returnItem().getGlobalBounds().top < window.getSize().y - armorSmallVector[i].returnItem().getGlobalBounds().height)
			armorSmallVector[i].returnItem().move(0.f, 2.5f * deltaTime * multiplier);
		else
			if (!freezeTime)
				armorSmallVector[i].returnItem().setPosition(armorSmallVector[i].returnItem().getPosition().x, window.getSize().y - armorSmallVector[i].returnItem().getGlobalBounds().height);
		//delete
		if (!freezeTime && armorSmallVector[i].returnTimeAlive() >= 5)
			armorSmallVector.erase(armorSmallVector.begin() + i);

		//collision with player (bullet collision handled in updateBulletCollision())
		else if (armorSmallVector[i].returnItem().getGlobalBounds().intersects(thePlayer->returnPlayer().getGlobalBounds()))
		{
			gotArmorSound.play();
			thePlayer->increaseArmor(1);
			armorSmallVector.erase(armorSmallVector.begin() + i);
			break;
		}
	}
}

void Game::updateBombs()
{
	//spawn bombs
	
	if (spawnTimers.bombSpawnTimer.getElapsedTime().asSeconds() >= 2)
	{
		baseItemsMap[BOMB_SMALL].returnItem().setPosition(bombSpawnZone(mersenneTwister), 0.f);
		baseItemsMap[BOMB_SMALL].setLeftRight(oneOrTwo(mersenneTwister));
		bombVector.push_back(baseItemsMap[BOMB_SMALL]);
		spawnTimers.bombSpawnTimer.restart();
	}
	
	//bounce bombs
	for (int i = 0; i < bombVector.size(); ++i)
	{
		if (bombVector[i].returnItem().getPosition().y >= window.getSize().y - (bombVector[i].returnItem().getGlobalBounds().height))
			bombVector[i].setUpDown(true);
		else if (bombVector[i].returnItem().getPosition().y <= 0)
			bombVector[i].setUpDown(false);
		if (bombVector[i].returnItem().getPosition().x >= window.getSize().x - (bombVector[i].returnItem().getGlobalBounds().width))
			bombVector[i].setLeftRight(true);
		else if (bombVector[i].returnItem().getPosition().x <= 0)
			bombVector[i].setLeftRight(false);
		if (!freezeTime)
		{
			if (!bombVector[i].returnUpDown())
				bombVector[i].returnItem().move(0.f, 4.5f * deltaTime * multiplier);
			else
				bombVector[i].returnItem().move(0.f, -4.5f * deltaTime * multiplier);
			if (!bombVector[i].returnLeftRight())
				bombVector[i].returnItem().move(4.5f * deltaTime * multiplier, 0.f);
			else
				bombVector[i].returnItem().move(-4.5f * deltaTime * multiplier, 0.f);
		}
		if (Collision::PixelPerfectTest(thePlayer->returnPlayer(), bombVector[i].returnItem()))
		{
			bombExplodeSound.play();
			playerDamageSound.play();
			int leftover{ thePlayer->returnCurrentArmor() - 2 };
			bombVector.erase(bombVector.begin() + i);
			thePlayer->decreaseArmor(2);
			if (thePlayer->returnCurrentArmor() <= 0)
				if (leftover != 0)
					thePlayer->decreaseHealth(std::abs(leftover));
			break;
		}
	}
	//collision with bullets is in updateBulletCollision()
	
}

void Game::updateBulletCollision()
{
	//BOMBS
	for (size_t i = 0; i < thePlayer->returnNormBulletVector().size(); ++i)
	{
		for (size_t j = 0; j < bombVector.size(); ++j)
		{
			if (Collision::PixelPerfectTest(thePlayer->returnNormBulletVector(i).theBullet, bombVector[j].returnItem()))
			{
				bombExplodeSound.play();
				bombVector.erase(bombVector.begin() + j);
				thePlayer->returnNormBulletVector().erase(thePlayer->returnNormBulletVector().begin() + i);
				thePlayer->increaseCurrentPoints(4);
				break;
			}
		}
	}
	//HEALTH_SMALL
	for (size_t i = 0; i < thePlayer->returnNormBulletVector().size(); ++i)
	{
		for (size_t j = 0; j < healthSmallVector.size(); ++j)
		{
			if (thePlayer->returnNormBulletVector()[i].theBullet.getGlobalBounds().intersects(healthSmallVector[j].returnItem().getGlobalBounds()))
			{
				healthSmallVector.erase(healthSmallVector.begin() + j);
				thePlayer->returnNormBulletVector().erase(thePlayer->returnNormBulletVector().begin() + i);
				break;
			}
		}
	}
	//ARMOR_SMALL
	for (size_t i = 0; i < thePlayer->returnNormBulletVector().size(); ++i)
	{
		for (size_t j = 0; j < armorSmallVector.size(); ++j)
		{
			if (thePlayer->returnNormBulletVector()[i].theBullet.getGlobalBounds().intersects(armorSmallVector[j].returnItem().getGlobalBounds()))
			{
				armorSmallVector.erase(armorSmallVector.begin() + j);
				thePlayer->returnNormBulletVector().erase(thePlayer->returnNormBulletVector().begin() + i);
				break;
			}
		}
	}
	//REDCARDS
	for (size_t i = 0; i < thePlayer->returnNormBulletVector().size(); ++i)
	{
		for (size_t j = 0; j < redCardVector.size(); ++j)
		{
			if (thePlayer->returnNormBulletVector()[i].theBullet.getGlobalBounds().intersects(redCardVector[j].returnItem().getGlobalBounds()))
			{
				redCardVector.erase(redCardVector.begin() + j);
				thePlayer->returnNormBulletVector().erase(thePlayer->returnNormBulletVector().begin() + i);
				break;
			}
		}
	}
	//GOLDCOINS
	for (size_t i = 0; i < thePlayer->returnNormBulletVector().size(); ++i)
	{
		for (size_t j = 0; j < goldCoinVector.size(); ++j)
		{
			if (thePlayer->returnNormBulletVector()[i].theBullet.getGlobalBounds().intersects(goldCoinVector[j].returnItem().getGlobalBounds()))
			{
				goldCoinVector.erase(goldCoinVector.begin() + j);
				thePlayer->returnNormBulletVector().erase(thePlayer->returnNormBulletVector().begin() + i);
				break;
			}
		}
	}

	//NET BULLETS
	for (size_t i = 0; i < thePlayer->returnNetBulletVector().size(); ++i)
	{
		//BOMBS
		for (size_t j = 0; j < bombVector.size(); ++j)
		{
			if (Collision::PixelPerfectTest(thePlayer->returnNetBulletVector(i).theBullet, bombVector[j].returnItem()))
			{
				bombExplodeSound.play();
				playerDamageSound.play();
				int leftover{ thePlayer->returnCurrentArmor() - 3 };
				bombVector.erase(bombVector.begin() + j);
				thePlayer->returnNetBulletVector().erase(thePlayer->returnNetBulletVector().begin() + i);
				thePlayer->decreaseArmor(3);
				if (thePlayer->returnCurrentArmor() <= 0)
					if (leftover != 0)
						thePlayer->decreaseHealth(std::abs(leftover));
				break;
			}
		}
	}
	//HEALTH_SMALL
	for (size_t i = 0; i < thePlayer->returnNetBulletVector().size(); ++i)
	{
		for (size_t j = 0; j < healthSmallVector.size(); ++j)
		{
			if (thePlayer->returnNetBulletVector()[i].theBullet.getGlobalBounds().intersects(healthSmallVector[j].returnItem().getGlobalBounds()))
			{
				gotHeartSound.play();
				thePlayer->increaseHealth(2);
				healthSmallVector.erase(healthSmallVector.begin() + j);
				thePlayer->returnNetBulletVector().erase(thePlayer->returnNetBulletVector().begin() + i);
				break;
			}
		}
	}
	//ARMOR_SMALL
	for (size_t i = 0; i < thePlayer->returnNetBulletVector().size(); ++i)
	{
		for (size_t j = 0; j < armorSmallVector.size(); ++j)
		{
			if (thePlayer->returnNetBulletVector()[i].theBullet.getGlobalBounds().intersects(armorSmallVector[j].returnItem().getGlobalBounds()))
			{
				gotArmorSound.play();
				thePlayer->increaseArmor(1);
				armorSmallVector.erase(armorSmallVector.begin() + j);
				thePlayer->returnNetBulletVector().erase(thePlayer->returnNetBulletVector().begin() + i);
				break;
			}
		}
	}
	//REDCARDS
	for (size_t i = 0; i < thePlayer->returnNetBulletVector().size(); ++i)
	{
		for (size_t j = 0; j < redCardVector.size(); ++j)
		{
			if (thePlayer->returnNetBulletVector()[i].theBullet.getGlobalBounds().intersects(redCardVector[j].returnItem().getGlobalBounds()))
			{
				gotLootSound.play();
				thePlayer->increaseCurrentPoints(20);
				redCardVector.erase(redCardVector.begin() + j);
				thePlayer->returnNetBulletVector().erase(thePlayer->returnNetBulletVector().begin() + i);
				break;
			}
		}
	}
	//GOLD COINS
	for (size_t i = 0; i < thePlayer->returnNetBulletVector().size(); ++i)
	{
		for (size_t j = 0; j < goldCoinVector.size(); ++j)
		{
			if (thePlayer->returnNetBulletVector()[i].theBullet.getGlobalBounds().intersects(goldCoinVector[j].returnItem().getGlobalBounds()))
			{
				gotLootSound.play();
				goldCoinVector.erase(goldCoinVector.begin() + j);
				thePlayer->returnNetBulletVector().erase(thePlayer->returnNetBulletVector().begin() + i);
				thePlayer->increaseCurrentPoints(3);
				break;
			}
		}
	}
}

void Game::updateEnemies()
{
	//spawnShooters();
}

void Game::spawnShooters()
{
	if (spawnTimers.shooter.getElapsedTime().asSeconds() >= 5)
	{
		std::cout << "should spawn\n";
		baseEnemyMap[SHOOTER].returnEntitySprite().setPosition(healthSmallSpawnZone(mersenneTwister), window.getSize().y - (baseEnemyMap[SHOOTER].returnEntitySprite().getGlobalBounds().height));
		shooterEnemyVector.push_back(baseEnemyMap[SHOOTER]);
		spawnTimers.shooter.restart();
	}
	if (spawnTimers.shooter.getElapsedTime().asSeconds() >= 1)
	{
		std::cout << "should spawn\n";
		baseEnemyMap[SHOOTER_TEST].returnEntitySprite().setPosition(healthSmallSpawnZone(mersenneTwister), window.getSize().y - (baseEnemyMap[SHOOTER_TEST].returnEntitySprite().getGlobalBounds().height));
		shooterTestVec.push_back(baseEnemyMap[SHOOTER_TEST]);
		spawnTimers.shooter.restart();
	}
}