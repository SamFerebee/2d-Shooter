#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
using namespace sf;

class Item
{
private:
	int itemPointValue;
	int itemHealthValue;
	int itemArmorValue;
	sf::Sprite theItem{};
	bool leftRightDirection;
	bool upDownDirection;
	Texture itemTexture;
	Clock aliveTimer;
public:
	Item() {}
	Item(Texture textureFile, float xSize, float ySize);
	bool returnLeftRight();
	bool returnUpDown();
	void setLeftRight(bool x);
	void setUpDown(bool x);
	Sprite& returnItem();
	Clock& returnAliveTimer() { return aliveTimer; }
	void resetAliveTimer() { aliveTimer.restart(); }
	int returnTimeAlive() { return static_cast<int>(aliveTimer.getElapsedTime().asSeconds()); }
};