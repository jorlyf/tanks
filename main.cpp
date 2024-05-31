#include <iostream>
#include <thread>
#include "TcpListener.h"
#include "TcpClient.h"
#include "GameEngine.h"
#include "MapEditorEngine.h"
#include "TextureManager.h"
#include "FontManager.h"

int main(int argc, char* args[])
{
	bool isServer = true;
	bool isOnline = true;
	bool isMapEditor = false;
	std::string socket = "0.0.0.0:5025";
	if (argc > 1)
	{
		std::string arg = args[1];
		if (arg == "server") isServer = true;
		else if (arg == "client") isServer = false;
		else if (arg == "editor") isMapEditor = true;

		if (argc > 2)
		{
			std::string arg2 = args[2];
			socket = arg2;
		}
	}

	TextureManager::load();
	FontManager::load();

	if (isMapEditor)
	{
		MapEditorEngine mapEditor{ "default", {1200, 800} };
		mapEditor.run();
	}
	else
	{
		const std::string windowTitle = isOnline ? (isServer ? "server" : "client") : "offline";

		GameEngine game{ isServer, isOnline, socket, {1200, 800}, windowTitle };
		game.run();
	}

	return 0;
}