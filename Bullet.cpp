#include "Bullet.h"
#include "TextureManager.h"
#include "DeltaTime.h"
#include "utils.h"

void Bullet::initSprite()
{
	_sprite.setTexture(TextureManager::bullet);
	_sprite.setScale({ 0.02f, 0.02f });
	sf::FloatRect baseRect = _sprite.getLocalBounds();
	_sprite.setOrigin(baseRect.width / 2, baseRect.height / 2);
	const float rotation = directionToAngle(_direction);
	_sprite.setRotation(rotation);
	_sprite.setPosition(_position);
}

void Bullet::move()
{
	sf::Vector2f position = _position + _direction * _speed * DeltaTime::getDt();
	setPosition(position);
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite);
}

Bullet::Bullet(const unsigned int id, const GUID& playerGuid, const sf::Vector2f& position, const sf::Vector2f& direction, const float maxSpeed)
{
	_id = id;
	_playerGuid = playerGuid;
	_position = position;
	_direction = direction;
	_speed = maxSpeed;

	initSprite();
}

void Bullet::update()
{
	move();
	_sprite.setPosition(_position);
	_speed *= 0.995f;
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
	return 0.f;
}
