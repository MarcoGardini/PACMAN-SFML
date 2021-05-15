#pragma once

#include "Constants.hpp"
#include "GameObject.hpp"

class GameObject;
class GraphicComponent;
class PacmanGame;

class PacmanGameObject : public GameObject
{
public:
	PacmanGameObject(PacmanGame* pacmanGame);

	virtual void Update(PacmanGame* pacmanGame) override;

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override;

public:
	int m_ghostEatenCurrentFrightened; // used for score
	int m_lives;
	bool m_alive;
};