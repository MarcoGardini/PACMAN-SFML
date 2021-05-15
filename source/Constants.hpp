#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <array>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>

// -----------------------------------ENUMS----------------------------------- // 

// Mode of Ghost, based on time passed in each level (see tables.h)
enum class GhostMode
{
	Scatter,
	Chase,
	Frightened,
	Resetting,
	Spawning,
	Caged,
	Retreating,
};

// Names of the Ghosts, aka position in ghost arrays
enum GhostName
{
	ghost_Blinky,
	ghost_Pinky,
	ghost_Inky,
	ghost_Clyde,
	MAX_GHOSTNAME,
};

// Directions. The order is deliberate.
enum Direction
{
	dir_Up,
	dir_Left,
	dir_Down,
	dir_Right,
	MAX_DIRECTION,
};

// Type of each tile, used to define the Maze and which tile moving entities can walk on
enum TileType
{
	tile_Outside, //0
	tile_Wall, //1 	
	tile_Cage, //2
	tile_Door, //3
	tile_Portal, //4
	tile_Empty, //5
	tile_Dot, //6
	tile_LargeDot, //7
	tile_Fruit, //8
};

// Animation behaviour when changing direction
enum class AnimationType
{
	anim_Static,
	anim_DirectionFrames,
	anim_RotatingTexture,
};

// Texture position in texture pool
enum Textures
{
	tex_Maze,
	tex_Grid,
	tex_Targets,
	tex_Pacman,
	tex_Blinky,
	tex_Pinky,
	tex_Inky,
	tex_Clyde,
	tex_Misc,
	tex_Menu,
	tex_Frightened,
	tex_Retreating,
	MAX_TEXTURE,
};

// Asset position in component pools
enum Assets
{
	ass_Maze,
	ass_Grid,
	ass_Pacman,
	ass_DyingPacman,
	ass_Blinky,
	ass_Pinky,
	ass_Inky,
	ass_Clyde,
	ass_BlinkyTarget,
	ass_PinkyTarget,
	ass_InkyTarget,
	ass_ClydeTarget,
	ass_Target,
	ass_FrightenedBlinky,
	ass_FrightenedPinky,
	ass_FrightenedInky,
	ass_FrightenedClyde,
	ass_Retreating,
	ass_Dot,
	ass_LargeDot1,
	ass_LargeDot2,
	ass_LargeDot3,
	ass_LargeDot4,
	ass_Fruit,
	ass_ScoreTitle,
	ass_Score,
	ass_GhostScore,
	ass_Lives,
	ass_Level,
	ass_FruitPoints,
	ass_Ready,
	ass_GameOver,
	MAX_ASSET,
};

// Audio position in audio pool
enum Audio
{
	audio_GameStart,
	audio_Death,
	audio_Munch1,
	audio_Munch2,
	audio_EatFruit,
	audio_EatGhost,
	audio_Siren1,
	audio_Siren2,
	audio_Siren3,
	audio_Siren4,
	audio_Siren5,
	audio_GhostFrighten,
	audio_GhostRetreating,
	audio_Extend,
	MAX_AUDIO,
};

// Text which doesn't change position, declared for position array
enum StaticText
{
	text_ScoreTitle,
	text_Score,
	text_Lives,
	text_Level,
	text_FruitPoints,
	text_Ready,
	text_GameOver,
	MAX_STATICTEXT,
};

// Possible state of the level
enum class LevelState
{
	state_FirstLevelStart,
	state_LevelStartAfterDeath,
	state_LevelStartLevelUp,
	state_LevelRunning,
	state_PacmanDead,
	state_LevelWon,
	state_GameOver,
	state_Frozen,
	MAX_LEVELMODE,
};

// Possible events in-game
enum class GameEvent
{
	event_GhostEaten,
	event_GhostSwapMode,
	event_PacmanHit,
	event_DotEaten,
	event_LargeDotEaten,
	event_LevelStart,
	event_GameObjectFreeze,
	event_GameObjectUnfreeze,
	event_LevelReset,
	event_LevelWon,
	event_GameOver,
	MAX_EVENT,
};

// Possible speeds in the game
enum Speeds
{
	Speed5,
	Speed10,
	Speed15,
	Speed20,
	Speed25,
	Speed30,
	Speed35,
	Speed40,
	Speed45,
	Speed50,
	Speed55,
	Speed60,
	Speed65,
	Speed70,
	Speed75,
	Speed80,
	Speed85,
	Speed90,
	Speed95,
	Speed100,
	Speed105,
	Speed110,
	Speed150,
	Speed200,
	MAX_SPEEDS,
};

//Bonus symbol for each level
enum BonusSymbols
{
	sym_Cherries,
	sym_Strawberry,
	sym_Peach,
	sym_Apple,
	sym_Grapes,
	sym_Galaxian,
	sym_Bell,
	sym_Key,
	MAX_BONUSSYMBOLS
};

// Values needed for each level
enum LevelValues
{
	value_LevelFruit,
	value_PointsPerFruit,
	value_PacManSpeed,
	value_GhostSpeed,
	value_GhostPortalSpeed,
	value_ElroyDots,
	value_ElroySpeed,
	value_FrightPacmanSpeed,
	value_FrightGhostSpeed,
	value_FrightTime,
	value_FrightFlashingTime,
	value_DurationOfScatter,
	value_DurationOfChase,	
	MAX_LEVELVALUES,
};

// -----------------------------------CONSTANTS----------------------------------- // 

// GRAPHIC / SIZES

// Window scaling factor
const int SCALE_FACTOR{ 2 };
// Width and height of the Maze in tiles
const int MAZE_TILES_WIDTH{ 28 };
const int MAZE_TILES_HEIGHT{ 36 };
// Width and height of the screen in pixels
const int MAZE_PIXELS_WIDTH{ 224 };
const int MAZE_PIXELS_HEIGHT{ 288 };
// Title
const std::string GAME_TITLE = "PACMAN SFML";

// Framerate
const int FRAME_RATE{ 60 };

// Font Sizes
const int FONTSIZE{ 7 };
const int SMALLFONTSIZE{ 6 };

// How many levels are defined
const int MAX_LEVEL{ 22 };

// GAMEPLAY

// Points received
const int DOT_POINTS{ 10 };
const int LARGEDOT_POINTS{ 50 };
const int GHOST_POINTS[4]{ 200, 400, 800, 1600 };
const int SCORE_NEXT_LIFE{ 10000 };

// Dimensions
const sf::Vector2i TILE_DIMENSIONS{ 8,8 };
const sf::Vector2i TILE_HALF_POINT{ TILE_DIMENSIONS / 2 };
const sf::Vector2i SPRITE_DIMENSIONS{ 14, 14 };

// Timings
const int GAME_STARTING_TIME{ 4200 }; 
const int LEVEL_STARTING_TIME{ 2000 };
const int GAMEOVER_WAIT_TIME{ 4000 };
const int GHOST_EATEN_FREEZE_TIME{ 1000 };
const int PACMAN_DEAD_FREEZE_TIME{ 1500 };
const int PACMAN_DEAD_ANIM_TIME{ 3800 };
const int LEVEL_WON_TIME{ 2000 };
const int TIME_FRUIT_ALIVE{ 9500 };

const std::array<std::array<int, MAX_GHOSTNAME>, 3> SPAWN_TIMES
{ {
	{ 0, 0,	   0,	  0 },
	{ 0, 0, 4000, 12000 },
	{ 0, 0,	   0,  7000 },
} };

const std::array<int, MAX_GHOSTNAME> RESPAWN_TIMES
{
	0, 
	5000,
	10000,
	15000,
};

// Positions
const sf::Vector2i PACMAN_STARTING_POSITION{ 112, 212 };
const sf::Vector2i FRUIT_POSITION{ 112, 164 };
const sf::Vector2i FRONT_OF_DOOR{ 112, 116 };

const std::array<sf::Vector2i, MAX_STATICTEXT> STATIC_TEXT_POS
{
	sf::Vector2i(MAZE_PIXELS_WIDTH / 2, 1),			//ScoreTitle,
	sf::Vector2i(MAZE_PIXELS_WIDTH / 2, 10),		//Score,
	sf::Vector2i(1, 273),							//Lives,
	sf::Vector2i(1, 1),								//Level,
	sf::Vector2i(FRUIT_POSITION),					//FruitPoints
	sf::Vector2i(FRUIT_POSITION),					//Ready,
	sf::Vector2i(FRUIT_POSITION),					//GameOver,			
};

const std::array<sf::Vector2i, 4> CANT_TURN_TILES // Special tiles where ghosts can't change direction
{
	sf::Vector2i(12, 14),
	sf::Vector2i(12, 26),
	sf::Vector2i(15, 14),
	sf::Vector2i(15, 26),
};

const std::array<sf::Vector2i, MAX_GHOSTNAME> SCATTER_HOMES
{
	sf::Vector2i(MAZE_TILES_WIDTH - 4, 0),
	sf::Vector2i(3, 0),
	sf::Vector2i(MAZE_TILES_WIDTH - 2, MAZE_TILES_HEIGHT - 1),
	sf::Vector2i(1, MAZE_TILES_HEIGHT - 1),
};

const std::array<sf::Vector2i, MAX_GHOSTNAME> STARTING_POSITIONS
{
	FRONT_OF_DOOR,
	sf::Vector2i(112, 140),
	sf::Vector2i(96, 140),
	sf::Vector2i(128, 140),
};

// Fixed Speeds
const Speeds GHOST_CAGED_SPAWNING_SPEED{ Speed40 };
const Speeds GHOST_RETREATING_SPEED{ Speed150 };

// Limits
const std::array<int, 2> DOTS_FOR_FRUIT{ 174, 74};
const std::array<int, 5> SIREN_DOTS_LIMITS{ 143, 72, 31, 10, 0 };

// Various settings
const int STARTING_DOTS{ 244 };
const int PACMAN_STARTING_LIVES{ 2 };
const int WARPING_ZONE{ 16 }; // Amount of out-of-bounds warping pixels
const int CLYDE_SCARE_DISTANCE_SQUARED{ 8 * 8 };

const std::array<GhostMode, MAX_GHOSTNAME> INITIAL_GHOST_MODE
{
	GhostMode::Spawning,
	GhostMode::Spawning,
	GhostMode::Caged,
	GhostMode::Caged,
};

// VALUES

// Steps in each speed definition
const int MAX_SPEED_STEPS{ 16 };

// Definition of the different speeds. Each step equals 5% of the speed.
const std::array<std::array<int, MAX_SPEED_STEPS>, MAX_SPEEDS> SPEEDS_ARRAY{
	{
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },   //Speed5
		{ 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, },   //Speed10
		{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, },   //Speed15
		{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, },   //Speed20
		{ 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, },   //Speed25
		{ 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, },   //Speed30
		{ 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, },   //Speed35
		{ 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, },   //Speed40
		{ 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, },   //Speed45
		{ 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1, 1, },   //Speed50
		{ 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, },   //Speed55
		{ 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, },   //Speed60
		{ 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, },   //Speed65
		{ 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, },   //Speed70
		{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },   //Speed75
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },   //Speed80
		{ 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },   //Speed85
		{ 2, 1, 1, 1, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, },   //Speed90
		{ 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, },   //Speed95
		{ 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, 2, 1, 1, 1, },   //Speed100
		{ 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 1, 1, },   //Speed105
		{ 2, 1, 1, 2, 1, 1, 2, 1, 1, 2, 1, 2, 1, 2, 1, 1, },   //Speed110
		{ 1, 2, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, },   //Speed150
		{ 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, },   //Speed200
	}
};

// Maze definition
typedef std::array<std::array<int, MAZE_TILES_WIDTH>, MAZE_TILES_HEIGHT> MAZE_TYPE;

const MAZE_TYPE MAZE_TILES =
{ {
		// 0-2 Score
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //0
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //1
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //2
		// 3-3 Wall
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //3
		// 4-16 First Half Maze
		{ 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1 }, //4
		{ 1, 6, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 6, 1 }, //5
		{ 1, 7, 1, 0, 0, 1, 6, 1, 0, 0, 0, 1, 6, 1, 1, 6, 1, 0, 0, 0, 1, 6, 1, 0, 0, 1, 7, 1 }, //6
		{ 1, 6, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 6, 1 }, //7
		{ 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1 }, //8
		{ 1, 6, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 6, 1 }, //9
		{ 1, 6, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 6, 1 }, //10
		{ 1, 6, 6, 6, 6, 6, 6, 1, 1, 6, 6, 6, 6, 1, 1, 6, 6, 6, 6, 1, 1, 6, 6, 6, 6, 6, 6, 1 }, //11
		{ 1, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 5, 1, 1, 5, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 1 }, //12
		{ 0, 0, 0, 0, 0, 1, 6, 1, 1, 1, 1, 1, 5, 1, 1, 5, 1, 1, 1, 1, 1, 6, 1, 0, 0, 0, 0, 0 }, //13
		{ 0, 0, 0, 0, 0, 1, 6, 1, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 6, 1, 0, 0, 0, 0, 0 }, //14
		{ 0, 0, 0, 0, 0, 1, 6, 1, 1, 5, 1, 1, 1, 3, 3, 1, 1, 1, 5, 1, 1, 6, 1, 0, 0, 0, 0, 0 }, //15
		{ 1, 1, 1, 1, 1, 1, 6, 1, 1, 5, 1, 2, 2, 2, 2, 2, 2, 1, 5, 1, 1, 6, 1, 1, 1, 1, 1, 1 }, //16
		// 17-17 Special, warping
		{ 4, 4, 4, 4, 4, 5, 6, 5, 5, 5, 1, 2, 2, 2, 2, 2, 2, 1, 5, 5, 5, 6, 5, 4, 4, 4, 4, 4}, //17
		// 18-32 Second Half Maze
		{ 1, 1, 1, 1, 1, 1, 6, 1, 1, 5, 1, 2, 2, 2, 2, 2, 2, 1, 5, 1, 1, 6, 1, 1, 1, 1, 1, 1 }, //18
		{ 0, 0, 0, 0, 0, 1, 6, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 6, 1, 0, 0, 0, 0, 0 }, //19
		{ 0, 0, 0, 0, 0, 1, 6, 1, 1, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1, 1, 6, 1, 0, 0, 0, 0, 0 }, //20
		{ 0, 0, 0, 0, 0, 1, 6, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 6, 1, 0, 0, 0, 0, 0 }, //21
		{ 1, 1, 1, 1, 1, 1, 6, 1, 1, 5, 1, 1, 1, 1, 1, 1, 1, 1, 5, 1, 1, 6, 1, 1, 1, 1, 1, 1 }, //22
		{ 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1, 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1 }, //23
		{ 1, 6, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 6, 1 }, //24
		{ 1, 6, 1, 1, 1, 1, 6, 1, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 1, 6, 1, 1, 1, 1, 6, 1 }, //25
		{ 1, 7, 6, 6, 1, 1, 6, 6, 6, 6, 6, 6, 6, 8, 8, 6, 6, 6, 6, 6, 6, 6, 1, 1, 6, 6, 7, 1 }, //26
		{ 1, 1, 1, 6, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 6, 1, 1, 1 }, //27
		{ 1, 1, 1, 6, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 6, 1, 1, 1 }, //28
		{ 1, 6, 6, 6, 6, 6, 6, 1, 1, 6, 6, 6, 6, 1, 1, 6, 6, 6, 6, 1, 1, 6, 6, 6, 6, 6, 6, 1 }, //29
		{ 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1 }, //30
		{ 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1, 1, 6, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 6, 1 }, //31
		{ 1, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 1 }, //32
		// 33-33 Wall
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }, //33
		// 34-35 Lives + Level
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //34
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //35
} };

// Definition of levels
const std::array<std::array<int, MAX_LEVELVALUES>, MAX_LEVEL> LEVELS{
	{
		{0,0,0,0,0,0,0,0,0,0,0,0}, //DUMMY LEVEL, AS LEVELS START AT ONE													   // VALUES ARE:	
		{sym_Cherries,   100,  Speed80,  Speed75, Speed40, 20, Speed85,  Speed90,  Speed50, 6000, 2500, 7000, 20000, },        // - value_LevelFruit,
		{sym_Strawberry, 300,  Speed90,  Speed85, Speed45, 30, Speed95,  Speed95,  Speed55, 5000, 2500, 7000, 20000, },		   // - value_PointsPerFruit,
		{sym_Peach,		 500,  Speed90,  Speed85, Speed45, 40, Speed95,  Speed95,  Speed55, 4000, 2500, 7000, 20000, },		   // - value_PacManSpeed,
		{sym_Peach,		 500,  Speed90,  Speed85, Speed45, 40, Speed95,  Speed95,  Speed55, 3000, 2500, 7000, 20000, },		   // - value_GhostSpeed,
		{sym_Apple,		 700,  Speed100, Speed95, Speed50, 40, Speed105, Speed90,  Speed60, 2000, 2500, 7000, 20000, },		   // - value_GhostPortalSpeed,
		{sym_Apple,		 700,  Speed100, Speed95, Speed50, 50, Speed105, Speed100, Speed60, 5000, 2500, 5000, 20000, },		   // - value_ElroyDots,
		{sym_Grapes,	 1000, Speed100, Speed95, Speed50, 50, Speed105, Speed100, Speed60, 2000, 2500, 5000, 20000, },		   // - value_ElroySpeed,
		{sym_Grapes,	 1000, Speed100, Speed95, Speed50, 50, Speed105, Speed100, Speed60, 2000, 2500, 5000, 20000, },		   // - value_FrightPacmanSpeed,
		{sym_Galaxian,	 2000, Speed100, Speed95, Speed50, 60, Speed105, Speed100, Speed60, 1000, 1500, 5000, 20000, },		   // - value_FrightGhostSpeed,
		{sym_Galaxian,	 2000, Speed100, Speed95, Speed50, 60, Speed105, Speed100, Speed60, 5000, 2500, 5000, 20000, },		   // - value_FrightTime,
		{sym_Bell,		 3000, Speed100, Speed95, Speed50, 60, Speed105, Speed100, Speed60, 2000, 2500, 5000, 20000, },		   // - value_FrightFlashingTime,
		{sym_Bell,		 3000, Speed100, Speed95, Speed50, 40, Speed105, Speed100, Speed60, 1000, 1500, 5000, 20000, },		   // - value_DurationOfScatter,
		{sym_Key,		 5000, Speed100, Speed95, Speed50, 40, Speed105, Speed100, Speed60, 1000, 1500, 5000, 20000, },		   // - value_DurationOfChase,
		{sym_Key,		 5000, Speed100, Speed95, Speed50, 40, Speed105, Speed100, Speed60, 3000, 2500, 5000, 20000, },
		{sym_Key,		 5000, Speed100, Speed95, Speed50, 50, Speed105, Speed100, Speed60, 1000, 1500, 5000, 20000, },
		{sym_Key,		 5000, Speed100, Speed95, Speed50, 50, Speed105, Speed100, Speed60, 1000, 1500, 5000, 20000, },
		{sym_Key,		 5000, Speed100, Speed95, Speed50, 50, Speed105, Speed100, Speed60,    0,    0,	5000, 20000, },
		{sym_Key,		 5000, Speed100, Speed95, Speed50, 50, Speed105, Speed100, Speed60, 1000, 1500, 5000, 20000, },
		{sym_Key,		 5000, Speed100, Speed95, Speed50, 60, Speed105, Speed100, Speed60,	   0,    0,	5000, 20000, },
		{sym_Key,		 5000, Speed100, Speed95, Speed50, 60, Speed105, Speed100, Speed60,	   0,    0,	5000, 20000, },
		{sym_Key,		 5000, Speed100, Speed95, Speed50, 60, Speed105, Speed100, Speed60,	   0,    0,	5000, 20000, },
	}
};

// -----------------------------------INLINE UTILS----------------------------------- // 

inline sf::Vector2i getDirectionVector(Direction direction)
{
	switch (direction)
	{
	case dir_Up: return sf::Vector2i(0, -1);
	case dir_Left: return sf::Vector2i(-1, 0);
	case dir_Down: return sf::Vector2i(0, 1);
	case dir_Right: return sf::Vector2i(1, 0);
	}
}

inline Direction getReverseDirection(Direction direction)
{
	return (Direction)(direction < 2 ? direction + 2 : direction - 2);
}

inline sf::Vector2i getCenterPositionFromTile(sf::Vector2i tile)
{
	return sf::Vector2i(tile.x * TILE_DIMENSIONS.x + TILE_HALF_POINT.x,
		tile.y * TILE_DIMENSIONS.y + TILE_HALF_POINT.y);
}

inline int getDistanceSquared(sf::Vector2i position1, sf::Vector2i position2)
{
	sf::Vector2i diffVector(position1 - position2);
	return diffVector.x * diffVector.x + diffVector.y * diffVector.y;
}

inline sf::Vector2i getTileFromAbsPosition(sf::Vector2i absPosition)
{
	return sf::Vector2i(absPosition.x / TILE_DIMENSIONS.x, absPosition.y / TILE_DIMENSIONS.y);
}

// -----------------------------------CLASSES UTILS----------------------------------- // 

class Position
{
public:
	Position(sf::Vector2i positionIn) :
		absPosition(positionIn)
	{
		calculateTilePosition();
	}

	void calculateTilePosition()
	{
		tilePosition = sf::Vector2i(absPosition.x / TILE_DIMENSIONS.x, absPosition.y / TILE_DIMENSIONS.y);
		inTilePosition = sf::Vector2i((int)absPosition.x % TILE_DIMENSIONS.x, (int)absPosition.y % TILE_DIMENSIONS.y);
	}

	Position& operator=(sf::Vector2i positionIn)
	{
		absPosition = positionIn;
		calculateTilePosition();
		return *this;
	}

	Position& operator+=(sf::Vector2i positionOffset)
	{
		absPosition += positionOffset;
		calculateTilePosition();
		return *this;
	}

	Position& operator-=(sf::Vector2i positionOffset)
	{
		absPosition += -positionOffset;
		return *this;
	}

	bool operator==(Position& positionIn)
	{
		return absPosition == positionIn.absPosition;
	}

public:
	sf::Vector2i absPosition;
	sf::Vector2i tilePosition;
	sf::Vector2i inTilePosition;
};

// adding utility to the sf::Clock class
class Timer : public sf::Clock
{
public:
	void Pause()
	{
		if (!m_paused)
		{
			m_millisecondsAtPause = getElapsedTime().asMilliseconds();
			m_paused = true;
		}
	}

	void Resume()
	{
		if (m_paused)
		{
			m_pausedMilliseconds += (getElapsedTime().asMilliseconds() - m_millisecondsAtPause);
			m_paused = false;
		}
	}

	void Reset()
	{
		m_paused = false;
		m_pausedMilliseconds = 0;
		m_millisecondsAtPause = 0;
		restart();
	}

	int getElapsedMilliseconds()
	{
		if (m_paused)
			return m_millisecondsAtPause - m_pausedMilliseconds;

		return getElapsedTime().asMilliseconds() - m_pausedMilliseconds;
	}

private:
	bool m_paused;
	int m_pausedMilliseconds;
	int m_millisecondsAtPause;
};