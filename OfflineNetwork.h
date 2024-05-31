#pragma once
#include <memory>
#include <guiddef.h>
#include "GameNetwork.h"
#include "Camera.h"

class World;
class Window;
class Player;

class OfflineNetwork : public GameNetwork
{
	GUID _localPlayerGuid{};
	World* _world = nullptr;
	Window* _window = nullptr;
	Camera _camera;

	void processPlayerInput();

public:
	OfflineNetwork(World* world, Window* window);
	~OfflineNetwork();
	void init(const std::string& ip = "0.0.0.0", const unsigned int port = 5025);
	void update();
	void run();
	void stop();
};
