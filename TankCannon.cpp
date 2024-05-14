#include "TankCannon.h"
#include "Tank.h"
#include "Player.h"
#include "utils.h"
#include "DeltaTime.h"
#include "TextureManager.h"

void TankCannon::rotate()
{
	const float diff = std::fmodf(_targetRotation - _rotation, 360.f);
	float shortestDistance = 180.f - std::fabsf(std::fabsf(diff) - 180.f);
	shortestDistance = std::fmodf(diff + 360.f, 360.f) < 180.f ? shortestDistance *= 1 : shortestDistance *= -1;

	if (std::abs(shortestDistance) < 1e-4f) return;

	float rot = _rotation;
	if (shortestDistance > 0) rot += _rotationSpeed * DeltaTime::getDt();
	else if (shortestDistance < 0) rot -= _rotationSpeed * DeltaTime::getDt();


	setRotation(rot);
}

void TankCannon::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_sprite);
}

void TankCannon::initSprite()
{
	_sprite.setTexture(TextureManager::tankCannon);
	_sprite.setScale({ 0.05f, 0.05f });
	sf::FloatRect baseRect = _sprite.getLocalBounds();
	_sprite.setOrigin(baseRect.width / 2, baseRect.height / 2);
	_sprite.setRotation(_rotation + 90.f);
	_sprite.setPosition(_tank->getPosition());
}

TankCannon::TankCannon(Tank* tank, const float rotation)
{
	_tank = tank;
	_rotation = rotation;
	_targetRotation = _rotation;
	_lastFireTimeMilliSeconds = 0L;

	initSprite();
}

void TankCannon::update()
{
	rotate();

	_sprite.setPosition(_tank->getPosition());
	_sprite.setRotation(_rotation + 90.f);
}

void TankCannon::fire()
{
	const float fireDelayMilliSeconds = _fireRatePerMinute / 60.f * 1000.f;
	const clock_t now = std::clock();
	if (now < _lastFireTimeMilliSeconds + fireDelayMilliSeconds) return;
	_lastFireTimeMilliSeconds = now;

	const GUID playerGuid = _tank->getPlayer()->getGuid();
	const sf::Vector2f fromPosition = _tank->getPosition();
	const sf::Vector2f direction = angleToDirection(_rotation);

	for (auto& onFire : _onFireListeners)
	{
		onFire(playerGuid, fromPosition, direction);
	}
}

float TankCannon::getRotation() const
{
	return _rotation;
}

void TankCannon::setRotation(const float rotation)
{
	_rotation = normalizeAngle(rotation);
}

float TankCannon::getTargetRotation() const
{
	return _targetRotation;
}

void TankCannon::setTargetRotation(const float targetRotation)
{
	_targetRotation = targetRotation;
}

float TankCannon::getFireRatePerMinute() const
{
	return _fireRatePerMinute;
}

float TankCannon::getRotationSpeed() const
{
	return _rotationSpeed;
}

Tank* TankCannon::getTank() const
{
	return _tank;
}

void TankCannon::addOnFireListener(std::function<void(const GUID& playerGuid, const sf::Vector2f& position, const sf::Vector2f& direction)> listener)
{
	_onFireListeners.push_back(listener);
}
