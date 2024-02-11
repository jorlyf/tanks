#pragma once
#include <memory>
#include <functional>
#include <list>
#include <guiddef.h>
#include <SFML/Graphics.hpp>
#include "Window.h"
#include "WorldState.h"

class Bullet;
class Player;

class World
{
	Window* _window = nullptr;
	std::list<std::unique_ptr<Player>> _players;
	std::list<std::unique_ptr<Bullet>> _bullets;
	WorldState _worldState;

	unsigned int _nextBulletId = 0;

	void updateWorldState();

public:
	World(Window* window);

	void update();
	void render(std::function<void(sf::Drawable& drawable)> render) const;
	void synchronizeWorldState(const WorldState& worldState);
	WorldState getWorldState() const;
	Player* getPlayer(const GUID& guid) const;
	void addPlayer(const GUID& guid);
	void removePlayer(const GUID& guid);
	Bullet* getBullet(const unsigned int id) const;
	void deleteBullet(const unsigned int id);
	void spawnPlayer(const GUID& guid);
	void spawnBullet(const GUID& playerGuid, const sf::Vector2f& position, const sf::Vector2f& direction);
};
