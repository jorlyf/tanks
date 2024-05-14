#include <iostream>
#include "Player.h"
#include "Tank.h"
#include "TankCannon.h"
#include "utils.h"
#include <combaseapi.h>
#include "GUIDUtils.h"
#include "FontManager.h"

void Player::processMovingInput()
{
	if (_tank == nullptr) return;
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
	if (_tank == nullptr) return;
	if (_input.lmb.isHolding)
	{
		_tank->getCannon()->fire();
	}
}

void Player::setCannonTargetRotation()
{
	if (_tank == nullptr) return;
	const sf::Vector2f tankPosition = _tank->getPosition();
	const sf::Vector2f targetDirection{ _input.worldMousePosition.x - tankPosition.x, _input.worldMousePosition.y - tankPosition.y };
	const float targetRotation = directionToAngle(targetDirection);
	_tank->getCannon()->setTargetRotation(targetRotation);
}

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (_tank != nullptr) target.draw(*_tank);
	if (_tank != nullptr)
	{
		const std::string guidString = GUIDUtils::guidToString(_guid);
		const sf::Font& font = FontManager::regular;
		sf::Text guidText(sf::String(guidString), font, 9);
		const sf::Color color = _team == Team::red ? sf::Color::Red : sf::Color::Blue;
		guidText.setFillColor(color);
		guidText.setPosition(_tank->getPosition());
		target.draw(guidText);
	}
}

Player::Player(const GUID& guid, const Team team, World* world)
{
	_guid = guid;
	_team = team;
	_world = world;
}

void Player::update()
{
	processMovingInput();
	processFiringInput();
	setCannonTargetRotation();
	if (_tank != nullptr) _tank->update();
}

GUID Player::getGuid() const
{
	return _guid;
}

Team Player::getTeam() const
{
	return _team;
}

Tank* Player::getTank()
{
	if (_tank == nullptr) return nullptr;
	return _tank.get();
}

Tank* Player::spawnTank(const sf::Vector2f& position, const float rotation)
{
	_tank = std::make_unique<Tank>(_world, this, position, rotation);
	return _tank.get();
}

void Player::destroyTank()
{
	if (_tank == nullptr) return;
	_tank.release();
	_tank = nullptr;
}

void Player::setInput(const PlayerInput& playerInput)
{
	_input = playerInput;
}
