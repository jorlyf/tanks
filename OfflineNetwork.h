#pragma once
#include <memory>
#include <guiddef.h>
#include "GameNetwork.h"

class World;
class Player;

class OfflineNetwork : public GameNetwork
{
	GUID _localPlayerGuid;
	World* _world = nullptr;

	void processPlayerInput();

public:
	OfflineNetwork(World* world);
	~OfflineNetwork();
	void init(const std::string& ip = "127.0.0.1", const unsigned int port = 5025);
	void update();
	void run();
	void stop();
};
