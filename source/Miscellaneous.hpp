#pragma once

#include "Constants.hpp"
#include "GameObject.hpp"

class GameObject;
class GraphicComponent;
class AudioSystem;
class AudioComponent;
class PacmanGame;
class GhostGameObject;
class StandardTextObject;

class MazeGameObject : public GameObject
{
public:
	MazeGameObject(PacmanGame* pacmanGame);

	virtual void Update(PacmanGame* pacmanGame) override;

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override;
	
public:
	MAZE_TYPE m_mazeTiles;
};

class GridGameObject : public GameObject
{
public:
	GridGameObject(PacmanGame* pacmanGame);

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override {};
};

class DotGameObject : public GameObject
{
public:
	DotGameObject(PacmanGame* pacmanGame, sf::Vector2i position);

	virtual void Update(PacmanGame* pacmanGame) override;

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override;
};

class LargeDotGameObject : public GameObject
{
public:
	LargeDotGameObject(PacmanGame* pacmanGame, sf::Vector2i position, Assets largeDotNum);

	virtual void Update(PacmanGame* pacmanGame) override;

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override;
};

class TargetGameObject : public GameObject
{
public:
	TargetGameObject(PacmanGame* pacmanGame, GhostGameObject* ghost, GhostName ghostNameIn);

	virtual void Update(PacmanGame* pacmanGame) override;

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override {};
public:
	GhostGameObject* m_myGhost;
	GhostName m_ghostName;
};

class FruitGameObject : public GameObject
{
public:
	FruitGameObject(PacmanGame* pacmanGame);

	virtual void Update(PacmanGame* pacmanGame) override;

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override {};

	void Wake(PacmanGame* pacmanGame);
};

class BackgroundMusicObject : public GameObject
{
public:
	BackgroundMusicObject(PacmanGame* pacmanGame);

	virtual void Update(PacmanGame* pacmanGame) override;

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override {};

	void PlayMusic(PacmanGame* pacmanGame, Audio audio);
	void Mute();
	void Unmute();

public:
	AudioComponent* m_component;
};
