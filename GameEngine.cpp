#include "GameEngine.h"
#include "World.h"
#include "Player.h"
#include "Bullet.h"
#include "ServerNetwork.h"
#include "ClientNetwork.h"
#include "OfflineNetwork.h"

GameEngine::GameEngine(const bool isServer, bool isOnline, const std::string& socket, const sf::Vector2u& size, const std::string& windowTitle) : Engine(size, windowTitle)
{
	_isServer = isServer;
	_isOnline = isOnline;
	_socket = socket;
}

GameEngine::~GameEngine() { }

void GameEngine::onStart()
{
	_world = std::make_unique<World>(_window.get());

	if (_isOnline)
	{
		if (_isServer)
		{
			_network = std::make_unique<ServerNetwork>(_world.get());
		}
		else
		{
			_network = std::make_unique<ClientNetwork>(_world.get(), _window.get());
		}
	}
	else
	{
		_network = std::make_unique<OfflineNetwork>(_world.get(), _window.get());
	}

	std::string ip;;
	size_t indexa = _socket.find(':');
	if (indexa == _socket.npos) ip = "0.0.0.0";
	else ip = _socket.substr(0, indexa);

	_network->init(ip);
	_network->run();
}

void GameEngine::onUpdate()
{
	_network->update();
	_world->update();
}

void GameEngine::onRender(std::function<void(sf::Drawable& drawable)> render)
{
	_world->render(render);
}

void GameEngine::onEvent(sf::Event& event)
{

}
