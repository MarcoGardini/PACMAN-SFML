#include "Pacman.hpp"
#include "PacmanGame.hpp"
#include "Graphics.hpp"
#include "Ghosts.hpp"

PacmanGameObject::PacmanGameObject(PacmanGame* pacmanGame) :
	GameObject(pacmanGame->m_graphicSystem->m_componentPool[ass_Pacman],
		true, false, PACMAN_STARTING_POSITION, dir_Up)
{
	m_speed = (Speeds)pacmanGame->GetLevelValue(LevelValues::value_PacManSpeed);
	m_lives = PACMAN_STARTING_LIVES;
	m_lowestWalkableTile = tile_Portal;
	m_desiredDirection = dir_Left;
	m_alive = true;
	m_shortCutTurn = true;
	m_graphicComponent->AnimPause();
}

void PacmanGameObject::Update(PacmanGame* pacmanGame)
{		
	if (m_alive)
	{
		int frameMovement = SPEEDS_ARRAY[m_speed][m_speedStep++];
		if (m_speedStep == MAX_SPEED_STEPS) m_speedStep = 0;

		while (frameMovement-- > 0 && m_moving && !m_frozen)
		{
			if (UpdatePosition(pacmanGame)) // if pacman's moving, he's animating, else stop to resting frame
			{
				m_graphicComponent->AnimPause();
			}
			else
			{
				m_graphicComponent->AnimPlay();
			}
		}
	}
	GameObject::Update(pacmanGame);
}

void PacmanGameObject::ManageEvent(PacmanGame* pacmanGame, GameEvent event)
{
	switch (event)
	{
	case GameEvent::event_LevelStart:
		m_direction = dir_Left;
		StartMoving();
		break;
	case GameEvent::event_GameObjectFreeze:
		Freeze();
		break;
	case GameEvent::event_GameObjectUnfreeze:
		Unfreeze();
		break;
	case GameEvent::event_LargeDotEaten:
		m_ghostEatenCurrentFrightened = 0;
		break;
	case GameEvent::event_GhostEaten:
		m_ghostEatenCurrentFrightened++;
		break;
	case GameEvent::event_PacmanHit:
		m_direction = dir_Up;
		m_lives--;
		m_graphicComponent = pacmanGame->m_graphicSystem->m_componentPool[ass_DyingPacman];
		m_graphicComponent->AnimReset();
		m_graphicComponent->AnimPlay();
		m_moving = false;
		m_alive = false;		
		break;
	case GameEvent::event_LevelReset:
		m_direction = dir_Up;
		m_graphicComponent = pacmanGame->m_graphicSystem->m_componentPool[ass_Pacman];
		m_moving = false;
		m_graphicComponent->AnimReset();
		m_position = PACMAN_STARTING_POSITION;
		m_alive = true;
		break;
	case GameEvent::event_GameOver:
		m_active = false;
		break;
	case GameEvent::event_LevelWon:
		StopMoving();
		break;
	}
}
