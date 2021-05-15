#include "Graphics.hpp"

// GRAPHIC SYSTEM

GraphicSystem::GraphicSystem()
{
	// init SFML
	m_window.create(sf::VideoMode(MAZE_PIXELS_WIDTH, MAZE_PIXELS_HEIGHT), GAME_TITLE, sf::Style::Titlebar | sf::Style::Close);
	m_window.setSize(sf::Vector2u(MAZE_PIXELS_WIDTH * SCALE_FACTOR, MAZE_PIXELS_HEIGHT * SCALE_FACTOR));
	m_window.setFramerateLimit(FRAME_RATE);

	// load textures to texture pool, keeping them loaded
	for (int i = 0; i < Textures::MAX_TEXTURE; i++)
		m_texturePool[i] = new sf::Texture();
	m_texturePool[Textures::tex_Maze]->loadFromFile("assets\\maze.png");
	m_texturePool[Textures::tex_Grid]->loadFromFile("assets\\grid.png");
	m_texturePool[Textures::tex_Pacman]->loadFromFile("assets\\pacman.png");
	m_texturePool[Textures::tex_Blinky]->loadFromFile("assets\\blinky.png");
	m_texturePool[Textures::tex_Pinky]->loadFromFile("assets\\pinky.png");
	m_texturePool[Textures::tex_Inky]->loadFromFile("assets\\inky.png");
	m_texturePool[Textures::tex_Clyde]->loadFromFile("assets\\clyde.png");
	m_texturePool[Textures::tex_Misc]->loadFromFile("assets\\misc.png");
	m_texturePool[Textures::tex_Targets]->loadFromFile("assets\\targets.png");
	m_texturePool[Textures::tex_Frightened]->loadFromFile("assets\\frightened.png");
	m_texturePool[Textures::tex_Retreating]->loadFromFile("assets\\retreating.png");

	// generate Graphic Components: prepare frames then create the component in component array
	
	// MAZE
	std::vector<sf::Vector2i> tempFrames;
	tempFrames.push_back(sf::Vector2i(0, 15));
	tempFrames.push_back(sf::Vector2i(1, 15));
	m_componentPool[Assets::ass_Maze] = new GraphicComponent(m_texturePool[Textures::tex_Maze],
		sf::Vector2f(MAZE_PIXELS_WIDTH, MAZE_PIXELS_HEIGHT), AnimationType::anim_Static, false, 2,
		tempFrames, 0);

	// GRID
	tempFrames.clear();
	tempFrames.push_back(sf::Vector2i(0, 0));
	m_componentPool[Assets::ass_Grid] = new GraphicComponent(m_texturePool[Textures::tex_Grid],
		sf::Vector2f(MAZE_PIXELS_WIDTH, MAZE_PIXELS_HEIGHT), AnimationType::anim_Static, false, 1,
		tempFrames, 0);

	// FRUIT (not animated, uses same tempFrames)
	m_componentPool[Assets::ass_Fruit] = new GraphicComponent(m_texturePool[Textures::tex_Misc],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_Static, false, 1,
		tempFrames, 2);

	// TARGETS (not animated, uses same tempFrames)
	m_componentPool[Assets::ass_Target] = new GraphicComponent(m_texturePool[Textures::tex_Targets],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_Static, false, 1,
		tempFrames, 0);

	// DOT (not animated, uses same tempFrames)
	m_componentPool[Assets::ass_Dot] = new GraphicComponent(m_texturePool[Textures::tex_Misc],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_Static, false, 1,
		tempFrames, 0);

	// LARGE DOT(s) (separated, as they can't use the same graphic for animations)
	tempFrames.clear();
	tempFrames.push_back(sf::Vector2i(1, 10));	tempFrames.push_back(sf::Vector2i(10, 10));
	m_componentPool[Assets::ass_LargeDot1] = new GraphicComponent(m_texturePool[Textures::tex_Misc],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_Static, true, 2,
		tempFrames, 0);
	m_componentPool[Assets::ass_LargeDot2] = new GraphicComponent(m_texturePool[Textures::tex_Misc],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_Static, true, 2,
		tempFrames, 0);
	m_componentPool[Assets::ass_LargeDot3] = new GraphicComponent(m_texturePool[Textures::tex_Misc],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_Static, true, 2,
		tempFrames, 0);
	m_componentPool[Assets::ass_LargeDot4] = new GraphicComponent(m_texturePool[Textures::tex_Misc],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_Static, true, 2,
		tempFrames, 0);

	// PACMAN
	tempFrames.clear();
	tempFrames.push_back(sf::Vector2i(0, 2)); tempFrames.push_back(sf::Vector2i(1, 2));
	tempFrames.push_back(sf::Vector2i(2, 2)); tempFrames.push_back(sf::Vector2i(1, 2));
	m_componentPool[Assets::ass_Pacman] = new GraphicComponent(m_texturePool[Textures::tex_Pacman],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_RotatingTexture, false, 4,
		tempFrames, 0, 1);

	// DYING PACMAN
	tempFrames.clear();
	tempFrames.push_back(sf::Vector2i(2, 90)); tempFrames.push_back(sf::Vector2i(3, 9));
	tempFrames.push_back(sf::Vector2i(4, 9)); tempFrames.push_back(sf::Vector2i(5, 9));
	tempFrames.push_back(sf::Vector2i(6, 9)); tempFrames.push_back(sf::Vector2i(7, 9));
	tempFrames.push_back(sf::Vector2i(8, 9)); tempFrames.push_back(sf::Vector2i(9, 9));
	tempFrames.push_back(sf::Vector2i(10, 9)); tempFrames.push_back(sf::Vector2i(11, 9));
	tempFrames.push_back(sf::Vector2i(12, 9)); tempFrames.push_back(sf::Vector2i(13, 150));
	m_componentPool[Assets::ass_DyingPacman] = new GraphicComponent(m_texturePool[Textures::tex_Pacman],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_DirectionFrames, false, 12,
		tempFrames, 0);

	// BLINKY
	tempFrames.clear();
	tempFrames.push_back(sf::Vector2i(0, 8)); tempFrames.push_back(sf::Vector2i(1, 8));
	m_componentPool[Assets::ass_Blinky] = new GraphicComponent(m_texturePool[Textures::tex_Blinky],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_DirectionFrames, false, 2,
		tempFrames, 0);

	// PINKY
	m_componentPool[Assets::ass_Pinky] = new GraphicComponent(m_texturePool[Textures::tex_Pinky],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_DirectionFrames, false, 2,
		tempFrames, 0);

	// INKY
	m_componentPool[Assets::ass_Inky] = new GraphicComponent(m_texturePool[Textures::tex_Inky],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_DirectionFrames, false, 2,
		tempFrames, 0);

	// CLYDE
	m_componentPool[Assets::ass_Clyde] = new GraphicComponent(m_texturePool[Textures::tex_Clyde],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_DirectionFrames, false, 2,
		tempFrames, 0);

	// FRIGHTENED GHOSTS
	tempFrames.push_back(sf::Vector2i(2, 8)); tempFrames.push_back(sf::Vector2i(3, 8));
	m_componentPool[Assets::ass_FrightenedBlinky] = new GraphicComponent(m_texturePool[Textures::tex_Frightened],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_Static, true, 2,
		tempFrames, 0);
	m_componentPool[Assets::ass_FrightenedPinky] = new GraphicComponent(m_texturePool[Textures::tex_Frightened],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_Static, true, 2,
		tempFrames, 0);
	m_componentPool[Assets::ass_FrightenedInky] = new GraphicComponent(m_texturePool[Textures::tex_Frightened],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_Static, true, 2,
		tempFrames, 0);
	m_componentPool[Assets::ass_FrightenedClyde] = new GraphicComponent(m_texturePool[Textures::tex_Frightened],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_Static, true, 2,
		tempFrames, 0);

	// RETREATING
	tempFrames.clear();
	tempFrames.push_back(sf::Vector2i(0, 1)); 
	m_componentPool[Assets::ass_Retreating] = new GraphicComponent(m_texturePool[Textures::tex_Retreating],
		sf::Vector2f(SPRITE_DIMENSIONS.x, SPRITE_DIMENSIONS.y), AnimationType::anim_DirectionFrames, true, 1,
		tempFrames, 0);

	// Generate text components, with different constructor
	m_font = new sf::Font();
	m_font->loadFromFile("assets\\Grand9K Pixel.ttf");
	
	m_componentPool[ass_ScoreTitle]  = new GraphicComponent("SCORE"		, FONTSIZE		 , sf::Color::White  , sf::Vector2f(13, 0), m_font);
	m_componentPool[ass_Score]	     = new GraphicComponent("00000000"  , FONTSIZE		 , sf::Color::White  , sf::Vector2f(24, 0), m_font);
	m_componentPool[ass_GhostScore]  = new GraphicComponent(""			, SMALLFONTSIZE  , sf::Color::Cyan   , sf::Vector2f(6 , 0), m_font);
	m_componentPool[ass_Lives]	     = new GraphicComponent("LIVES: "	, FONTSIZE		 , sf::Color::White  , sf::Vector2f(0 , 0), m_font);
	m_componentPool[ass_Level]	     = new GraphicComponent("LEVEL: "   , FONTSIZE		 , sf::Color::White  , sf::Vector2f(0 , 0), m_font);
	m_componentPool[ass_FruitPoints] = new GraphicComponent(""			, SMALLFONTSIZE  , sf::Color::Magenta, sf::Vector2f(6 , 0), m_font);
	m_componentPool[ass_Ready]	     = new GraphicComponent("READY!"    , FONTSIZE		 , sf::Color::Yellow , sf::Vector2f(15, 3), m_font);
	m_componentPool[ass_GameOver]	 = new GraphicComponent("GAME OVER!", FONTSIZE		 , sf::Color::Red	 , sf::Vector2f(23, 3), m_font);
}

void GraphicSystem::Draw()
{	
	m_window.display();
}


// GRAPHIC COMPONENT

GraphicComponent::GraphicComponent(sf::Texture* const textureIn, const sf::Vector2f& dimensionIn,
	const AnimationType graphicTypeIn, bool isPlaying, int numberOfFrames,
								   std::vector<sf::Vector2i>& framesIn, int frameOffset, int restingFrame) :
	m_texture(textureIn), m_textureDimension(dimensionIn), m_textureRotation(0), m_animDirectionMethod(graphicTypeIn),
	m_animIsPlaying(isPlaying), m_animNumberOfFrames(numberOfFrames), m_animFrames(framesIn),
	m_animFrameOffset(frameOffset), m_animRestingFrame(restingFrame)
{
	m_sprite = new sf::Sprite();
	m_sprite->setTexture(*m_texture);
	m_sprite->setTextureRect(sf::IntRect(frameOffset * m_textureDimension.x, 0, m_textureDimension.x, m_textureDimension.y));
	m_sprite->setOrigin(m_textureDimension.x / 2.0f, m_textureDimension.y / 2.0f);

	m_active = true;

	m_text = nullptr;
}

GraphicComponent::GraphicComponent(const std::string& initText, const int fontDimension, const sf::Color color, const sf::Vector2f origin, const sf::Font* font)
{
	m_text = new sf::Text();
	m_text->setString(initText);
	m_text->setFont(*font);
	m_text->setCharacterSize(fontDimension);
	m_text->setFillColor(color);
	m_text->setOrigin(origin);
	m_sprite = nullptr;
}

void GraphicComponent::Update(const GameObject* gameObject)
{
	if (m_text != nullptr)
	{
		m_text->setPosition(gameObject->m_position.absPosition.x, gameObject->m_position.absPosition.y);
	}

	if (m_sprite != nullptr)
	{
		m_sprite->setPosition(gameObject->m_position.absPosition.x, gameObject->m_position.absPosition.y);

		setDirection(gameObject->m_direction);

		if (m_animIsPlaying)
		{
			if (m_animCurrentFrameRepetition.y >= m_animFrames[m_animCurrentFrameRepetition.x].y)
			{
				m_animCurrentFrameRepetition.y = 0;
				if (++m_animCurrentFrameRepetition.x == m_animNumberOfFrames)
					m_animCurrentFrameRepetition.x = 0;
			}
			m_animCurrentFrameRepetition.y++;
		}

		setCurrentFrameRectangle();
	}
}

void GraphicComponent::setCurrentFrameRectangle()
{
	m_sprite->setTextureRect(sf::IntRect((m_animFrames[m_animCurrentFrameRepetition.x].x + m_animFrameOffset) * m_textureDimension.x,
		0, m_textureDimension.x, m_textureDimension.y));
}

void GraphicComponent::setDirection(const Direction directionIn)
{
	switch (m_animDirectionMethod)
	{
	case AnimationType::anim_RotatingTexture:
		m_sprite->setRotation(360. - (int)directionIn * 90.); break; 
	case AnimationType::anim_DirectionFrames:
		m_animFrameOffset = (int)directionIn * m_animNumberOfFrames; break;
	case AnimationType::anim_Static:
	default:
		break;
	}
}

void GraphicComponent::AnimPlay()
{
	m_animIsPlaying = true;
}

void GraphicComponent::AnimStop()
{
	AnimReset();
	m_animIsPlaying = false;
}

void GraphicComponent::AnimPause()
{
	m_animIsPlaying = false;
	m_animCurrentFrameRepetition = sf::Vector2i(m_animRestingFrame, 0);
}

void GraphicComponent::AnimReset()
{
	m_animCurrentFrameRepetition = sf::Vector2i(0, 0);
}
