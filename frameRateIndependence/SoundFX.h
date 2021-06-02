#pragma once
#include <SFML/Audio.hpp>

class SoundFX
{
protected:
	sf::SoundBuffer gunShotBuffer;
	sf::Sound gunShotSound;
	sf::SoundBuffer playerDamageSoundBuffer;
	sf::Sound playerDamageSound;
	sf::SoundBuffer gotLootSoundBuffer;
	sf::Sound gotLootSound;
	sf::SoundBuffer gotArmorSoundBuffer;
	sf::Sound gotArmorSound;
	sf::SoundBuffer gotHeartSoundBuffer;
	sf::Sound gotHeartSound;
	sf::SoundBuffer bombExplodeSoundBuffer;
	sf::Sound bombExplodeSound;
	sf::SoundBuffer playerJumpBuffer;
	sf::Sound playerJumpSound;
public:
	SoundFX();
	sf::Sound returnBombExoplodeSound();

};