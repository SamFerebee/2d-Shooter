#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
using namespace sf;

class Tile
{
private:
	Sprite tileSprite;
	bool damaging;
public:
	Tile();
};