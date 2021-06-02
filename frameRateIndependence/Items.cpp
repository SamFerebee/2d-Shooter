#include "Items.h"

Item::Item(Texture textureFile, float xSize, float ySize)
{ 
	itemPointValue = 0; itemHealthValue = 0; itemArmorValue = 0; 
	itemTexture = textureFile;
	theItem.setTexture(itemTexture);
	theItem.setScale(xSize, ySize);

}


void Item::setLeftRight(bool x) { leftRightDirection = x; }
void Item::setUpDown(bool x) { upDownDirection = x; }
bool Item::returnLeftRight() { return leftRightDirection; }
bool Item::returnUpDown() { return upDownDirection; }

Sprite& Item::returnItem() { return theItem; }