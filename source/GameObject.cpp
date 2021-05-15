#include "GameObject.hpp"
#include "Graphics.hpp"
#include "PacmanGame.hpp"

GameObject::GameObject(GraphicComponent* graphicIn, bool activeIn, bool movingIn, sf::Vector2i positionIn, Direction directionIn) :
	m_graphicComponent(graphicIn), m_active(activeIn), m_moving(movingIn), m_position(positionIn), m_direction(directionIn),
	m_desiredDirection(directionIn), m_speedStep{ 0 }
{}

void GameObject::Update(PacmanGame* pacmanGame)
{
	// update and draw the sprite
	if (m_graphicComponent != nullptr && m_graphicComponent->m_active)
	{
		m_graphicComponent->Update(this);
		if (m_graphicComponent->m_sprite != nullptr)
			pacmanGame->m_graphicSystem->m_window.draw(*m_graphicComponent->m_sprite);
		if (m_graphicComponent->m_text != nullptr)
			pacmanGame->m_graphicSystem->m_window.draw(*m_graphicComponent->m_text);
	}
}

bool GameObject::isWarping(const MAZE_TYPE& mazeTiles)
{
	return m_position.tilePosition.x <= 0 || m_position.tilePosition.x >= MAZE_TILES_WIDTH ||
		m_position.tilePosition.y <= 0 || m_position.tilePosition.y >= MAZE_TILES_HEIGHT ||
		MAZE_TILES[m_position.tilePosition.y][m_position.tilePosition.x] == (int)TileType::tile_Portal;
}

bool GameObject::ChangeDirection(const MAZE_TYPE& mazeTiles)
{
	if (isWarping(mazeTiles)) // can change direction only on the same line while warping
	{
		if (m_desiredDirection % 2 == m_direction % 2)
		{
			m_direction = m_desiredDirection;
			return true;
		}
		else 
			return false;
	}
	if (m_shortCutTurn || m_position.inTilePosition == TILE_HALF_POINT)
	{
		sf::Vector2i nextTile = m_position.tilePosition + getDirectionVector(m_desiredDirection);
		if (mazeTiles[nextTile.y][nextTile.x] >= (int)m_lowestWalkableTile)
		{
			m_direction = m_desiredDirection;
			return true;
		}
	}
	return false;
}

bool GameObject::ChangeDirection(const MAZE_TYPE& mazeTiles, Direction nextDirection)
{
	m_desiredDirection = nextDirection;
	return ChangeDirection(mazeTiles);
}

bool GameObject::UpdatePosition(PacmanGame* pacmanGame)
{
	Position oldPosition = m_position;
	if (m_moving)
	{
		ChangeDirection(pacmanGame->getMazeTiles());
		m_position += getSpriteMovementOnTile(pacmanGame->getMazeTiles());
		// warping checks
		if (m_position.absPosition.x > MAZE_PIXELS_WIDTH + WARPING_ZONE) m_position.absPosition = sf::Vector2i(-WARPING_ZONE, m_position.absPosition.y);
		else if (m_position.absPosition.x < -WARPING_ZONE) m_position.absPosition = sf::Vector2i(MAZE_PIXELS_WIDTH + WARPING_ZONE, m_position.absPosition.y);
		else if (m_position.absPosition.y > MAZE_PIXELS_HEIGHT + WARPING_ZONE) m_position.absPosition = sf::Vector2i(m_position.absPosition.x, -WARPING_ZONE);
		else if (m_position.absPosition.y < -WARPING_ZONE) m_position.absPosition = sf::Vector2i(m_position.absPosition.x, MAZE_PIXELS_HEIGHT + WARPING_ZONE);
	}

	return m_position == oldPosition;
}

void GameObject::ReverseDirection()
{
	m_desiredDirection = getReverseDirection(m_direction);
}

void GameObject::StartMoving()
{
	m_moving = true;
	m_graphicComponent->AnimPlay();
}

void GameObject::StopMoving()
{
	m_moving = false;
	m_graphicComponent->AnimPause();
}

void GameObject::Freeze()
{
	m_frozen = true;
	m_graphicComponent->AnimPause();
}

void GameObject::Unfreeze()
{
	m_frozen = false;
}

sf::Vector2i& GameObject::getSpriteMovementOnTile(MAZE_TYPE maze)
{
	sf::Vector2i movement(0, 0);
	sf::Vector2i directionVector = getDirectionVector(m_direction);
	sf::Vector2i nextTile(m_position.tilePosition.x + directionVector.x, m_position.tilePosition.y + directionVector.y);
	switch (m_direction)
	{
	case dir_Up:
		if (m_shortCutTurn)		
			if (m_position.inTilePosition.x > TILE_HALF_POINT.x) movement.x--;
			else if (m_position.inTilePosition.x < TILE_HALF_POINT.x) movement.x++;
		if (m_position.inTilePosition.y > TILE_HALF_POINT.y || isWarping(maze) || maze[nextTile.y][nextTile.x] >= m_lowestWalkableTile)
			movement.y--;
		break;
	case dir_Left:
		if (m_shortCutTurn)
			if (m_position.inTilePosition.y > TILE_HALF_POINT.y) movement.y--;
			else if (m_position.inTilePosition.y < TILE_HALF_POINT.y) movement.y++;
		if (m_position.inTilePosition.x > TILE_HALF_POINT.x || isWarping(maze) || maze[nextTile.y][nextTile.x] >= m_lowestWalkableTile)
			movement.x--;
		break;
	case dir_Down:
		if (m_shortCutTurn)
			if (m_position.inTilePosition.x > TILE_HALF_POINT.x) movement.x--;
			else if (m_position.inTilePosition.x < TILE_HALF_POINT.x) movement.x++;
		if (m_position.inTilePosition.y < TILE_HALF_POINT.y || isWarping(maze) || maze[nextTile.y][nextTile.x] >= m_lowestWalkableTile)
			movement.y++;
		break;
	case dir_Right:
		if (m_shortCutTurn)
			if (m_position.inTilePosition.y > TILE_HALF_POINT.y) movement.y--;
			else if (m_position.inTilePosition.y < TILE_HALF_POINT.y) movement.y++;
		if (m_position.inTilePosition.x < TILE_HALF_POINT.x || isWarping(maze) || maze[nextTile.y][nextTile.x] >= m_lowestWalkableTile)
			movement.x++;
		break;
	}

	return movement;
}