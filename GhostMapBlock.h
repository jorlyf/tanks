#pragma once
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "MapState.h"

class MapBlock;

class GhostMapBlock : public sf::Drawable
{
	sf::Vector2i _position;
	MapBlockType _type = MapBlockType::metal;

	const sf::Texture* _texture = nullptr;
	sf::Sprite _sprite;

	void initSprite();
	void updateSprite();

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	GhostMapBlock();

	void setPosition(const sf::Vector2i& position);
	void setType(const MapBlockType type);
	MapBlockState getState() const;
};
