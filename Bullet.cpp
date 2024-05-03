#include "Bullet.h"
#include "TextureManager.h"
#include "DeltaTime.h"
#include "Map.h"
#include "MapBlock.h"
#include "World.h"
#include "Player.h"
#include "Tank.h"
#include "utils.h"

void Bullet::initSprite()
{
	_sprite.setTexture(TextureManager::bullet);
	_sprite.setScale({ 0.02f, 0.02f });
	sf::FloatRect baseRect = _sprite.getLocalBounds();
	_sprite.setOrigin(baseRect.width / 2, baseRect.height / 2);
	const float rotation = directionToAngle(_direction);
	_sprite.setRotation(rotation + 90.f);
	_sprite.setPosition(_position);
}

void Bullet::move()
{
	sf::Vector2f position = _position + _direction * _speed * DeltaTime::getDt();
	setPosition(position);
}

bool Bullet::isCollideMap(const sf::Vector2f& position, const float rotation, MapBlock** mapBlockPtr)
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
				*mapBlockPtr = block;
				break;
			}
		}
	}

	_sprite.setPosition(_position);
	const float _rotation = directionToAngle(_direction);
	_sprite.setRotation(_rotation + 90.f);

	return isCollide;
}

bool Bullet::isCollideTank(const sf::Vector2f& position, const float rotation, Tank** tankPtr)
{
	_sprite.setPosition(position);
	_sprite.setRotation(rotation + 90.f);

	bool isCollide = false;

	Map* map = _world->getMap();
	if (map != nullptr)
	{
		std::list<std::unique_ptr<Player>>* players = _world->getPlayers();
		for (auto it = players->cbegin(); it != players->cend(); it++)
		{
			Player* player = it->get();
			Tank* tank = player->getTank();
			if (tank == nullptr) continue;
			RectCollider blockCollider = tank->getCollider();
			if (_collider.intersects(blockCollider))
			{
				isCollide = true;
				*tankPtr = tank;
				break;
			}
		}
	}

	_sprite.setPosition(_position);
	const float _rotation = directionToAngle(_direction);
	_sprite.setRotation(_rotation + 90.f);

	return isCollide;
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite);
	target.draw(_collider);
}

Bullet::Bullet(const unsigned int id, const GUID& playerGuid, World* world, const sf::Vector2f& position, const sf::Vector2f& direction, const float maxSpeed)
{
	_id = id;
	_playerGuid = playerGuid;
	_world = world;
	_position = position;
	_direction = direction;
	_speed = maxSpeed;

	initSprite();
	_collider = RectCollider(&_sprite);
}

void Bullet::update()
{
	move();
	_sprite.setPosition(_position);
	_speed *= 0.998f;

	_collider.calculateVertices();

	Tank* tankPtr = nullptr;
	MapBlock* mapBlockPtr = nullptr;
	if (isCollideTank(_position, directionToAngle(_direction), &tankPtr))
	{
		_onTankHit.invoke(tankPtr);
	}
	else if (isCollideMap(_position, directionToAngle(_direction), &mapBlockPtr))
	{
		_onMapBlockHit.invoke(mapBlockPtr);
	}
}

unsigned int Bullet::getId() const
{
	return _id;
}

GUID Bullet::getPlayerGuid() const
{
	return _playerGuid;
}

sf::Vector2f Bullet::getPosition() const
{
	return _position;
}

void Bullet::setPosition(const sf::Vector2f& position)
{
	_position = position;
}

sf::Vector2f Bullet::getDirection() const
{
	return _direction;
}

void Bullet::setDirection(const sf::Vector2f& direction)
{
	_direction = direction;
}

float Bullet::getSpeed() const
{
	return _speed;
}

float Bullet::getDamage() const
{
	return 100.f;
}

void Bullet::addOnMapBlockHitListener(const EventHandler<MapBlock*>& handler)
{
	_onMapBlockHit += handler;
}

void Bullet::addOnTankHitListener(const EventHandler<Tank*>& handler)
{
	_onTankHit -= handler;
}
