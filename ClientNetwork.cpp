#include "ClientNetwork.h"
#include "World.h"

void ClientNetwork::processPacket(const Packet& packet)
{
	const GamePacketTypes packetType = (GamePacketTypes)packet.type;
	_packetHandlers[packetType](packet.data);
}

void ClientNetwork::initPacketHandlers()
{
	_packetHandlers[GamePacketTypes::worldState] = [this](const void* data)
		{
			const WorldState& worldState = *(WorldState*)data;
			_world->synchronizeWorldState(worldState);
		};
}

void ClientNetwork::sendPlayerInput()
{
	const InputState w = WindowInput::getKeyboardKeyState(sf::Keyboard::Key::W);
	const InputState a = WindowInput::getKeyboardKeyState(sf::Keyboard::Key::A);
	const InputState s = WindowInput::getKeyboardKeyState(sf::Keyboard::Key::S);
	const InputState d = WindowInput::getKeyboardKeyState(sf::Keyboard::Key::D);
	const InputState lmb = WindowInput::getMouseButtonState(sf::Mouse::Left);
	const sf::Vector2f worldMousePosition = WindowInput::getWorldMousePosition();

	const Player::PlayerInput playerInput{ w, a, s, d, lmb, worldMousePosition };

	Packet packet;
	packet.type = GamePacketTypes::playerInput;
	memcpy(packet.data, &playerInput, sizeof(Player::PlayerInput));
	_socket->sendPacket(packet);
}

ClientNetwork::ClientNetwork(World* world)
{
	_world = world;
}

ClientNetwork::~ClientNetwork()
{
	stop();
}

void ClientNetwork::init(const std::string& ip, const unsigned int port)
{
	initPacketHandlers();
	_socket = std::make_unique<TcpClient>(ip, port);
}

void ClientNetwork::update()
{
	if (!_socket->isRegistrated()) return;

	Packet packet;
	while (_socket->getPacket(&packet))
	{
		processPacket(packet);
	}
	sendPlayerInput();
}

void ClientNetwork::run()
{
	_socketThread = std::thread(&TcpClient::run, _socket.get());
}

void ClientNetwork::stop()
{
	_socket->stop();
	if (_socketThread.joinable()) _socketThread.join();
}
