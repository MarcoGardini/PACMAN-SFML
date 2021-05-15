#include "PacmanGame.hpp"

#include "Graphics.hpp"
#include "Audio.hpp"
#include "GameObject.hpp"
#include "Pacman.hpp"
#include "Ghosts.hpp"
#include "Miscellaneous.hpp"
#include "Text.hpp"

void PacmanGame::Play()
{
	while (m_graphicSystem->m_window.isOpen())
	{
		ManageOSEvents();

		m_graphicSystem->m_window.clear();		

		for (GameObject* gameObject : m_objectPool)
			if (gameObject->m_active) gameObject->Update(this);
		
		CheckLevelState();		

		m_graphicSystem->Draw();
	}	
}

void PacmanGame::ManageOSEvents()
{
	sf::Event event;
	while (m_graphicSystem->m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_graphicSystem->m_window.close();
		if (event.type == sf::Event::KeyPressed)
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				m_graphicSystem->m_window.close();
				break;
			case sf::Keyboard::M:
				m_audioSystem->MuteAll();
				break;
			case sf::Keyboard::N:
				m_audioSystem->UnmuteAll();
				break;
			case sf::Keyboard::T:
				for (TargetGameObject* target : m_targets)
					target->m_active = !target->m_active;
				break;
			case sf::Keyboard::G:
				m_grid->m_active = !m_grid->m_active;
				break;
			case sf::Keyboard::Left:
			case sf::Keyboard::A:
				m_pacman->m_desiredDirection = dir_Left;
				break;
			case sf::Keyboard::Right:
			case sf::Keyboard::D:
				m_pacman->m_desiredDirection = dir_Right;
				break;
			case sf::Keyboard::Up:
			case sf::Keyboard::W:
				m_pacman->m_desiredDirection = dir_Up;
				break;
			case sf::Keyboard::Down:
			case sf::Keyboard::S:
				m_pacman->m_desiredDirection = dir_Down;
				break;
			}			
	}
}

void PacmanGame::ResetLevel(bool isLevelUp)
{
	// After animation, reset level or gameover
	if (m_pacman->m_lives < 0)
	{
		PropagateGameEvent(GameEvent::event_GameOver);
	}
	else
	{
		if (isLevelUp)
		{
			m_level++;
			m_ghostModeTimer.Reset();
			m_globalTimer.Reset();
			m_numberOfModeChanges = 0;
			m_globalGhostMode = GhostMode::Scatter;
			m_activeDots = STARTING_DOTS;
			m_levelState = LevelState::state_LevelStartLevelUp;
		}
		else
		{			
			m_levelState = LevelState::state_LevelStartAfterDeath;
		}

		m_ghostModeTimer.Pause();
		m_timerTarget = m_globalTimer.getElapsedMilliseconds() + LEVEL_STARTING_TIME;
		m_globalTimer.Resume();
		m_ghostsCurrentlyFrightened = 0;

		PropagateGameEvent(GameEvent::event_LevelReset);
	}
}

void PacmanGame::PropagateGameEvent(GameEvent gameEvent)
{
	// Manage gameEvent globally
	switch(gameEvent)
	{
	case GameEvent::event_LevelStart:
		m_ghostModeTimer.Resume();
		m_levelState = LevelState::state_LevelRunning;		
		break;
	case GameEvent::event_DotEaten:
		m_score += DOT_POINTS;
		m_activeDots--;
		CheckFruit();
		break;
	case GameEvent::event_LargeDotEaten:
		m_score += LARGEDOT_POINTS;
		CheckFruit();
		m_ghostModeTimer.Pause();
		m_activeDots--;
		break;
	case GameEvent::event_GhostEaten:
		m_score += GHOST_POINTS[m_pacman->m_ghostEatenCurrentFrightened];
		m_audioSystem->m_componentPool[Audio::audio_EatGhost]->Play();
		FreezeGameObjects(GHOST_EATEN_FREEZE_TIME);
		break;
	case GameEvent::event_PacmanHit:		
		m_globalTimer.Pause();
		m_ghostModeTimer.Pause();
		m_backgroundAudio->Mute();
		m_audioSystem->m_componentPool[Audio::audio_Death]->Play();
		m_levelState = LevelState::state_PacmanDead;
		m_timerTarget = m_globalTimer.getElapsedMilliseconds() + PACMAN_DEAD_ANIM_TIME;
		FreezeGameObjects(PACMAN_DEAD_FREEZE_TIME);
		break;
	case GameEvent::event_GameOver:
		m_timerTarget = GAMEOVER_WAIT_TIME;
		m_txtGameOver->ActivateWithTimeout(GAMEOVER_WAIT_TIME);
		m_levelState = LevelState::state_GameOver;
		m_globalTimer.Reset();
		break;
	case GameEvent::event_LevelWon:
		m_timerTarget = LEVEL_WON_TIME;
		m_globalTimer.Reset();
		m_backgroundAudio->Mute();
		m_levelState = LevelState::state_LevelWon;
		FreezeGameObjects(LEVEL_WON_TIME);
		break;
	case GameEvent::event_GameObjectUnfreeze:		
		if (m_ghostsCurrentlyFrightened == 0 && m_levelState == LevelState::state_LevelRunning)
		{
			m_ghostModeTimer.Resume();
		}
		if (!m_pacman->m_active)
			m_pacman->m_active = true;
		break;
	}

	// Propagate to game objects
	for (GameObject* gameObject : m_objectPool)
		gameObject->ManageEvent(this, gameEvent);
}

void PacmanGame::FreezeGameObjects(int ms)
{
	m_freezeTimer.Reset();
	m_globalTimer.Pause();
	m_ghostModeTimer.Pause();
	m_levelStateWhenFrozen = m_levelState;
	m_levelState = LevelState::state_Frozen;
	m_freezeTarget = ms;

	PropagateGameEvent(GameEvent::event_GameObjectFreeze);
}

void PacmanGame::CheckLevelState()
{
	switch (m_levelState)
	{
	case LevelState::state_Frozen:
		if (m_freezeTimer.getElapsedMilliseconds() >= m_freezeTarget)
		{
			m_levelState = m_levelStateWhenFrozen;
			m_globalTimer.Resume();
			PropagateGameEvent(GameEvent::event_GameObjectUnfreeze);
		}
		break;

	case LevelState::state_PacmanDead:
		if (m_globalTimer.getElapsedMilliseconds() > m_timerTarget)
		{
			ResetLevel(false);
		}
		break;

	case LevelState::state_LevelStartAfterDeath:
	case LevelState::state_LevelStartLevelUp:
		if (m_globalTimer.getElapsedMilliseconds() > m_timerTarget)
		{
			PropagateGameEvent(GameEvent::event_LevelStart);
		}
		break;

	case LevelState::state_GameOver:
		if (m_globalTimer.getElapsedMilliseconds() > m_timerTarget)
		{
			m_graphicSystem->m_window.close();
		}
		break;

	case LevelState::state_LevelRunning:	
		if (m_activeDots <= 0)
		{
			PropagateGameEvent(GameEvent::event_LevelWon);
		}	

		if (m_score >= m_scoreToGainLife)
		{
			m_scoreToGainLife += SCORE_NEXT_LIFE;
			m_audioSystem->m_componentPool[Audio::audio_Extend]->Play();
			m_pacman->m_lives++;
		}

		if (m_ghostsCurrentlyFrightened == 0)
			m_ghostModeTimer.Resume();

		if (m_numberOfModeChanges < 5 && m_globalGhostMode == GhostMode::Scatter)
		{
			if (m_ghostModeTimer.getElapsedMilliseconds() > GetLevelValue(LevelValues::value_DurationOfScatter))
			{
				m_numberOfModeChanges++;
				m_ghostModeTimer.Reset();
				m_globalGhostMode = GhostMode::Chase;
				PropagateGameEvent(GameEvent::event_GhostSwapMode);
			}
		}

		if (m_numberOfModeChanges < 5 && m_globalGhostMode == GhostMode::Chase)
		{
			if (m_ghostModeTimer.getElapsedMilliseconds() > GetLevelValue(LevelValues::value_DurationOfChase))
			{
				m_numberOfModeChanges++;
				m_ghostModeTimer.Reset();
				m_globalGhostMode = GhostMode::Scatter;
				PropagateGameEvent(GameEvent::event_GhostSwapMode);
			}
		}
		break;

	case LevelState::state_LevelWon:
		if (m_globalTimer.getElapsedMilliseconds() >= m_timerTarget)
		{			
			ResetLevel(true);
		}
		break;
	}
}

int PacmanGame::GetLevelValue(LevelValues valueToGet)
{
	// after level 21 all levels have the same values
	return LEVELS[m_level >= MAX_LEVEL ? m_level = MAX_LEVEL - 1 : m_level][valueToGet];
}

void PacmanGame::CheckFruit()
{
	for (int m_dots : DOTS_FOR_FRUIT)
	{
		if (m_activeDots == m_dots)
		{
			m_fruit->Wake(this);
		}
	}
}

const MAZE_TYPE& PacmanGame::getMazeTiles()
{
	return m_maze->m_mazeTiles;
}

PacmanGame::PacmanGame() :
	m_level(1), m_levelState(LevelState::state_FirstLevelStart), m_globalGhostMode(GhostMode::Scatter),
	m_score(0), m_scoreToGainLife(SCORE_NEXT_LIFE), m_activeDots{ STARTING_DOTS }, m_ghostsCurrentlyFrightened(0)
{
	// Initialization of all components of the game.

	m_graphicSystem = new GraphicSystem();
	m_audioSystem = new AudioSystem();
	m_munchPlayer = new MunchPlayer(m_audioSystem->m_componentPool[Audio::audio_Munch1], m_audioSystem->m_componentPool[Audio::audio_Munch2]);

	m_maze = new MazeGameObject(this); m_objectPool.push_back(m_maze);
	m_grid = new GridGameObject(this); m_objectPool.push_back(m_grid);

	// Scan the maze and initialize Dots properly
	DotGameObject* thisDot;
	LargeDotGameObject* thisLargeDot;
	Assets thisLargeDotNumber = Assets::ass_LargeDot1;
	for (int i = 0; i < MAZE_TILES_HEIGHT; i++)
	{
		for (int j = 0; j < MAZE_TILES_WIDTH; j++)
		{
			if (getMazeTiles()[i][j] == tile_Dot)
			{
				thisDot = new DotGameObject(this, getCenterPositionFromTile(sf::Vector2i(j, i)));
				m_dots.push_back(thisDot);
				m_objectPool.push_back(thisDot);
			}
			if (getMazeTiles()[i][j] == tile_LargeDot)
			{
				thisLargeDot = new LargeDotGameObject(this, getCenterPositionFromTile(sf::Vector2i(j, i)), thisLargeDotNumber);
				m_largeDots.push_back(thisLargeDot);
				m_objectPool.push_back(thisLargeDot);
				thisLargeDotNumber = (Assets)(thisLargeDotNumber + 1);
			}
		}
	}

	m_pacman = new PacmanGameObject(this); m_objectPool.push_back(m_pacman);
	m_blinky = new BlinkyGameObject(this); m_objectPool.push_back(m_blinky); m_ghosts.push_back(m_blinky);
	m_pinky = new PinkyGameObject(this);   m_objectPool.push_back(m_pinky);  m_ghosts.push_back(m_pinky);
	m_inky = new InkyGameObject(this);     m_objectPool.push_back(m_inky);   m_ghosts.push_back(m_inky);
	m_clyde = new ClydeGameObject(this);   m_objectPool.push_back(m_clyde);  m_ghosts.push_back(m_clyde);
	TargetGameObject* target;
	target = new TargetGameObject(this, m_blinky, ghost_Blinky); m_objectPool.push_back(target); m_targets.push_back(target);
	target = new TargetGameObject(this, m_pinky, ghost_Pinky);   m_objectPool.push_back(target); m_targets.push_back(target);
	target = new TargetGameObject(this, m_inky, ghost_Inky);     m_objectPool.push_back(target); m_targets.push_back(target);
	target = new TargetGameObject(this, m_clyde, ghost_Clyde);   m_objectPool.push_back(target); m_targets.push_back(target);		

	m_backgroundAudio = new BackgroundMusicObject(this); m_objectPool.push_back(m_backgroundAudio);
	m_fruit = new FruitGameObject(this); m_objectPool.push_back(m_fruit);

	m_txtScoreTitle = new StandardTextObject(this, ass_ScoreTitle, STATIC_TEXT_POS[StaticText::text_ScoreTitle]); m_objectPool.push_back(m_txtScoreTitle);
	m_txtFruitPoints = new StandardTextObject(this, ass_FruitPoints, STATIC_TEXT_POS[StaticText::text_FruitPoints]); m_objectPool.push_back(m_txtFruitPoints);
	m_txtFruitPoints->m_active = false;
	m_txtGhostScore = new StandardTextObject(this, ass_GhostScore, sf::Vector2i(0,0)); m_objectPool.push_back(m_txtGhostScore);
	m_txtGhostScore->m_active = false;
	m_txtReady = new StandardTextObject(this, ass_Ready, STATIC_TEXT_POS[StaticText::text_Ready]); m_objectPool.push_back(m_txtReady);
	m_txtGameOver = new StandardTextObject(this, ass_GameOver, STATIC_TEXT_POS[StaticText::text_GameOver]); m_objectPool.push_back(m_txtGameOver);
	m_txtGameOver->m_active = false;
	m_txtLives = new AutoUpdateTextObject(this, ass_Lives, STATIC_TEXT_POS[StaticText::text_Lives], &m_pacman->m_lives, "LIVES: "); m_objectPool.push_back(m_txtLives);
	m_txtLevel = new AutoUpdateTextObject(this, ass_Level, STATIC_TEXT_POS[StaticText::text_Level], &m_level, "LEVEL: "); m_objectPool.push_back(m_txtLevel);
	m_txtScore = new ScoreTextObject(this, ass_Score, STATIC_TEXT_POS[StaticText::text_Score], &m_score); m_objectPool.push_back(m_txtScore);

	// Prepare game for starting

	PropagateGameEvent(GameEvent::event_LevelStart);

	m_audioSystem->m_componentPool[Audio::audio_GameStart]->Play();

	m_txtReady->ActivateWithTimeout(GAME_STARTING_TIME);
	FreezeGameObjects(GAME_STARTING_TIME);
}