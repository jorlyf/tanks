#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <functional>
#include <thread>
#include <map>
#include <queue>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Packet.h"
#include "ServerPacketOut.h"
#include "ServerPacketIn.h"
#include <mutex>

bool operator < (const GUID& guid1, const GUID& guid2);

class TcpListener
{
	class Client
	{
		TcpListener* _listener;
		SOCKET _socket;
		GUID _guid;
		std::thread _receivingThread;
		std::thread _sendingThread;
		std::atomic_bool _isRunning;
		std::atomic_bool _shouldBeClosed;

		std::queue<Packet> _packetsOut;
		std::mutex _packetsOutMutex;

		void receiving();
		void sending();
		bool receivePacket(Packet* packet);
		int sendPacket(const Packet& packet);

	public:
		Client(const GUID& guid, SOCKET socket, TcpListener* listener);
		~Client();
		GUID getGuid() const;
		SOCKET getSocket() const;
		void process();
		void addPacketOut(const Packet& packet);
		void close();
		bool shouldBeClosed();
	};

	SOCKET _socket;
	std::string _ip;
	unsigned short _port;
	std::atomic_bool _isRunning;

	std::map<GUID, std::unique_ptr<Client>> _clients;
	std::mutex _clientsMutex;

	std::function<void(const GUID& guid)> _onSocketOpened = nullptr;
	std::function<void(const GUID& guid)> _onSocketClosed = nullptr;

	std::queue<ServerPacketIn> _packetsIn;
	std::mutex _packetsInMutex;

	std::thread _pickingClientsThread;

	void initWSA();
	void bindSocket();
	SOCKET acceptSocket();
	void processAcceptedSocket(SOCKET acceptedSocket);
	void addPacketIn(const ServerPacketIn& packet);
	void pickingClients();

public:
	TcpListener(const std::string& ip = "127.0.0.1", const unsigned short port = 5025);
	~TcpListener();
	void run();
	void stop();
	bool getPacket(ServerPacketIn* packet);
	void sendPacket(const ServerPacketOut& packet);
	void setSocketOpenHandler(std::function<void(GUID guid)> socketOpenHandler);
	void setSocketCloseHandler(std::function<void(GUID guid)> socketCloseHandler);
	void disconnectClient(const GUID& guid);
	void disconnectAllClients();
};
