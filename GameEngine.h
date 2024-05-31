#pragma once
#include "Engine.h"
#include "GameNetwork.h"

class World;

class GameEngine : public Engine
{
	std::unique_ptr<World> _world;
	std::unique_ptr<GameNetwork> _network;
	bool _isServer;
	bool _isOnline;
	std::string _socket;

public:
	GameEngine(const bool isServer, bool isOnline, const std::string& socket, const sf::Vector2u& size, const std::string& windowTitle = "Unnamed window");
	virtual ~GameEngine();
	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onRender(std::function<void(sf::Drawable& drawable)> render) override;
	virtual void onEvent(sf::Event& event) override;
};
