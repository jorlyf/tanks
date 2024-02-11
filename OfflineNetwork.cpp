#include "OfflineNetwork.h"
#include "World.h";
#include "Player.h"
#include "WindowInput.h"

void OfflineNetwork::processPlayerInput()
{
	const InputState w = WindowInput::getKeyboardKeyState(sf::Keyboard::Key::W);
	const InputState a = WindowInput::getKeyboardKeyState(sf::Keyboard::Key::A);
	const InputState s = WindowInput::getKeyboardKeyState(sf::Keyboard::Key::S);
	const InputState d = WindowInput::getKeyboardKeyState(sf::Keyboard::Key::D);
	const InputState lmb = WindowInput::getMouseButtonState(sf::Mouse::Left);
	const sf::Vector2f worldMousePosition = WindowInput::getWorldMousePosition();

	const Player::PlayerInput playerInput{ w, a, s, d, lmb, worldMousePosition };

	Player* localPlayer = _world->getPlayer(_localPlayerGuid);
	localPlayer->setInput(playerInput);
}

OfflineNetwork::OfflineNetwork(World* world)
{
	_world = world;
}

OfflineNetwork::~OfflineNetwork()
{
	stop();
}

void OfflineNetwork::init(const std::string& ip, const unsigned int port)
{

}

void OfflineNetwork::update()
{
	processPlayerInput();
}

void OfflineNetwork::run()
{
	_world->addPlayer(_localPlayerGuid);
}

void OfflineNetwork::stop()
{
	_world->removePlayer(_localPlayerGuid);
}
