#pragma once
#include <memory>
#include <functional>
#include <list>
#include <guiddef.h>
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "WorldState.h"
#include "Map.h"
#include "Team.h"
#include "TimeoutCallback.h"

class Bullet;
class Player;
class Map;

class World
{
	Window* _window = nullptr;
	std::list<std::unique_ptr<Player>> _players;
	std::list<std::unique_ptr<Bullet>> _bullets;
	WorldState _worldState;

	std::list<TimeoutCallback> _respawnCallbacks;
	MapBlock* _redSpawnerBlock = nullptr;
	MapBlock* _blueSpawnerBlock = nullptr;

	std::unique_ptr<Map> _map = nullptr;

	unsigned int _nextBulletId = 0;

	std::list<unsigned int> _bulletIdsToDelete;

	void updateWorldState();
	void updateSpawnerBlocks();

public:
	World(Window* window);

	void update();
	void render(std::function<void(sf::Drawable& drawable)> render) const;
	void synchronizeWorldState(const WorldState& worldState);
	WorldState getWorldState() const;
	Player* getPlayer(const GUID& guid) const;
	std::list<std::unique_ptr<Player>>* getPlayers();
	void addPlayer(const GUID& guid, const Team team);
	void removePlayer(const GUID& guid);
	void spawnTank(const GUID& guid);
	void spawnTank(const GUID& guid, const sf::Vector2f& position, const float rotation);
	void spawnTank(Player* player, const sf::Vector2f& position, const float rotation);
	Bullet* getBullet(const unsigned int id) const;
	void deleteBullet(const unsigned int id);
	void spawnBullet(const GUID& playerGuid, const sf::Vector2f& position, const sf::Vector2f& direction);
	Map* getMap() const;
	void setMapName(const std::string& mapName);
};
