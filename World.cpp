#include "World.h"
#include "Player.h"
#include "Tank.h"
#include "TankCannon.h"
#include "Bullet.h"
#include <set>

void World::updateWorldState()
{
	WorldState worldState;
	worldState.playerCount = (unsigned char)_players.size();
	unsigned char playerIndex = 0;
	for (auto& player : _players)
	{
		PlayerState& playerState = worldState.players[playerIndex++];
		TankState& tankState = playerState.tank;
		TankCannonState& tankCannonState = tankState.cannon;

		Tank* tank = player->getTank();
		TankCannon* tankCannon = tank->getCannon();

		playerState.guid = player->getGuid();
		playerState.cursorPosition = WindowInput::getWorldMousePosition();
		tankState.hp = tank->getHp();
		tankState.position = tank->getPosition();
		tankState.rotation = tank->getRotation();
		tankCannonState.rotation = tankCannon->getRotation();
		tankCannonState.targetRotation = tankCannon->getTargetRotation();
	}
	worldState.bulletCount = (unsigned char)_bullets.size();
	unsigned char bulletIndex = 0;
	for (auto& bullet : _bullets)
	{
		BulletState& bulletState = worldState.bullets[bulletIndex++];
		bulletState.id = bullet->getId();
		bulletState.playerGuid = bullet->getPlayerGuid();
		bulletState.position = bullet->getPosition();
		bulletState.direction = bullet->getDirection();
		bulletState.speed = bullet->getSpeed();
	}
	_worldState = worldState;
}

World::World(Window* window)
{
	_window = window;
	updateWorldState();
}

void World::update()
{
	for (auto& player : _players)
	{
		player->update();
	}

	std::set<unsigned int> bulletIdsToDelete;
	for (auto& bullet : _bullets)
	{
		bullet->update();
		if (bullet->getSpeed() < 200.f)
		{
			bulletIdsToDelete.insert(bullet->getId());
		}
	}

	for (auto bulletId : bulletIdsToDelete)
	{
		deleteBullet(bulletId);
	}

	updateWorldState();
}

void World::render(std::function<void(sf::Drawable& drawable)> render) const
{
	for (auto& player : _players)
	{
		render(*player);
	}

	for (auto& bullet : _bullets)
	{
		render(*bullet);
	}
}

void World::synchronizeWorldState(const WorldState& worldState)
{
	_worldState = worldState;

	_players.clear();
	_bullets.clear();

	for (unsigned char i = 0; i < _worldState.playerCount; i++)
	{
		PlayerState& playerState = _worldState.players[i];
		TankState& tankState = playerState.tank;
		TankCannonState& tankCannonState = tankState.cannon;

		_players.push_back(std::make_unique<Player>(playerState.guid, this));

		Player* player = getPlayer(playerState.guid);
		Tank* tank = player->getTank();
		TankCannon* tankCannon = tank->getCannon();

		tank->setHp(tankState.hp);
		tank->setPosition(tankState.position);
		tank->setRotation(tankState.rotation);
		tankCannon->setRotation(tankCannonState.rotation);
		tankCannon->setTargetRotation(tankCannonState.targetRotation);
	}

	for (unsigned char i = 0; i < _worldState.bulletCount; i++)
	{
		BulletState& bulletState = _worldState.bullets[i];

		_bullets.push_back(std::make_unique<Bullet>(bulletState.id, bulletState.playerGuid, bulletState.position, bulletState.direction, bulletState.speed));
	}
}

WorldState World::getWorldState() const
{
	return _worldState;
}

Player* World::getPlayer(const GUID& guid) const
{
	for (auto& player : _players)
	{
		if (player->getGuid() == guid) return player.get();
	}
	return nullptr;
}

void World::addPlayer(const GUID& guid)
{
	_players.push_back(std::make_unique<Player>(guid, this));

	Player* player = getPlayer(guid);
	player->getTank()->getCannon()->addOnFireListener([this](const GUID& playerGuid, const sf::Vector2f& position, const sf::Vector2f& direction) { spawnBullet(playerGuid, position, direction); });
}

void World::removePlayer(const GUID& guid)
{
	auto it = std::find_if(_players.cbegin(), _players.cend(), [guid](const std::unique_ptr<Player>& player) { return player->getGuid() == guid; });
	if (it != _players.cend()) _players.erase(it);
}

Bullet* World::getBullet(const unsigned int id) const
{
	for (auto& bullet : _bullets)
	{
		if (bullet->getId() == id) return bullet.get();
	}
	return nullptr;
}

void World::deleteBullet(const unsigned int id)
{
	auto it = std::find_if(_bullets.cbegin(), _bullets.cend(), [id](const std::unique_ptr<Bullet>& bullet) { return bullet->getId() == id; });
	if (it != _bullets.cend()) _bullets.erase(it);
}

void World::spawnPlayer(const GUID& guid)
{
}

void World::spawnBullet(const GUID& playerGuid, const sf::Vector2f& position, const sf::Vector2f& direction)
{
	_bullets.push_back(std::make_unique<Bullet>(++_nextBulletId, playerGuid, position, direction, 2500.f));
}
