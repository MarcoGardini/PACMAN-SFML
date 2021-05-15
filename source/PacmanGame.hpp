#pragma once

#include "Constants.hpp"

class GraphicSystem;
class AudioSystem;
class MunchPlayer;
class GameObject;
class MazeGameObject;
class GridGameObject;
class DotGameObject;
class LargeDotGameObject;
class FruitGameObject;
class PacmanGameObject;
class GhostGameObject;
class BlinkyGameObject;
class PinkyGameObject;
class InkyGameObject;
class ClydeGameObject;
class TargetGameObject;
class BackgroundMusicObject;
class StandardTextObject;
class AutoUpdateTextObject;
class ScoreTextObject;

class PacmanGame
{
public:
	// Constructor
	PacmanGame();
	// Start the game
	void Play();

	void ResetLevel(bool isLevelUp);
	// Manages input and events from OS
	void ManageOSEvents();
	// React to GameEvents and send them to GameObjects
	void PropagateGameEvent(GameEvent gameEvent);
	
	void CheckLevelState();

	int GetLevelValue(LevelValues valueToGet);
	// Activates the fruit based on the remaining dots
	void CheckFruit();

	void FreezeGameObjects(int ms);

	const MAZE_TYPE& getMazeTiles();

public:
	// Global graphic manager
	GraphicSystem* m_graphicSystem;

	// Global audio manager
	AudioSystem* m_audioSystem;
	// Player for alternated munches
	MunchPlayer* m_munchPlayer; 
	
		
	std::vector<GameObject*> m_objectPool;

	MazeGameObject* m_maze;
	GridGameObject* m_grid;
	PacmanGameObject* m_pacman;
	BlinkyGameObject* m_blinky;
	PinkyGameObject* m_pinky;
	InkyGameObject* m_inky;
	ClydeGameObject* m_clyde;
	std::vector<GhostGameObject*>  m_ghosts;
	std::vector<DotGameObject*> m_dots;
	std::vector<LargeDotGameObject*>  m_largeDots;
	std::vector<TargetGameObject*>  m_targets;
	FruitGameObject* m_fruit;
	BackgroundMusicObject* m_backgroundAudio;

	StandardTextObject* m_txtScoreTitle;	
	StandardTextObject* m_txtFruitPoints;
	StandardTextObject* m_txtGhostScore;
	StandardTextObject* m_txtReady;
	StandardTextObject* m_txtGameOver;
	AutoUpdateTextObject* m_txtLives;
	AutoUpdateTextObject* m_txtLevel;
	ScoreTextObject* m_txtScore;

	int m_level;							 // current level
	LevelState m_levelState;				 // current state
	LevelState m_levelStateWhenFrozen;		 // state befor frozing
	GhostMode m_globalGhostMode;			 // global ghost mode, if scatter or chase
	Timer m_globalTimer;					 // general timer for animations and misc
	int m_timerTarget;						 // global target for timer (ie: after x seconds something happens)
	Timer m_freezeTimer;					 // freeze timer
	int m_freezeTarget;						 // freeze target, aka how long will everything stay frozen
	Timer m_ghostModeTimer;					 // timer for ghost modes, based on level value
	int m_numberOfModeChanges;				 // after the 5th change, the mode will always stay chase
	int m_ghostsCurrentlyFrightened;		 // needed for background music and timers
	int m_score;							 // current score
	int m_scoreToGainLife;					 // score needed to gain a life
	int m_activeDots;						 // current active dots, for limits
};