#pragma once

#include "Constants.hpp"
#include "GameObject.hpp"

class GameObject;
class GraphicComponent;
class PacmanGame;
class PacmanGameObject;
class BackgroundMusicObject;

class GhostGameObject : public GameObject
{
public:
	GhostGameObject(PacmanGame* pacmanGame, GhostName ghostNameIn);

	virtual void Update(PacmanGame* pacmanGame) override;

	virtual bool UpdatePosition(PacmanGame* pacmanGame) override; // overridden for ghostMode check

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override;

	void setGhostMode(PacmanGame* pacmanGame, GhostMode mode);

	// List of functions for behaviour
	void setRandomDirection(const MAZE_TYPE& mazeTiles);
	void setDirectionToTarget(const MAZE_TYPE& mazeTiles);
	void CheckPacmanPosition(PacmanGame* pacmanGame);
	void CheckFrightenedStatus(PacmanGame* pacmanGame);
	void CagedRoutine(PacmanGame* pacmanGame);
	void setTimeToSpawn(PacmanGame* pacmanGame);
	void ReachFrontOfDoor(PacmanGame* pacmanGame);
	void LeaveFromCage(PacmanGame* pacmanGame);
	void setGhostSpeed(PacmanGame* pacmanGame);
	void ResetToStartingPosition(PacmanGame* pacmanGame);

	virtual void calcChaseTarget(PacmanGame* pacmanGame) = 0; // each Ghost will override this for its behaviour

public:
	sf::Vector2i m_targetTile;
	sf::Vector2i m_scatterHomeTile;
	GhostMode m_ghostMode;
	GhostName m_ghostName;
};

class BlinkyGameObject : public GhostGameObject
{
public:
	BlinkyGameObject(PacmanGame* pacmanGame);

	virtual void calcChaseTarget(PacmanGame* pacmanGame) override;
};

class PinkyGameObject : public GhostGameObject
{
public:
	PinkyGameObject(PacmanGame* pacmanGame);

	virtual void calcChaseTarget(PacmanGame* pacmanGame) override;
};

class InkyGameObject : public GhostGameObject
{
public:
	InkyGameObject(PacmanGame* pacmanGame);

	virtual void calcChaseTarget(PacmanGame* pacmanGame) override;
};

class ClydeGameObject : public GhostGameObject
{
public:
	ClydeGameObject(PacmanGame* pacmanGame);

	virtual void calcChaseTarget(PacmanGame* pacmanGame) override;
};