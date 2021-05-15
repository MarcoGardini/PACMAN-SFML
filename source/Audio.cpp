#include "Audio.hpp"

AudioComponent::AudioComponent(const sf::SoundBuffer* const sb, bool act) :
	m_active(true)
{
	m_sound = new sf::Sound();
	m_sound->setBuffer(*sb);
}

void AudioComponent::Play() const
{
	if (m_active)
		m_sound->play();
}

AudioSystem::AudioSystem()	
{
	for (int i = 0; i < Audio::MAX_AUDIO; i++)
		m_soundBufferPool[i] = new sf::SoundBuffer();

	m_soundBufferPool[Audio::audio_GameStart]->loadFromFile("assets\\game_start.wav");
	m_soundBufferPool[Audio::audio_Death]->loadFromFile("assets\\death.wav");
	m_soundBufferPool[Audio::audio_Munch1]->loadFromFile("assets\\munch_1.wav");
	m_soundBufferPool[Audio::audio_Munch2]->loadFromFile("assets\\munch_2.wav");
	m_soundBufferPool[Audio::audio_EatFruit]->loadFromFile("assets\\eat_fruit.wav");
	m_soundBufferPool[Audio::audio_EatGhost]->loadFromFile("assets\\eat_ghost.wav");
	m_soundBufferPool[Audio::audio_Siren1]->loadFromFile("assets\\siren_1.wav");
	m_soundBufferPool[Audio::audio_Siren2]->loadFromFile("assets\\siren_2.wav");
	m_soundBufferPool[Audio::audio_Siren3]->loadFromFile("assets\\siren_3.wav");
	m_soundBufferPool[Audio::audio_Siren4]->loadFromFile("assets\\siren_4.wav");
	m_soundBufferPool[Audio::audio_Siren5]->loadFromFile("assets\\siren_5.wav");
	m_soundBufferPool[Audio::audio_GhostFrighten]->loadFromFile("assets\\power_pellet.wav");
	m_soundBufferPool[Audio::audio_GhostRetreating]->loadFromFile("assets\\retreating.wav");
	m_soundBufferPool[Audio::audio_Extend]->loadFromFile("assets\\extend.wav");

	for (int i = 0; i < Audio::MAX_AUDIO; i++)
	{
		m_componentPool[i] = new AudioComponent(m_soundBufferPool[i]);
	}
}

void AudioSystem::MuteAll()
{
	for (AudioComponent* comp : m_componentPool)
	{
		comp->m_sound->stop();
		comp->m_active = false;
	}
}

void AudioSystem::UnmuteAll()
{
	for (AudioComponent* comp : m_componentPool)
		comp->m_active = true;
}

MunchPlayer::MunchPlayer(const AudioComponent* munch1, const AudioComponent* munch2) :
	m_munch1(munch1), m_munch2(munch2), m_munchToPlay(false)
{
}

void MunchPlayer::PlayMunch()
{
	if (m_munchToPlay)
		m_munch1->Play();
	else
		m_munch2->Play();

	m_munchToPlay = !m_munchToPlay;
}
