#pragma once
#include <memory>
#include <map>
#include <mutex>
#include "GameNetwork.h"
#include "TcpClient.h"
#include "Player.h"
#include "WorldState.h"
#include "GamePacketTypes.h"
#include "Camera.h"

class World;

class ClientNetwork : public GameNetwork
{
	std::unique_ptr<TcpClient> _socket = nullptr;
	std::thread _socketThread;

	World* _world = nullptr;
	Window* _window = nullptr;
	Camera _camera;

	std::map<GamePacketTypes, std::function<void(const void* data)>> _packetHandlers;

	void processPacket(const Packet& packet);
	void initPacketHandlers();
	void sendPlayerInput();

public:
	ClientNetwork(World* world, Window* window);
	~ClientNetwork();
	void init(const std::string& ip = "127.0.0.1", const unsigned int port = 5025);
	void update();
	void run();
	void stop();
};
