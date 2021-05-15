#pragma once

#include <SFML/Audio.hpp>
#include <array>
#include <vector>

#include "Constants.hpp"

class AudioComponent
{
public:
	// Constructor
	AudioComponent(const sf::SoundBuffer* const sb, bool act = true);
		
	void Play() const;

public:
	bool m_active;
		
	sf::Sound* m_sound;	
};

class MunchPlayer
{
public:
	// Constructor
	MunchPlayer(const AudioComponent* munch1, const AudioComponent* munch2);

	// Alternate between the 2 munches
	void PlayMunch();

public:
	const AudioComponent* m_munch1;
	const AudioComponent* m_munch2;

	bool m_munchToPlay;
};

class AudioSystem
{
public:
	// Constructor
	AudioSystem();	

	void MuteAll();
	void UnmuteAll();

public:
	std::array<AudioComponent*, Audio::MAX_AUDIO> m_componentPool;
	std::array<sf::SoundBuffer*, Audio::MAX_AUDIO> m_soundBufferPool;	
};