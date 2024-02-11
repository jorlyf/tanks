#include "Player.h"
#include "Tank.h"
#include "TankCannon.h"
#include "utils.h"
#include <iostream>

void Player::processMovingInput()
{
	if (_input.w.isHolding)
	{
		_tank->moveForward();
	}
	if (_input.a.isHolding)
	{
		_tank->rotateLeft();
	}
	if (_input.s.isHolding)
	{
		_tank->moveBackward();
	}
	if (_input.d.isHolding)
	{
		_tank->rotateRight();
	}
}

void Player::processFiringInput()
{
	if (_input.lmb.isHolding)
	{
		_tank->getCannon()->fire();
	}
}

void Player::setCannonTargetRotation()
{
	const sf::Vector2f tankPosition = _tank->getPosition();
	const sf::Vector2f targetDirection{ _input.worldMousePosition.x - tankPosition.x, _input.worldMousePosition.y - tankPosition.y };
	const float targetRotation = directionToAngle(targetDirection);
	_tank->getCannon()->setTargetRotation(targetRotation);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*_tank);
}

Player::Player(const GUID& guid, World* world)
{
	_guid = guid;
	_world = world;

	_tank = std::make_unique<Tank>(_world, this);
}

void Player::update()
{
	processMovingInput();
	processFiringInput();
	setCannonTargetRotation();
	_tank->update();
}

GUID Player::getGuid() const
{
	return _guid;
}

Tank* Player::getTank()
{
	return _tank.get();
}

void Player::setInput(const PlayerInput& playerInput)
{
	_input = playerInput;
}
