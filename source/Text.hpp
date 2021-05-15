#pragma once

#include "GameObject.hpp"
#include "Constants.hpp"

#include <string>

class PacmanGame;

// Standard Text, can be active or activated with a timeout
class StandardTextObject : public GameObject
{
public:
	StandardTextObject(PacmanGame* pacmanGame, Assets asset, const sf::Vector2i& positionIn);

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override {};

	virtual void Update(PacmanGame* pacmanGame) override;

	void setText(const std::string& text);

	void ActivateWithTimeout(int timeout);
};

// Text with a Header and a "followed" value, example: LIVES: *PacmanGame->m_pacman->m_lives;
class AutoUpdateTextObject : public StandardTextObject
{
public:
	AutoUpdateTextObject(PacmanGame* pacmanGame, Assets asset, const sf::Vector2i& positionIn, int* value, std::string headerIn = "");

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override {};

	virtual void Update(PacmanGame* pacmanGame) override;

public:
	int* m_valueToPrint;
	std::string m_header;
};

// Special "Score" Text, fills with 0 
class ScoreTextObject : public StandardTextObject
{
public:
	ScoreTextObject(PacmanGame* pacmanGame, Assets asset, const sf::Vector2i& positionIn, int* value);

	virtual void ManageEvent(PacmanGame* pacmanGame, GameEvent event) override {};

	virtual void Update(PacmanGame* pacmanGame) override;

public:
	int* m_valueToPrint;
};