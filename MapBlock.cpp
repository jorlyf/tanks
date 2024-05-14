#include "MapBlock.h"
#include "TextureManager.h"

int MapBlock::SIZE = 64;

void MapBlock::notifyMapBlockDestroyListeners()
{
	for (auto& listener : _onBlockDestroy)
	{
		listener(this);
	}
}

void MapBlock::initSprite()
{
	if (_texture == nullptr)
	{
		_sprite = sf::Sprite();
		return;
	}
	_sprite.setTexture(*_texture);
	sf::FloatRect baseRect = _sprite.getLocalBounds();
	_sprite.setOrigin(baseRect.width / 2, baseRect.height / 2);
	sf::Vector2f worldPosition = getWorldPosition();
	_sprite.setPosition(worldPosition);

	_collider = RectCollider(&_sprite);
}

sf::Vector2f MapBlock::getWorldPosition() const
{
	return getWorldPosition(_position);
}

bool MapBlock::isUnbreakable() const
{
	switch (_type)
	{
	case MapBlockType::metal:
	case MapBlockType::redSpawner:
	case MapBlockType::blueSpawner:
		return true;

	default: return false;
	}
}

void MapBlock::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite);

	target.draw(_collider);
}

MapBlock::MapBlock(const MapBlockState& state)
{
	_type = state.type;
	_position = state.position;
	_health = state.health;

	_texture = getTexture(_type);
	initSprite();
}

void MapBlock::update()
{
}

MapBlockType MapBlock::getType() const
{
	return _type;
}

sf::Vector2i MapBlock::getPosition() const
{
	return _position;
}

float MapBlock::getHealth() const
{
	return _health;
}

RectCollider MapBlock::getCollider() const
{
	return _collider;
}

sf::Vector2f MapBlock::getWorldPosition(const sf::Vector2i position)
{
	const float x = position.x * SIZE;
	const float y = position.y * SIZE;
	return sf::Vector2f(x, y);
}

void MapBlock::damage(const float damage)
{
	_health -= damage;
	if (_health <= 0) notifyMapBlockDestroyListeners();
}

void MapBlock::addOnMapBlockDestroyListener(std::function<void(MapBlock* mapBlock)> listener)
{
	_onBlockDestroy.push_back(listener);
}

sf::Texture* MapBlock::getTexture(const MapBlockType type)
{
	switch (type)
	{
	case MapBlockType::metal:
		return &TextureManager::metalBlock;
	case MapBlockType::redBase:
		return  &TextureManager::redBaseBlock;
	case MapBlockType::blueBase:
		return  &TextureManager::blueBaseBlock;
	case MapBlockType::redSpawner:
		return nullptr;
	case MapBlockType::blueSpawner:
		return nullptr;
	default:
		return nullptr;
	}
}
