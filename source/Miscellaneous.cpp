#include "Miscellaneous.hpp"
#include "GameObject.hpp"
#include "PacmanGame.hpp"
#include "Graphics.hpp"
#include "Audio.hpp"
#include "Pacman.hpp"
#include "Ghosts.hpp"
#include "Text.hpp"

MazeGameObject::MazeGameObject(PacmanGame* pacmanGame) :
	GameObject(pacmanGame->m_graphicSystem->m_componentPool[ass_Maze],
		true, false, sf::Vector2i(MAZE_PIXELS_WIDTH / 2,
			MAZE_PIXELS_HEIGHT / 2)), 
	m_mazeTiles(MAZE_TILES)
{
}

void MazeGameObject::Update(PacmanGame* pacmanGame)
{
	// Maze flashing when the game is won
	switch (pacmanGame->m_levelState) 
	{
	case LevelState::state_LevelWon:		
		m_graphicComponent->m_animIsPlaying = true;
		break;	
	default:
		m_graphicComponent->m_animIsPlaying = false;
		m_graphicComponent->AnimReset();
		break;
	}	

	GameObject::Update(pacmanGame);
}

void MazeGameObject::ManageEvent(PacmanGame* pacmanGame, GameEvent event)
{
	switch (event)
	{
	case GameEvent::event_LevelReset:
		m_graphicComponent->m_animIsPlaying = false;
		m_graphicComponent->AnimReset();
		break;
	}
}

GridGameObject::GridGameObject(PacmanGame* pacmanGame) :
	GameObject(pacmanGame->m_graphicSystem->m_componentPool[ass_Grid],
		false, false, sf::Vector2i(MAZE_PIXELS_WIDTH / 2,
			MAZE_PIXELS_HEIGHT / 2))
{
}

DotGameObject::DotGameObject(PacmanGame* pacmanGame, sf::Vector2i position) : 
	GameObject(pacmanGame->m_graphicSystem->m_componentPool[ass_Dot],
		true, false, position)
{
}

void DotGameObject::Update(PacmanGame* pacmanGame)
{	
	if (pacmanGame->m_pacman->m_position.tilePosition == m_position.tilePosition)
	{
		m_active = false;
		pacmanGame->m_munchPlayer->PlayMunch();
		pacmanGame->PropagateGameEvent(GameEvent::event_DotEaten);
	}
	GameObject::Update(pacmanGame);
}

void DotGameObject::ManageEvent(PacmanGame* pacmanGame, GameEvent event)
{
	switch (event)
	{
	case GameEvent::event_LevelReset:
		if (pacmanGame->m_levelState != LevelState::state_LevelStartAfterDeath)
			m_active = true;
		break;
	}
}

LargeDotGameObject::LargeDotGameObject(PacmanGame* pacmanGame, sf::Vector2i position, Assets largeDotNum) :
	GameObject(pacmanGame->m_graphicSystem->m_componentPool[largeDotNum],
		true, false, position)
{
	m_graphicComponent->AnimPause();
}

void LargeDotGameObject::Update(PacmanGame* pacmanGame)
{
	if (pacmanGame->m_pacman->m_position.tilePosition == m_position.tilePosition)
	{
		m_active = false;
		pacmanGame->m_munchPlayer->PlayMunch();
		pacmanGame->PropagateGameEvent(GameEvent::event_LargeDotEaten);
	}
	GameObject::Update(pacmanGame);
}

void LargeDotGameObject::ManageEvent(PacmanGame* pacmanGame, GameEvent event)
{
	switch (event)
	{
	case GameEvent::event_LevelStart:
		m_graphicComponent->AnimPlay();
		break;
	case GameEvent::event_LevelReset:
		if (pacmanGame->m_levelState != LevelState::state_LevelStartAfterDeath)
		{
			m_active = true;
		}
		break;
	}
}

TargetGameObject::TargetGameObject(PacmanGame* pacmanGame, GhostGameObject* ghost, GhostName ghostNameIn) :
	GameObject(pacmanGame->m_graphicSystem->m_componentPool[ass_Target],
		false, false, ghost->m_position.absPosition), m_myGhost(ghost), m_ghostName(ghostNameIn)
{
}

void TargetGameObject::Update(PacmanGame* pacmanGame)
{
	m_position = getCenterPositionFromTile(m_myGhost->m_targetTile);
	m_graphicComponent->m_animFrameOffset = m_ghostName;

	GameObject::Update(pacmanGame);
}

BackgroundMusicObject::BackgroundMusicObject(PacmanGame* pacmanGame) :
	GameObject(nullptr, false, false, sf::Vector2i(0,0))
{	
	// As the manager of background music, set all music to be looped and assign my audio to the first Siren
	pacmanGame->m_audioSystem->m_componentPool[Audio::audio_GhostRetreating]->m_sound->setLoop(true);
	pacmanGame->m_audioSystem->m_componentPool[Audio::audio_GhostFrighten]->m_sound->setLoop(true);
	pacmanGame->m_audioSystem->m_componentPool[Audio::audio_Siren1]->m_sound->setLoop(true);
	pacmanGame->m_audioSystem->m_componentPool[Audio::audio_Siren2]->m_sound->setLoop(true);
	pacmanGame->m_audioSystem->m_componentPool[Audio::audio_Siren3]->m_sound->setLoop(true);
	pacmanGame->m_audioSystem->m_componentPool[Audio::audio_Siren4]->m_sound->setLoop(true);
	pacmanGame->m_audioSystem->m_componentPool[Audio::audio_Siren5]->m_sound->setLoop(true);

	m_component = pacmanGame->m_audioSystem->m_componentPool[Audio::audio_Siren1];
}

void BackgroundMusicObject::Update(PacmanGame* pacmanGame)
{
	for (GhostGameObject* ghost : pacmanGame->m_ghosts)
	{
		if (ghost->m_ghostMode == GhostMode::Retreating)
		{
			PlayMusic(pacmanGame, Audio::audio_GhostRetreating);
			return;
		}
	}

	if (pacmanGame->m_ghostsCurrentlyFrightened > 0)
	{
		PlayMusic(pacmanGame, Audio::audio_GhostFrighten);
	}

	else
	{
		for (int i = 0; i < SIREN_DOTS_LIMITS.size(); i++)
		{
			if (pacmanGame->m_activeDots > SIREN_DOTS_LIMITS[i])
			{
				PlayMusic(pacmanGame, (Audio)(Audio::audio_Siren1 + i));
				return;
			}
		}
	}
}

void BackgroundMusicObject::PlayMusic(PacmanGame* pacmanGame, Audio audio)
{
	// If the music to play is already playing just force, else stop current, assign and play
	if (m_component != pacmanGame->m_audioSystem->m_componentPool[audio])
	{
		m_component->m_sound->stop();
		m_component = pacmanGame->m_audioSystem->m_componentPool[audio];
		m_component->Play();
	}
	else if (m_component->m_sound->getStatus() != sf::SoundSource::Status::Playing)
		m_component->Play();
}

void BackgroundMusicObject::Mute()
{
	m_component->m_sound->stop();
	m_active = false;
}

void BackgroundMusicObject::Unmute()
{
	m_active = true;
}

FruitGameObject::FruitGameObject(PacmanGame* pacmanGame) :
	GameObject(pacmanGame->m_graphicSystem->m_componentPool[ass_Fruit], false, false, FRUIT_POSITION)
{
}

void FruitGameObject::Update(PacmanGame* pacmanGame)
{
	if (m_internalTimer.getElapsedMilliseconds() >= m_timerTarget)
	{
		m_active = false;
	}
	else if (pacmanGame->m_pacman->m_position.tilePosition == m_position.tilePosition)
	{
		m_active = false;
		pacmanGame->m_audioSystem->m_componentPool[Audio::audio_EatFruit]->Play();
		pacmanGame->m_txtFruitPoints->ActivateWithTimeout(GHOST_EATEN_FREEZE_TIME);
		pacmanGame->m_txtFruitPoints->setText(std::to_string(pacmanGame->GetLevelValue(LevelValues::value_PointsPerFruit)));
		pacmanGame->m_score += pacmanGame->GetLevelValue(LevelValues::value_PointsPerFruit);
	}
	else 
	{ 
		GameObject::Update(pacmanGame); 
	}
	
}

void FruitGameObject::Wake(PacmanGame* pacmanGame)
{
	// Turned active by PacmanGame only after a precise limit of Dots is reached
	if (!m_active)
	{
		m_graphicComponent->m_animFrameOffset = pacmanGame->GetLevelValue(LevelValues::value_LevelFruit) + 2;
		m_active = true;
		m_internalTimer.Reset();
		m_timerTarget = TIME_FRUIT_ALIVE;
	}
}
