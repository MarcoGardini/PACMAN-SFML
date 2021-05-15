#pragma once

#include "Constants.hpp"

class GraphicComponent;
class PacmanGame;

class GameObject
{
public:
	// Constructor
	GameObject(GraphicComponent* graphicIn, bool activeIn, bool movingIn, sf::Vector2i positionIn, Direction directionIn = dir_Up);

	// Called every frame and overridden by inherited classes
	virtual void Update(PacmanGame* pacmanGame);
	
	// Event Manager, each class will define its own based on the event it subscribes to
	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) = 0;

	// Move the object based on rules
	virtual bool UpdatePosition(PacmanGame* pacmanGame);
	
	// Returns wheter the object is on a portal tile or out of bounds
	bool isWarping(const MAZE_TYPE& mazeTiles);

	// Tries to set direction to desiredDirection
	bool ChangeDirection(const MAZE_TYPE& mazeTiles);
	bool ChangeDirection(const MAZE_TYPE& mazeTiles, Direction nextDirection);
	
	void ReverseDirection();

	void StartMoving();
	void StopMoving();

	void Freeze();
	void Unfreeze();

	// In-tile movement
	sf::Vector2i& getSpriteMovementOnTile(MAZE_TYPE maze);

public:
	GraphicComponent* m_graphicComponent;

	Position m_position;
	Direction m_direction;
	Direction m_desiredDirection;
	bool m_active;
	bool m_moving;
	bool m_frozen;
	bool m_shortCutTurn; // only pacman can cut corners
	TileType m_lowestWalkableTile;
	Speeds m_speed;
	int m_speedStep;
	Timer m_internalTimer;
	int m_timerTarget;
};