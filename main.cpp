#include <iostream>
#include <thread>
#include "TcpListener.h"
#include "TcpClient.h"
#include "GameEngine.h"
#include "MapEditorEngine.h"
#include "TextureManager.h"

int main(int argc, char* args[])
{
	bool isServer = true;
	bool isOnline = false;
	bool isMapEditor = false;
	if (argc > 1)
	{
		std::string arg = args[1];
		if (arg == "server") isServer = true;
		else if (arg == "client") isServer = false;
		else if (arg == "editor") isMapEditor = true;
	}

	TextureManager::load();

	if (isMapEditor)
	{
		MapEditorEngine mapEditor{ "default", {1200, 800} };
		mapEditor.run();
	}
	else
	{
		const std::string windowTitle = isOnline ? (isServer ? "server" : "client") : "offline";

		GameEngine game{ isServer, isOnline, {1200, 800} ,windowTitle };
		game.run();
	}

	return 0;
}