#include "GameEngine.h"
#include "World.h"
#include "Player.h"
#include "Bullet.h"
#include "ServerNetwork.h"
#include "ClientNetwork.h"
#include "OfflineNetwork.h"
#include "TextureManager.h"

GameEngine::GameEngine(const bool isServer, bool isOnline, const sf::Vector2u& size, const std::string& windowTitle) : Engine(size, windowTitle)
{
	_isServer = isServer;
	_isOnline = isOnline;

	TextureManager::load();
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
			_network = std::make_unique<ClientNetwork>(_world.get());
		}
	}
	else
	{
		_network = std::make_unique<OfflineNetwork>(_world.get());
	}

	_network->init();
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
	if (event.type == sf::Event::KeyPressed) WindowInput::handleKeyboardKeyPressed(event.key.code);
	else if (event.type == sf::Event::KeyReleased) WindowInput::handleKeyboardKeyReleased(event.key.code);
	else if (event.type == sf::Event::MouseButtonPressed) WindowInput::handleMouseButtonPressed(event.mouseButton.button);
	else if (event.type == sf::Event::MouseButtonReleased) WindowInput::handleMouseButtonReleased(event.mouseButton.button);
}
