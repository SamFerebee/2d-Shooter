#include "SoundFX.h"

SoundFX::SoundFX()
{
	gunShotBuffer.loadFromFile("Sound/gunshot.wav");
	gunShotSound.setBuffer(gunShotBuffer);
	gunShotSound.setVolume(25);

	playerDamageSoundBuffer.loadFromFile("Sound/playerdamage.wav");
	playerDamageSound.setBuffer(playerDamageSoundBuffer);
	playerDamageSound.setVolume(100);

	gotLootSoundBuffer.loadFromFile("Sound/gotloot.wav");
	gotLootSound.setBuffer(gotLootSoundBuffer);
	gotLootSound.setVolume(80);

	gotArmorSoundBuffer.loadFromFile("Sound/gotarmor.wav");
	gotArmorSound.setBuffer(gotArmorSoundBuffer);
	gotArmorSound.setVolume(10);

	gotHeartSoundBuffer.loadFromFile("Sound/gotheart.wav");
	gotHeartSound.setBuffer(gotHeartSoundBuffer);
	gotHeartSound.setVolume(50);

	bombExplodeSoundBuffer.loadFromFile("Sound/bombexplode.wav");
	bombExplodeSound.setBuffer(bombExplodeSoundBuffer);
	bombExplodeSound.setVolume(50);

	playerJumpBuffer.loadFromFile("Sound/playerjump.wav");
	playerJumpSound.setBuffer(playerJumpBuffer);
	playerJumpSound.setVolume(15);
}

sf::Sound SoundFX::returnBombExoplodeSound()
{
	return bombExplodeSound;
}