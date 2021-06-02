#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <math.h>
#include <random>
#include <memory>
#include "Player.h"
#include "BulletClass.h"
#include "Items.h"
#include <unordered_map>
#include "Enemy.h"

using namespace sf;

enum EnemyNames
{
	SHOOTER,
	SHOOTER_TEST,
	CREEPER_SLOW,
	MAX_NAMES
};

enum ItemNames
{
	BOMB_SMALL,
	GOLDCOIN,
	KEYCARD,
	HEART_SMALL,
	ARMOR_SMALL,
	MAX_ITEMS
};

struct SpawnTimersStruct
{
	Clock bombSpawnTimer;
	Clock healthSmallSpawnTimer;
	Clock armorSmallSpawnTimer;
	Clock redCardSpawnTimer;
	Clock goldCoin;
	Clock shooter;
};

struct itemSpawnChance
{
	std::uniform_int_distribution<> healthSmall;
	std::uniform_int_distribution<> armorSmall;
	std::uniform_int_distribution<> redCard;
	std::uniform_int_distribution<> goldCoin;
};

struct itemSpawnDiceRoll
{
	int healthSmall;
	int armorSmall;
	int redCard;
	int goldCoin;
};

class Game : protected SoundFX
{
private:
	RenderWindow window;
	void initWindow();
	Event event;
	std::unique_ptr<Player> thePlayer;
	void initPlayer();
	Clock baseClock;
	float deltaTime;
	float multiplier = 153.f;
	std::vector<std::unique_ptr<Music>> musicLibrary;
	int songNumber = 0;
	bool musicPaused = false, gamePaused = false, isDead = false;
	View playerCamera;
	void updateCamera();
	void createWalls();
	std::vector<RectangleShape> wallVector;
	Font theFont;
	Text pausedText;
	Texture bombTexture, goldCoinTexture, keyCardTexture, smallHeartTexture, smallArmorTexture, crossHairTexture, shooterTexture, shooterTestTexture, backgroundTexture;
	Sprite cursorSprite, backgroundSprite;
	bool freezeTime = false;
	//ITEM STUFF
	std::unordered_map<ItemNames, Item> baseItemsMap;
	std::vector<Item> bombVector{}, healthSmallVector{}, armorSmallVector{}, redCardVector{}, goldCoinVector{};
	itemSpawnChance itemSpawnChance;
	itemSpawnDiceRoll itemSpawnDiceRoll;
	//TIMERS
	SpawnTimersStruct spawnTimers;
	//RNG stuff
	std::mt19937 mersenneTwister{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };
	std::uniform_real_distribution<float> bombSpawnZone;
	std::uniform_real_distribution<float> healthSmallSpawnZone;
	std::uniform_real_distribution<float> armorSmallSpawnZone;
	std::uniform_int_distribution<> oneOrTwo{ 0,1 };
	//ENEMY STUFF
	std::unordered_map<EnemyNames, Enemy> baseEnemyMap;
	void updateEnemies();
	void initEnemies();
	std::vector<Enemy> shooterEnemyVector;
	void spawnShooters();
	std::vector<Enemy> shooterTestVec;
public:
	Game();
	//~Game();

	///////FUNCTIONS
	const RenderWindow& returnWindow() const;
	void update();
	void render();
	void updatePlayer();
	void initMusic();
	void playMusic();
	void controlMusic();
	void checkForPause();
	void checkPlayerCollision();
	void initText();
	void initTextures();
	void initItems();
	void updateItems();
	void updateBombs();
	void updateBulletCollision();
	void updateHealthItems();
	void updateArmorItems();
	void updateKeyCardItems();
	void updateGoldCoinItems();
};
