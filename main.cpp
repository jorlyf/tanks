#include <iostream>
#include <thread>
#include "TcpListener.h"
#include "TcpClient.h"
#include "GameEngine.h"

int main(int argc, char* args[])
{
	bool isServer = true;
	bool isOnline = false;
	if (argc > 1)
	{
		std::string arg = args[1];
		if (arg == "server") isServer = true;
		else if (arg == "client") isServer = false;
	}

	const std::string windowTitle = isOnline ? (isServer ? "server" : "client") : "offline";

	GameEngine game{ isServer, isOnline, {1200, 800} ,windowTitle };
	game.run();

	return 0;
}