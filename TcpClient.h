#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <functional>
#include <queue>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <mutex>
#include "Packet.h"

class TcpClient
{
	GUID _guid;
	SOCKET _socket;
	std::string _ip;
	unsigned short _port;
	std::atomic_bool _isRunning;
	std::atomic_bool _registrated;

	std::mutex _packetsInMutex;
	std::queue<Packet> _packetsIn;
	std::mutex _packetsOutMutex;
	std::queue<Packet> _packetsOut;

	std::thread _receivingThread;
	std::thread _sendingThread;

	std::function<void()> _onSocketOpened = nullptr;
	std::function<void()> _onSocketClosed = nullptr;

	void receiving();
	void sending();
	bool receivePacket(Packet* packet);
	int _sendPacket(const Packet& packet);
	void initWSA();
	void connectSocket();
	void registrate();

public:
	TcpClient(const std::string& ip = "127.0.0.1", const unsigned short port = 5025);
	~TcpClient();
	void run();
	void stop();
	bool getPacket(Packet* packet);
	void sendPacket(const Packet& packet);
	void setSocketOpenHandler(std::function<void()> socketOpenHandler);
	void setSocketCloseHandler(std::function<void()> socketCloseHandler);
	void disconnect();
	bool isRegistrated();
};
