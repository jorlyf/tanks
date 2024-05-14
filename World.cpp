#include <set>
#include "World.h"
#include "Player.h"
#include "Tank.h"
#include "TankCannon.h"
#include "Bullet.h"
#include "Map.h"
#include "MapBlock.h"
#include "EventHandler.h"

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

		playerState.guid = player->getGuid();
		playerState.team = player->getTeam();
		playerState.cursorPosition = WindowInput::getWorldMousePosition();

		Tank* tank = player->getTank();
		if (tank == nullptr)
		{
			playerState.isTankAlive = false;
			continue;
		}
		playerState.isTankAlive = true;

		TankCannon* tankCannon = tank->getCannon();
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

	setMapName("default");
}

void World::updateSpawnerBlocks()
{
	auto& blocks = *_map->getBlocks();
	for (auto& it : blocks)
	{
		auto block = it.second.get();
		if (block == nullptr) continue;
		if (block->getType() == MapBlockType::redSpawner) _redSpawnerBlock = block;
		else if (block->getType() == MapBlockType::blueSpawner) _blueSpawnerBlock = block;
	}
}

void World::update()
{
	for (auto& respawnCallback : _respawnCallbacks)
	{
		respawnCallback.update();
	}
	_respawnCallbacks.remove_if([](const TimeoutCallback& timeoutCallback) { return timeoutCallback.isInvoked(); });

	for (auto& player : _players)
	{
		player->update();
	}

	for (auto& bullet : _bullets) // TODO: add BulletManager
	{
		bullet->update();

		if (bullet->getSpeed() < 100.f)
		{
			_bulletIdsToDelete.push_back(bullet->getId());
		}
	}

	for (auto& player : _players)
	{
		Tank* tank = player->getTank();
		if (tank == nullptr) continue;
		if (tank->getHp() <= 0)
		{
			player->destroyTank();
			Player* playerPtr = player.get();
			_respawnCallbacks.push_back(TimeoutCallback([=]() {
				if (playerPtr == nullptr) return;
				spawnTank(playerPtr->getGuid());
				}, 5000));
		}
	}

	_map->update();

	for (const unsigned int bulletId : _bulletIdsToDelete)
	{
		deleteBullet(bulletId);
	}
	_bulletIdsToDelete.clear();

	updateWorldState();
}

void World::render(std::function<void(sf::Drawable& drawable)> render) const
{
	render(*_map);

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

		_players.push_back(std::make_unique<Player>(playerState.guid, playerState.team, this));

		Player* player = getPlayer(playerState.guid);
		if (!playerState.isTankAlive) continue;
		spawnTank(player->getGuid());
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

		_bullets.push_back(std::make_unique<Bullet>(bulletState.id, bulletState.playerGuid, this, bulletState.position, bulletState.direction, bulletState.speed));
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

std::list<std::unique_ptr<Player>>* World::getPlayers()
{
	return &_players;
}

void World::addPlayer(const GUID& guid, const Team team)
{
	_players.push_back(std::make_unique<Player>(guid, team, this));
}

void World::removePlayer(const GUID& guid)
{
	auto it = std::find_if(_players.cbegin(), _players.cend(), [guid](const std::unique_ptr<Player>& player) { return player->getGuid() == guid; });
	if (it != _players.cend()) _players.erase(it);
}

void World::spawnTank(const GUID& guid)
{
	Player* player = getPlayer(guid);
	if (player == nullptr) return;
	MapBlock* spawnerBlock = player->getTeam() == Team::red ? _redSpawnerBlock : _blueSpawnerBlock;
	const sf::Vector2f position = spawnerBlock->getWorldPosition();
	const float rotation = 0.f;
	spawnTank(player, position, rotation);
}

void World::spawnTank(const GUID& guid, const sf::Vector2f& position, const float rotation)
{
	Player* player = getPlayer(guid);
	if (player == nullptr) return;
	spawnTank(player, position, rotation);
}

void World::spawnTank(Player* player, const sf::Vector2f& position, const float rotation)
{
	Tank* tank = player->spawnTank(position, rotation);
	tank->getCannon()->addOnFireListener([this](const GUID& playerGuid, const sf::Vector2f& position, const sf::Vector2f& direction) { spawnBullet(playerGuid, position, direction); });

	tank->addOnTankDestroyListener(EventHandler<>([=]() {

		}));
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

void World::spawnBullet(const GUID& playerGuid, const sf::Vector2f& position, const sf::Vector2f& direction)
{
	_bullets.push_back(std::make_unique<Bullet>(++_nextBulletId, playerGuid, this, position, direction, 2500.f));
	Bullet* bullet = getBullet(_nextBulletId);

	EventHandler<Tank*> onTankHit([this, bullet](Tank* tank) {
		const Player* hittedPlayer = tank->getPlayer();
		const Player* attackedPlayer = getPlayer(bullet->getPlayerGuid());
		if (hittedPlayer->getGuid() == bullet->getPlayerGuid()) return;
		if (hittedPlayer->getTeam() == attackedPlayer->getTeam()) return;
		const float damage = bullet->getDamage();
		tank->damage(damage);
		_bulletIdsToDelete.push_back(bullet->getId());
		});
	bullet->addOnTankHitListener(onTankHit);

	EventHandler<MapBlock*> onMapBlockHit([this, bullet](MapBlock* mapBlock) {
		const float damage = bullet->getDamage();
		mapBlock->damage(damage);
		_bulletIdsToDelete.push_back(bullet->getId());
		});
	bullet->addOnMapBlockHitListener(onMapBlockHit);
}

Map* World::getMap() const
{
	if (_map == nullptr) return nullptr;
	return _map.get();
}

void World::setMapName(const std::string& mapName)
{
	if (_map != nullptr) _map.release();
	_map = std::make_unique<Map>(mapName);
	_map->load();
	updateSpawnerBlocks();
}
