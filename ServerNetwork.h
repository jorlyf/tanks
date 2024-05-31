#pragma once
#include <memory>
#include <list>
#include <thread>
#include <mutex>
#include "GameNetwork.h"
#include "TcpListener.h"
#include "WorldState.h"
#include "GamePacketTypes.h"

class World;

class ServerNetwork : public GameNetwork
{
	class ServerClient
	{
		GUID _guid;

	public:
		ServerClient(const GUID& guid);
		GUID getGuid() const;
	};

	std::unique_ptr<TcpListener> _socket = nullptr;
	std::thread _socketThread;
	std::list<std::unique_ptr<ServerClient>> _clients;
	std::list<GUID> _openedSockets;
	std::list<GUID> _closedSockets;
	std::mutex _openedSocketsMutex;
	std::mutex _closedSocketsMutex;

	World* _world = nullptr;
	WorldState _worldState;
	std::mutex _worldStateMutex;

	std::map<GamePacketTypes, std::function<void(const GUID& playerGuid, const void* data)>> _packetHandlers;

	void onSocketOpened(const GUID& playerGuid);
	void onSocketClosed(const GUID& playerGuid);
	void processOpenedClosedSockets();
	void processPacket(const ServerPacketIn& packet);
	void sendWorldState();
	void initPacketHandlers();

public:
	ServerNetwork(World* world);
	~ServerNetwork();
	void init(const std::string& ip = "0.0.0.0", const unsigned int port = 5025);
	void update();
	void run();
	void stop();
};
