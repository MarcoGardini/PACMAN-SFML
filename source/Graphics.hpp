#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <string>

#include "Constants.hpp"
#include "GameObject.hpp"

class GraphicComponent
{
public:
	// Constructors
	GraphicComponent(sf::Texture* const textureIn, const sf::Vector2f& dimensionIn, const AnimationType graphicTypeIn,
		bool isPlaying, int numberOfFrames, std::vector<sf::Vector2i>& framesIn, int frameOffset, int restingFrame = 0);
	GraphicComponent(const std::string& initText, const int fontDimension, const sf::Color color, const sf::Vector2f origin, const sf::Font* font);
	GraphicComponent() = delete;

	// Update animation and position
	void Update(const GameObject* gameObject);

	// Set direction based on the method (rotated or different animation)
	void setDirection(const Direction directionIn);

	// Set current texture rect to match animation frame
	void setCurrentFrameRectangle();

	// Animation managing
	void AnimPlay();
	void AnimStop();
	void AnimPause();
	void AnimReset();
	
public:
	// active (should this be drawn or not)
	bool m_active;

	// sprite/text to be drawn
	sf::Sprite* m_sprite;
	sf::Text* m_text;

	// Texture managing
	sf::Texture* m_texture;
	sf::Vector2f m_textureDimension; // in pixels
	float m_textureRotation; // in degrees

	// Animation configuration
	bool m_animIsPlaying; 
	AnimationType m_animDirectionMethod; // change in direction is static, rotation o different animation?
	sf::Vector2i m_animCurrentFrameRepetition; // x = currentFrame y = currentRepetition
	int m_animNumberOfFrames; 
	std::vector<sf::Vector2i> m_animFrames;
	int m_animFrameOffset; // offset to start on the n-th frame
	int m_animRestingFrame; // frame on pause
};

class GraphicSystem
{
public:
	GraphicSystem();

	void Draw();

public:
	sf::RenderWindow m_window;
	sf::Font* m_font; // global font

	std::array<GraphicComponent*, Assets::MAX_ASSET> m_componentPool;
	std::array<sf::Texture*, Textures::MAX_TEXTURE> m_texturePool; // textures must be kept loaded in memory
};