#include "Tank.h"
#include "TankCannon.h"
#include "utils.h"
#include "DeltaTime.h"
#include "TextureManager.h"

void Tank::initSprite()
{
	_sprite.setTexture(TextureManager::tank);
	_sprite.setScale({ 0.1f, 0.1f });
	sf::FloatRect baseRect = _sprite.getLocalBounds();
	_sprite.setOrigin(baseRect.width / 2, baseRect.height / 2);
	_sprite.setRotation(_rotation + 90.f);
	_sprite.setPosition(_position);
}

void Tank::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite);
	target.draw(*_cannon);

	target.draw(_collider);
}

Tank::Tank(World* world, Player* player)
{
	_world = world;
	_player = player;

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
	setPosition(position);
}

void Tank::moveBackward()
{
	sf::Vector2f direction = -angleToDirection(_rotation);
	const sf::Vector2f position = _position + direction * _speed * DeltaTime::getDt();
	setPosition(position);
}

void Tank::rotateLeft()
{
	const float rotation = _rotation - _rotationSpeed * DeltaTime::getDt();
	setRotation(rotation);
}

void Tank::rotateRight()
{
	const float rotation = _rotation + _rotationSpeed * DeltaTime::getDt();
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
