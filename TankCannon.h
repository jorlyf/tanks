#pragma once
#include <functional>
#include <list>
#include <guiddef.h>
#include <SFML/Graphics.hpp>

class Tank;

class TankCannon : public sf::Drawable
{
	Tank* _tank = nullptr;

	float _rotationSpeed = 200.f;
	float _fireRatePerMinute = 60.f;
	float _rotation;
	float _targetRotation;

	sf::Sprite _sprite;

	std::list<std::function<void(const GUID& playerGuid, const sf::Vector2f& fromPosition, const sf::Vector2f& direction)>> _onFireListeners;

	std::clock_t _lastFireTimeMilliSeconds;

	void rotate();

	void initSprite();

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	TankCannon(Tank* tank, const float rotation);
	void update();
	void fire();
	float getRotation() const;
	void setRotation(const float rotation);
	float getTargetRotation() const;
	void setTargetRotation(const float targetRotation);
	float getFireRatePerMinute() const;
	float getRotationSpeed() const;
	Tank* getTank() const;
	void addOnFireListener(std::function<void(const GUID& playerGuid, const sf::Vector2f& fromPosition, const sf::Vector2f& direction)> listener);
};
