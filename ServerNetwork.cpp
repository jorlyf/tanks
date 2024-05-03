#include "ServerNetwork.h"
#include "World.h"
#include "Player.h"

ServerNetwork::ServerClient::ServerClient(const GUID& guid)
{
	_guid = guid;
}

GUID ServerNetwork::ServerClient::getGuid() const
{
	return _guid;
}

void ServerNetwork::onSocketOpened(const GUID& guid)
{
	_openedSocketsMutex.lock();
	_openedSockets.push_back(guid);
	_openedSocketsMutex.unlock();
}

void ServerNetwork::onSocketClosed(const GUID& guid)
{
	_closedSocketsMutex.lock();
	_closedSockets.push_back(guid);
	_closedSocketsMutex.unlock();
}

void ServerNetwork::processOpenedClosedSockets()
{
	_openedSocketsMutex.lock();
	for (GUID playerGuid : _openedSockets)
	{
		_clients.push_back(std::make_unique<ServerClient>(playerGuid));
		std::cout << "Player connected." << std::endl;
		_world->addPlayer(playerGuid);
		_world->spawnTank(playerGuid);
	}
	_openedSockets.clear();
	_openedSocketsMutex.unlock();

	_closedSocketsMutex.lock();
	for (GUID playerGuid : _closedSockets)
	{
		auto it = std::find_if(_clients.cbegin(), _clients.cend(), [playerGuid](const std::unique_ptr<ServerClient>& client) { return client->getGuid() == playerGuid; });
		if (it != _clients.cend())
		{
			std::cout << "Player disconnected." << std::endl;
			_world->removePlayer(playerGuid);
		}
	}
	_closedSockets.clear();
	_closedSocketsMutex.unlock();
}

void ServerNetwork::processPacket(const ServerPacketIn& packet)
{
	const GUID playerGuid = packet.senderGuid;
	const GamePacketTypes packetType = (GamePacketTypes)packet.type;
	_packetHandlers[packetType](playerGuid, packet.data);
}

void ServerNetwork::sendWorldState()
{
	if (_clients.size() == 0) return;

	std::set<GUID> playerGuids;
	for (auto& client : _clients)
	{
		playerGuids.insert(client->getGuid());
	}

	ServerPacketOut packet;
	packet.type = GamePacketTypes::worldState;
	memcpy(packet.data, &_worldState, sizeof(WorldState));
	packet.recipientsGuids = playerGuids;

	_socket->sendPacket(packet);
}

void ServerNetwork::initPacketHandlers()
{
	_packetHandlers[GamePacketTypes::playerInput] = [this](const GUID& playerGuid, const void* data)
		{
			Player::PlayerInput& playerInput = *(Player::PlayerInput*)data;
			Player* player = _world->getPlayer(playerGuid);
			if (player == nullptr) return;
			player->setInput(playerInput);
		};
}

ServerNetwork::ServerNetwork(World* world)
{
	_world = world;
}

ServerNetwork::~ServerNetwork()
{
	stop();
}

void ServerNetwork::init(const std::string& ip, const unsigned int port)
{
	initPacketHandlers();
	_socket = std::make_unique<TcpListener>(ip, port);
	_socket->setSocketOpenHandler([this](const GUID& guid) { onSocketOpened(guid); });
	_socket->setSocketCloseHandler([this](const GUID& guid) { onSocketClosed(guid); });
}

void ServerNetwork::update()
{
	processOpenedClosedSockets();

	ServerPacketIn packet;
	while (_socket->getPacket(&packet))
	{
		processPacket(packet);
	}

	_worldState = _world->getWorldState();
	sendWorldState();
	std::chrono::milliseconds t(10);
	std::this_thread::sleep_for(t);
}

void ServerNetwork::run()
{
	_socketThread = std::thread(&TcpListener::run, _socket.get());
}

void ServerNetwork::stop()
{
	_socket->stop();
	if (_socketThread.joinable()) _socketThread.join();
}
