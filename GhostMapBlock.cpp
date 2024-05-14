#include "GhostMapBlock.h"
#include "MapBlock.h"

void GhostMapBlock::initSprite()
{
	if (_texture == nullptr)
	{
		_sprite = sf::Sprite();
		return;
	}
	_sprite.setTexture(*_texture);
	sf::FloatRect baseRect = _sprite.getLocalBounds();
	_sprite.setOrigin(baseRect.width / 2, baseRect.height / 2);

	updateSprite();
}

void GhostMapBlock::updateSprite()
{
	const sf::Vector2f worldPosition = MapBlock::getWorldPosition(_position);
	_sprite.setPosition(worldPosition);
}

void GhostMapBlock::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite);
}

GhostMapBlock::GhostMapBlock()
{
	setType(_type);
}

void GhostMapBlock::setPosition(const sf::Vector2i& position)
{
	_position = position;
	updateSprite();
}

void GhostMapBlock::setType(const MapBlockType type)
{
	_type = type;
	_texture = MapBlock::getTexture(_type);
	initSprite();
}

MapBlockState GhostMapBlock::getState() const
{
	MapBlockState state;
	state.position = _position;
	state.type = _type;
	state.health = 0.f;
	return state;
}
