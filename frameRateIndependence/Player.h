#pragma once
#include "Entity.h"
using namespace sf;

///STATS TEXT
struct playerUIStruct
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

class Player : public SoundFX
{
private:
	Sprite playerSprite;
	Texture playerTextureSheet;
	Vector2f direction;
	//clocks
	Clock animationClock;
	Clock shotTimerClock;
	Clock netShotTimerClock;
	Clock jumpTimerClock;
	///window info
	float windowSizeX;
	float windowSizeY;
	//physics
	Vector2f currentVelocity;
	float maxVelocity;
	float minVelocity;
	float acceleration;
	float drag;
	float gravity;
	float maxGravity;
	float minGravity;
	float multiplier;
	float deltaTime;
	bool isMoving;
	FloatRect nextPos;
	Vector2f playerCenter;
	Vector2f mousePosition;
	Vector2f aimDirection;
	Vector2f aimDirectionNormal;
	///animations
	IntRect currentFrame;
	short animationState;
	///functions
	void initSprite();
	void initTexture();
	void initAnimations();
	void initBullet();
	BulletClass normalBullet;
	std::vector<BulletClass> normalBulletVector;
	void deleteBullets();
	BulletClass netBullet;
	std::vector<BulletClass> netBulletVector;
	void deleteNetBullets();
	///STATS
	int currentPoints = 0;
	int health;
	int maxHealth;
	int armor;
	int maxArmor;
	int speed = 1;
	playerUIStruct playerUI;
	//STATS BARS
	void updateStats();
	void initPhysics();
	void updatePhysics();
	//void moveP(const float dir_x, const float dir_y);
	void updateNextPos();
public:
	Player();
	//~Player();
	bool isJumping;
	bool canMoveRight = true;
	bool canMoveLeft = true;
	bool isGrounded = true;
	///////////FUNCITONS
	void update();
	void render(RenderWindow& targetWindow);
	Sprite& returnPlayer();
	void setDeltaTime(float time);
	void movePlayer();
	void setWindowSize(float x, float y);
	void updateAnimations();
	void setStartingPosition();
	void shootPistol(RenderWindow& window, View& view);
	void moveBullets();
	std::vector<BulletClass>& returnNormBulletVector();
	BulletClass& returnNormBulletVector(int i);
	void updateInfo(RenderWindow& window);
	void shootNetGun(RenderWindow& window, View& view);
	void moveNetBullets();
	std::vector<BulletClass>& returnNetBulletVector();
	BulletClass& returnNetBulletVector(int i);
	void initStats();
	void initText();
	void moveP(const float dir_x, const float dir_y);
	void resetVelocity();
	void renderUI(RenderWindow& targetWindow);
	FloatRect returnNextPos();
	void setVelocityX(float x) { currentVelocity.x = x; }
	void setVelocityY(float y) { currentVelocity.y = y; }
	void increaseCurrentPoints(int x) { currentPoints += x; }
	void decreaseHealth(int x) { health -= x; }
	void increaseHealth(int x) { health += x; }
	void decreaseArmor(int x) { armor -= x; }
	void increaseArmor(int x) { armor += x; }
	int returnCurrentArmor() { return armor; }
	int returnCurrentHealth() { return health; }
	int returnMaxHealth() { return maxHealth; }
	void setHealth(int x) { health = x; }
	int returnMaxArmor() { return maxArmor; }
	void setArmor(int x) { armor = x; }
};

