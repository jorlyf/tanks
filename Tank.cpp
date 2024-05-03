#include "Tank.h"
#include "TankCannon.h"
#include "utils.h"
#include "DeltaTime.h"
#include "TextureManager.h"
#include "World.h"
#include "MapBlock.h"

void Tank::initSprite()
{
	_sprite.setTexture(TextureManager::tank);
	_sprite.setScale({ 0.1f, 0.1f });
	sf::FloatRect baseRect = _sprite.getLocalBounds();
	_sprite.setOrigin(baseRect.width / 2, baseRect.height / 2);
	_sprite.setRotation(_rotation + 90.f);
	_sprite.setPosition(_position);
}

bool Tank::isCollideMap(const sf::Vector2f& position, const float rotation)
{
	_sprite.setPosition(position);
	_sprite.setRotation(rotation + 90.f);

	bool isCollide = false;

	Map* map = _world->getMap();
	if (map != nullptr)
	{
		std::map<sf::Vector2i, std::unique_ptr<MapBlock>, Vector2iComparator>* blocks = map->getBlocks();
		for (auto it = blocks->cbegin(); it != blocks->cend(); it++)
		{
			MapBlock* block = it->second.get();
			if (block == nullptr) continue;
			RectCollider blockCollider = block->getCollider();
			if (_collider.intersects(blockCollider))
			{
				isCollide = true;
				break;
			}
		}
	}

	_sprite.setPosition(_position);
	_sprite.setRotation(_rotation + 90.f);

	return isCollide;
}

void Tank::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite);
	target.draw(*_cannon);

	target.draw(_collider);
}

Tank::Tank(World* world, Player* player, const sf::Vector2f position, const float rotation)
{
	_world = world;
	_player = player;
	_position = position;
	_rotation = normalizeAngle(rotation);

	_cannon = std::make_unique<TankCannon>(this, 0.f);

	initSprite();

	_collider = RectCollider(&_sprite);
}

void Tank::update()
{
	_cannon->update();

	_sprite.setPosition(_position);
	_sprite.setRotation(_rotation + 90.f);

	_collider.calculateVertices();
}

void Tank::moveForward()
{
	sf::Vector2f direction = angleToDirection(_rotation);
	float dt = DeltaTime::getDt();
	const sf::Vector2f position = _position + direction * _speed * DeltaTime::getDt();
	if (isCollideMap(position, _rotation)) return;
	setPosition(position);
}

void Tank::moveBackward()
{
	sf::Vector2f direction = -angleToDirection(_rotation);
	const sf::Vector2f position = _position + direction * _speed * DeltaTime::getDt();
	if (isCollideMap(position, _rotation)) return;
	setPosition(position);
}

void Tank::rotateLeft()
{
	const float rotation = _rotation - _rotationSpeed * DeltaTime::getDt();
	if (isCollideMap(_position, rotation)) return;
	setRotation(rotation);
}

void Tank::rotateRight()
{
	const float rotation = _rotation + _rotationSpeed * DeltaTime::getDt();
	if (isCollideMap(_position, rotation)) return;
	setRotation(rotation);
}

float Tank::getRotation() const
{
	return _rotation;
}

void Tank::setRotation(const float rotation)
{
	_rotation = normalizeAngle(rotation);
}

sf::Vector2f Tank::getPosition() const
{
	return _position;
}

void Tank::setPosition(const sf::Vector2f& position)
{
	_position = position;
}

float Tank::getHp() const
{
	return _hp;
}

void Tank::setHp(const float hp)
{
	_hp = hp;
}

Player* Tank::getPlayer() const
{
	return _player;
}

TankCannon* Tank::getCannon() const
{
	return _cannon.get();
}

float Tank::damage(const float damage)
{
	setHp(_hp - damage);
	return getHp();
}

RectCollider Tank::getCollider() const
{
	return _collider;
}
