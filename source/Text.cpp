#include "Text.hpp"

#include "PacmanGame.hpp";
#include "Graphics.hpp";

StandardTextObject::StandardTextObject(PacmanGame* pacmanGame, Assets asset, const sf::Vector2i& positionIn) :
	GameObject(pacmanGame->m_graphicSystem->m_componentPool[asset], true, false, positionIn)
{
	m_graphicComponent->m_active = true;
}

void StandardTextObject::Update(PacmanGame* pacmanGame)
{
	if (m_active && m_timerTarget != 0 && m_internalTimer.getElapsedMilliseconds() > m_timerTarget)
	{
		m_active = false;
		m_timerTarget = 0;
	}
	GameObject::Update(pacmanGame);
}

void StandardTextObject::setText(const std::string& text)
{
	m_graphicComponent->m_text->setString(text);
}

void StandardTextObject::ActivateWithTimeout(int timeout)
{
	m_active = true;
	m_timerTarget = timeout;
	m_internalTimer.Reset();
}

AutoUpdateTextObject::AutoUpdateTextObject(PacmanGame* pacmanGame, Assets asset, const sf::Vector2i& positionIn, int* value, std::string headerIn) :
	StandardTextObject(pacmanGame, asset, positionIn), m_valueToPrint(value), m_header(headerIn)
{
}

void AutoUpdateTextObject::Update(PacmanGame* pacmanGame)
{
	setText(m_header + std::to_string(*m_valueToPrint));
	StandardTextObject::Update(pacmanGame);
}

ScoreTextObject::ScoreTextObject(PacmanGame* pacmanGame, Assets asset, const sf::Vector2i& positionIn, int* value) :
	StandardTextObject(pacmanGame, asset, positionIn), m_valueToPrint(value)
{
}

void ScoreTextObject::Update(PacmanGame* pacmanGame)
{
	std::stringstream oss;
	oss << std::setw(8) << std::setfill('0') << *m_valueToPrint;
	setText(oss.str());

	StandardTextObject::Update(pacmanGame);
}
