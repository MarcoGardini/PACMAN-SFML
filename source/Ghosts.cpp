#include "Ghosts.hpp"

#include "PacmanGame.hpp"
#include "Graphics.hpp"
#include "Pacman.hpp"
#include "Miscellaneous.hpp"
#include "Text.hpp"

GhostGameObject::GhostGameObject(PacmanGame* pacmanGame, GhostName ghostNameIn) :
	GameObject(pacmanGame->m_graphicSystem->m_componentPool[ass_Blinky + ghostNameIn], true, false, FRONT_OF_DOOR, dir_Up), 
	m_ghostName(ghostNameIn), m_scatterHomeTile(SCATTER_HOMES[ghostNameIn]), m_ghostMode(INITIAL_GHOST_MODE[ghostNameIn])
{	
	m_shortCutTurn = false;	
	m_position = STARTING_POSITIONS[m_ghostName];
	setGhostMode(pacmanGame, GhostMode::Caged);
	setTimeToSpawn(pacmanGame);
}

void GhostGameObject::Update(PacmanGame* pacmanGame)
{
	if (m_moving && !m_frozen)
		UpdatePosition(pacmanGame);
	
	GameObject::Update(pacmanGame);
}

bool GhostGameObject::UpdatePosition(PacmanGame* pacmanGame)
{
	bool result = false;

	int frameMovement = SPEEDS_ARRAY[m_speed][m_speedStep++];
	if (m_speedStep == MAX_SPEED_STEPS) m_speedStep = 0;

	pacmanGame->m_backgroundAudio->Unmute(); // as Ghosts move, the background must be playing

	// check behaviour only if the ghost should move, avoids multiple(illegal) direction changes	
	while (frameMovement-- > 0)
	{		
		switch (m_ghostMode)
		{
		case GhostMode::Caged:
			CagedRoutine(pacmanGame);
			break;
		case GhostMode::Spawning:
			LeaveFromCage(pacmanGame);
			break;
		case GhostMode::Resetting:
			ResetToStartingPosition(pacmanGame);
			break;
		case GhostMode::Retreating:
			ReachFrontOfDoor(pacmanGame);
			setDirectionToTarget(pacmanGame->getMazeTiles());
			break;
		case GhostMode::Frightened:
			CheckFrightenedStatus(pacmanGame);
			break;
		case GhostMode::Scatter:
			m_targetTile = m_scatterHomeTile;
			CheckPacmanPosition(pacmanGame);
			setDirectionToTarget(pacmanGame->getMazeTiles());
			break;
		case GhostMode::Chase:
			calcChaseTarget(pacmanGame);
			CheckPacmanPosition(pacmanGame);
			setDirectionToTarget(pacmanGame->getMazeTiles());
			break;
		}
		result = GameObject::UpdatePosition(pacmanGame);		
	}
	setGhostSpeed(pacmanGame);
	
	return result;
}

void GhostGameObject::ManageEvent(PacmanGame* pacmanGame, GameEvent event)
{
	switch (event)
	{
	case GameEvent::event_GhostSwapMode:
		if (m_ghostMode == GhostMode::Scatter)
		{
			setGhostMode(pacmanGame, GhostMode::Chase);
		}
		else if (m_ghostMode == GhostMode::Chase)
		{
			setGhostMode(pacmanGame, GhostMode::Scatter);
		}
		break;

	case GameEvent::event_LargeDotEaten:
		setGhostMode(pacmanGame, GhostMode::Frightened);
	
	case GameEvent::event_LevelStart:
		StartMoving();
		setTimeToSpawn(pacmanGame);
		m_internalTimer.Reset();
		break;

	case GameEvent::event_GameObjectFreeze:
		Freeze();
		m_internalTimer.Pause();
		break;

	case GameEvent::event_GameObjectUnfreeze:
		Unfreeze();
		StartMoving();
		m_internalTimer.Resume();
		break;

	case GameEvent::event_PacmanHit:
		StopMoving();
		m_active = false;		
		break;

	case GameEvent::event_LevelWon:
		StopMoving();
		m_active = false;		
		break;

	case GameEvent::event_LevelReset:
		m_active = true;	
		StopMoving();
		m_position = STARTING_POSITIONS[m_ghostName];
		m_internalTimer.Reset();
		setGhostMode(pacmanGame, GhostMode::Caged);
		m_direction = dir_Up;
		break;
	}	
}

void GhostGameObject::setGhostMode(PacmanGame* pacmanGame, GhostMode mode)
{
	switch (mode)
	{
	case GhostMode::Caged:
		m_ghostMode = mode;
		m_lowestWalkableTile = TileType::tile_Cage;
		if (m_graphicComponent != pacmanGame->m_graphicSystem->m_componentPool[ass_Blinky + m_ghostName])
			m_graphicComponent = pacmanGame->m_graphicSystem->m_componentPool[ass_Blinky + m_ghostName];
		break;
	case GhostMode::Spawning:
		m_ghostMode = mode;
		if (m_graphicComponent != pacmanGame->m_graphicSystem->m_componentPool[ass_Blinky + m_ghostName])
			m_graphicComponent = pacmanGame->m_graphicSystem->m_componentPool[ass_Blinky + m_ghostName];
		break;
	case GhostMode::Resetting:
		m_direction = dir_Down;
		m_lowestWalkableTile = TileType::tile_Cage;
		m_ghostMode = mode;
		break;
	case GhostMode::Retreating:
		m_graphicComponent = pacmanGame->m_graphicSystem->m_componentPool[ass_Retreating];
		m_ghostMode = mode;
		m_speed = GHOST_RETREATING_SPEED;
		break;

	case GhostMode::Frightened:
		if (m_ghostMode == GhostMode::Scatter || m_ghostMode == GhostMode::Chase || m_ghostMode == GhostMode::Frightened)
		{
			if (m_ghostMode != GhostMode::Frightened)
			{
				m_desiredDirection = getReverseDirection(m_direction);
				m_direction = getReverseDirection(m_direction);
				pacmanGame->m_ghostsCurrentlyFrightened++;
			}

			if (m_graphicComponent != pacmanGame->m_graphicSystem->m_componentPool[ass_FrightenedBlinky + m_ghostName])
				m_graphicComponent = pacmanGame->m_graphicSystem->m_componentPool[ass_FrightenedBlinky + m_ghostName];

			m_internalTimer.Reset();
			m_graphicComponent->m_animNumberOfFrames = 2;
			m_graphicComponent->AnimReset();
			m_ghostMode = mode;
		}
		break;

	case GhostMode::Chase:
		if (m_ghostMode == GhostMode::Scatter)
		{
			m_desiredDirection = getReverseDirection(m_direction);
			m_direction = getReverseDirection(m_direction);
			pacmanGame->m_ghostModeTimer.Reset();
		}
		if (m_graphicComponent != pacmanGame->m_graphicSystem->m_componentPool[ass_Blinky + m_ghostName])
			m_graphicComponent = pacmanGame->m_graphicSystem->m_componentPool[ass_Blinky + m_ghostName];
		m_speed = (Speeds)pacmanGame->GetLevelValue(LevelValues::value_GhostSpeed);
		m_lowestWalkableTile = tile_Portal;
		m_ghostMode = mode;
		break;
	case GhostMode::Scatter:
		if (m_ghostMode == GhostMode::Chase)
		{
			m_desiredDirection = getReverseDirection(m_direction);
			m_direction = getReverseDirection(m_direction);
			pacmanGame->m_ghostModeTimer.Reset();
		}
		if (m_graphicComponent != pacmanGame->m_graphicSystem->m_componentPool[ass_Blinky + m_ghostName])
			m_graphicComponent = pacmanGame->m_graphicSystem->m_componentPool[ass_Blinky + m_ghostName];
		m_speed = (Speeds)pacmanGame->GetLevelValue(LevelValues::value_GhostSpeed);
		m_lowestWalkableTile = tile_Portal;
		m_ghostMode = mode;
		break;
	}
}

void GhostGameObject::CheckFrightenedStatus(PacmanGame* pacmanGame)
{
	if (m_position.tilePosition == pacmanGame->m_pacman->m_position.tilePosition) // got eaten, go back to cage
	{
		pacmanGame->m_txtGhostScore->setText(std::to_string(GHOST_POINTS[pacmanGame->m_pacman->m_ghostEatenCurrentFrightened]));
		pacmanGame->m_txtGhostScore->m_position = m_position;
		pacmanGame->m_txtGhostScore->ActivateWithTimeout(GHOST_EATEN_FREEZE_TIME);
		pacmanGame->m_pacman->m_active = false;
		pacmanGame->PropagateGameEvent(GameEvent::event_GhostEaten);
		setGhostMode(pacmanGame, GhostMode::Retreating);
		pacmanGame->m_ghostsCurrentlyFrightened--;
		m_graphicComponent->m_active = false;
	}
	else // check if the frightened time is over
	{		
		if (m_internalTimer.getElapsedMilliseconds() > pacmanGame->GetLevelValue(value_FrightTime))
		{
			if (m_internalTimer.getElapsedMilliseconds() > 
				pacmanGame->GetLevelValue(value_FrightFlashingTime) + pacmanGame->GetLevelValue(value_FrightTime))
			{			
				setGhostMode(pacmanGame, pacmanGame->m_globalGhostMode);
				pacmanGame->m_ghostsCurrentlyFrightened--;
				return;
			}		
			m_graphicComponent->m_animNumberOfFrames = 4; 
		}
		setRandomDirection(pacmanGame->getMazeTiles());
	}		
}

void GhostGameObject::setRandomDirection(const MAZE_TYPE& mazeTiles)
{
	if (!isWarping(mazeTiles))
	{
		Direction backwardDirection = getReverseDirection(m_direction);
		Direction randomDirection = (Direction)(rand() % 4);
		sf::Vector2i nextRandomTile = m_position.tilePosition + getDirectionVector(randomDirection);
		while (randomDirection == backwardDirection || mazeTiles[nextRandomTile.y][nextRandomTile.x] < (int)m_lowestWalkableTile)
		{
			randomDirection = (Direction)(rand() % 4);
			nextRandomTile = m_position.tilePosition + getDirectionVector(randomDirection);
		}
		m_desiredDirection = randomDirection;
	} 
}

void GhostGameObject::setDirectionToTarget(const MAZE_TYPE& mazeTiles)
{
	if (!isWarping(mazeTiles))
	{
		int bestDistance = 9999;
		Direction bestDirectionFound;
		if (std::find(CANT_TURN_TILES.begin(), CANT_TURN_TILES.end(), m_position.tilePosition) == CANT_TURN_TILES.end())
			bestDirectionFound = dir_Up;
		else
			bestDirectionFound = dir_Left;

		sf::Vector2i thisTile;

		for (int thisDirection = bestDirectionFound; thisDirection < MAX_DIRECTION; thisDirection++)
		{
			thisTile = m_position.tilePosition + getDirectionVector((Direction)thisDirection);
			if (mazeTiles[thisTile.y][thisTile.x] < m_lowestWalkableTile || thisDirection == getReverseDirection(m_direction))
				continue;

			int thisDistance = getDistanceSquared(m_targetTile, thisTile);

			if (thisDistance < bestDistance)
			{
				bestDistance = thisDistance;
				bestDirectionFound = (Direction)thisDirection;
			}
		}
		m_desiredDirection = bestDirectionFound;
	}
}

void GhostGameObject::CheckPacmanPosition(PacmanGame* pacmanGame)
{
	if (m_position.tilePosition == pacmanGame->m_pacman->m_position.tilePosition)
	{
		pacmanGame->PropagateGameEvent(GameEvent::event_PacmanHit);
	}
}

void GhostGameObject::CagedRoutine(PacmanGame* pacmanGame)
{
	if (m_internalTimer.getElapsedMilliseconds() > m_timerTarget)
	{
		setGhostMode(pacmanGame, GhostMode::Spawning);
		return;
	}
	if (m_position.inTilePosition.y == TILE_DIMENSIONS.y - 1)
		m_direction = dir_Up;
	else if (m_position.inTilePosition.y == 0)
		m_direction = dir_Down;
}

void GhostGameObject::setTimeToSpawn(PacmanGame* pacmanGame)
{	
	if (pacmanGame->m_levelState == LevelState::state_LevelStartAfterDeath)
		m_timerTarget = RESPAWN_TIMES[m_ghostName];
	else if (pacmanGame->m_level >= 3)
		m_timerTarget = 0;
	else
		m_timerTarget = SPAWN_TIMES[pacmanGame->m_level][m_ghostName];
}

void GhostGameObject::ReachFrontOfDoor(PacmanGame* pacmanGame)
{
	if (!m_graphicComponent->m_active)
		m_graphicComponent->m_active = true;
	if (m_position.absPosition == FRONT_OF_DOOR)
	{
		setGhostMode(pacmanGame, GhostMode::Resetting);
		return;
	}
	m_targetTile = getTileFromAbsPosition(FRONT_OF_DOOR);
}

void GhostGameObject::LeaveFromCage(PacmanGame* pacmanGame)
{
	if (m_position.absPosition == FRONT_OF_DOOR)
	{
		m_direction = dir_Left;
		setGhostMode(pacmanGame, pacmanGame->m_globalGhostMode);
	}
	else if (m_position.absPosition.x == FRONT_OF_DOOR.x)
	{
		m_direction = dir_Up; m_desiredDirection = dir_Up;
	}
	else if (m_position.absPosition.x < FRONT_OF_DOOR.x)
	{
		m_direction = dir_Right; m_desiredDirection = dir_Right;
	}
	else
	{
		m_direction = dir_Left; m_desiredDirection = dir_Left;
	}
}

void GhostGameObject::setGhostSpeed(PacmanGame* pacmanGame)
{
	if (m_ghostMode == GhostMode::Retreating || m_ghostMode == GhostMode::Resetting)
		m_speed = GHOST_RETREATING_SPEED;
	else
	{
		if (isWarping(pacmanGame->getMazeTiles()))
			m_speed = (Speeds)pacmanGame->GetLevelValue(LevelValues::value_GhostPortalSpeed);
		else switch (m_ghostMode)
		{
		case GhostMode::Caged:
		case GhostMode::Spawning:
			m_speed = GHOST_CAGED_SPAWNING_SPEED;
			break;
		case GhostMode::Frightened:
			m_speed = (Speeds)pacmanGame->GetLevelValue(LevelValues::value_FrightGhostSpeed);
			break;
		case GhostMode::Chase:
		case GhostMode::Scatter:
			m_speed = (Speeds)pacmanGame->GetLevelValue(LevelValues::value_GhostSpeed);
			break;
		}		
	}
}

void GhostGameObject::ResetToStartingPosition(PacmanGame* pacmanGame)
{
	sf::Vector2i resetPosition = STARTING_POSITIONS[m_ghostName == GhostName::ghost_Blinky ? GhostName::ghost_Pinky : m_ghostName];
	if (m_position.absPosition == resetPosition)
	{
		setGhostMode(pacmanGame, GhostMode::Spawning);
	}
	else if (m_position.absPosition.y < resetPosition.y)
	{
		m_direction = dir_Down; m_desiredDirection = dir_Down;
	}
	else if (m_position.absPosition.x < resetPosition.x)
	{
		m_direction = dir_Right; m_desiredDirection = dir_Right;
	}
	else if (m_position.absPosition.x > resetPosition.x)
	{
		m_direction = dir_Left; m_desiredDirection = dir_Left;
	}
}


// SPECIFIC GHOST IMPLEMENTATION

BlinkyGameObject::BlinkyGameObject(PacmanGame* pacmanGame) : GhostGameObject(pacmanGame, GhostName::ghost_Blinky) {}

PinkyGameObject::PinkyGameObject(PacmanGame* pacmanGame) : GhostGameObject(pacmanGame, GhostName::ghost_Pinky) {}

InkyGameObject::InkyGameObject(PacmanGame* pacmanGame) : GhostGameObject(pacmanGame, GhostName::ghost_Inky) {}

ClydeGameObject::ClydeGameObject(PacmanGame* pacmanGame) : GhostGameObject(pacmanGame, GhostName::ghost_Clyde) {}

void BlinkyGameObject::calcChaseTarget(PacmanGame* pacmanGame)
{	
	m_targetTile = pacmanGame->m_pacman->m_position.tilePosition;
}

void PinkyGameObject::calcChaseTarget(PacmanGame* pacmanGame)
{
	Direction pacmanDirection = pacmanGame->m_pacman->m_direction;
	sf::Vector2i directionVector = getDirectionVector(pacmanDirection);
	directionVector *= 4;
	// Reproduce a bug in original code, when pacman looks up, Pinky targets 4 tiles up and 4 left
	if (pacmanDirection == dir_Up)
		directionVector.x -= 4;

	m_targetTile = pacmanGame->m_pacman->m_position.tilePosition + directionVector;
}

void InkyGameObject::calcChaseTarget(PacmanGame* pacmanGame)
{
	Direction pacmanDirection = pacmanGame->m_pacman->m_direction;
	sf::Vector2i directionVector = getDirectionVector(pacmanDirection);
	directionVector *= 2;
	// Reproduce a bug in original code, when pacman looks up, Inky use 2 up 2 left as the center point
	if (pacmanDirection == dir_Up)
		directionVector.x -= 2;

	sf::Vector2i centerPoint(pacmanGame->m_pacman->m_position.tilePosition + directionVector);
	sf::Vector2i blinkyPosition(pacmanGame->m_blinky->m_position.tilePosition);

	m_targetTile = 2 * centerPoint - blinkyPosition;
}

void ClydeGameObject::calcChaseTarget(PacmanGame* pacmanGame)
{
	sf::Vector2i pacmanPosition(pacmanGame->m_pacman->m_position.tilePosition);
	if (getDistanceSquared(pacmanPosition, m_position.tilePosition) > CLYDE_SCARE_DISTANCE_SQUARED)
		m_targetTile = pacmanGame->m_pacman->m_position.tilePosition;
	else m_targetTile = m_scatterHomeTile;
}
