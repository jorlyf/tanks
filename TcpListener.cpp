#include "TcpListener.h"
#include "Registration.h"
#include "ServerNetwork.h"

void TcpListener::Client::receiving()
{
	while (_isRunning.load())
	{
		Packet packet{};
		if (receivePacket(&packet))
		{
			ServerPacketIn serverPacketIn;
			memcpy(&serverPacketIn, &packet, sizeof(Packet));
			serverPacketIn.senderGuid = _guid;
			_listener->addPacketIn(serverPacketIn);
		}
		else
		{
			std::cout << "TcpListener::Client::receiving() error. Disconnect client." << std::endl;
			_shouldBeClosed.store(true);
			return;
		}
	}
}

void TcpListener::Client::sending()
{
	while (_isRunning.load())
	{
		_packetsOutMutex.lock();
		if (_packetsOut.empty())
		{
			_packetsOutMutex.unlock();
			std::chrono::milliseconds t(10);
			std::this_thread::sleep_for(t);
			continue;
		}
		Packet packet = _packetsOut.front();
		_packetsOut.pop();
		_packetsOutMutex.unlock();
		sendPacket(packet);
	}
}

bool TcpListener::Client::receivePacket(Packet* packet)
{
	if (recv(_socket, (char*)packet, sizeof(Packet), MSG_WAITALL) > 0)
	{
		return true;
	}
	return false;
}

int TcpListener::Client::sendPacket(const Packet& packet)
{
	return send(_socket, (const char*)&packet, Packet::SIZE, 0);
}

TcpListener::Client::Client(const GUID& guid, SOCKET socket, TcpListener* listener)
{
	_guid = guid;
	_socket = socket;
	_isRunning = false;
	_listener = listener;
	_shouldBeClosed.store(false);
}

TcpListener::Client::~Client()
{
	std::cout << "TcpListener::Client::~Client\n";
	close();
}

GUID TcpListener::Client::getGuid() const
{
	return _guid;
}

SOCKET TcpListener::Client::getSocket() const
{
	return _socket;
}

void TcpListener::Client::process()
{
	_isRunning = true;
	_receivingThread = std::thread(&TcpListener::Client::receiving, this);
	_sendingThread = std::thread(&TcpListener::Client::sending, this);
}

void TcpListener::Client::addPacketOut(const Packet& packet)
{
	_packetsOutMutex.lock();
	_packetsOut.push(packet);
	_packetsOutMutex.unlock();
}

void TcpListener::Client::close()
{
	if (!_isRunning.load()) return;
	_isRunning.store(false);
	closesocket(_socket);
	if (_receivingThread.joinable()) _receivingThread.join();
	if (_sendingThread.joinable())_sendingThread.join();
}

bool TcpListener::Client::shouldBeClosed()
{
	return _shouldBeClosed.load();
}

void TcpListener::initWSA()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 2);
	int wsaerr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaerr != 0)
	{
		std::cout << "The Winsock dll not found." << std::endl;
		throw std::exception("The Winsock dll not found.");
	}
}

void TcpListener::bindSocket()
{
	sockaddr_in sockAddress;
	sockAddress.sin_family = AF_INET;
	sockAddress.sin_port = htons(_port);
	inet_pton(AF_INET, _ip.c_str(), &sockAddress.sin_addr.S_un.S_addr);

	if (bind(_socket, (SOCKADDR*)&sockAddress, sizeof(sockAddress)) == SOCKET_ERROR)
	{
		std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
		throw std::exception("bind() failed: " + WSAGetLastError());
	}
}

SOCKET TcpListener::acceptSocket()
{
	sockaddr_in sockAddress{};
	sockAddress.sin_family = AF_INET;
	sockAddress.sin_port = htons(_port);
	int len = sizeof(SOCKADDR);
	SOCKET socket = accept(_socket, ((SOCKADDR*)&sockAddress), &len);
	return socket;
}

void TcpListener::processAcceptedSocket(SOCKET acceptedSocket)
{
	Packet packet;
	if (recv(acceptedSocket, (char*)&packet, sizeof(Packet), MSG_WAITALL) <= 0)
	{
		closesocket(acceptedSocket);
		std::cout << "TcpListener::listenSocket() recv error. Close accepted socket." << std::endl;
		return;
	}

	if (packet.type != REGISTRATION_PACKET_TYPE)
	{
		closesocket(acceptedSocket);
		std::cout << "Accepted socket send packet that is not registration. Close accepted socket." << std::endl;
		return;
	}

	Registration registration = *(Registration*)packet.data;
	GUID guid = registration.guid;
	_clientsMutex.lock();
	if (_clients.count(guid) > 0)
	{
		_clientsMutex.unlock();
		std::cout << "Client with that guid exist." << std::endl;
		closesocket(acceptedSocket);
		return;
	}
	_clients[guid] = std::make_unique<Client>(guid, acceptedSocket, this);
	Client* client = _clients[guid].get();
	_clientsMutex.unlock();
	if (_onSocketOpened != nullptr) _onSocketOpened(guid);
	client->process();
}

void TcpListener::addPacketIn(const ServerPacketIn& packet)
{
	_packetsInMutex.lock();
	_packetsIn.push(packet);
	_packetsInMutex.unlock();
}

void TcpListener::pickingClients()
{
	while (_isRunning.load())
	{
		Client* clientToBeClosed = nullptr;
		_clientsMutex.lock();
		for (auto& pair : _clients)
		{
			Client* client = pair.second.get();
			if (client->shouldBeClosed())
			{
				clientToBeClosed = client;
				break;
			}
		}
		_clientsMutex.unlock();

		if (clientToBeClosed != nullptr)
		{
			disconnectClient(clientToBeClosed->getGuid());
		}

		std::chrono::milliseconds sleepTime(500);
		std::this_thread::sleep_for(sleepTime);
	}
}

TcpListener::TcpListener(const std::string& ip, const unsigned short port)
{
	_socket = 0L;
	_ip = ip;
	_port = port;
	_isRunning = false;
}

TcpListener::~TcpListener()
{
	stop();
}

void TcpListener::run()
{
	_isRunning.store(true);
	initWSA();
	_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	bindSocket();

	if (listen(_socket, 1) == SOCKET_ERROR)
	{
		std::cout << "TcpListener listen() failed: " << WSAGetLastError() << std::endl;
		throw std::exception("TcpListener listen() failed: " + WSAGetLastError());
	}

	_pickingClientsThread = std::thread(&TcpListener::pickingClients, this);

	while (_isRunning)
	{
		SOCKET acceptedSocket = acceptSocket();
		if (acceptedSocket == INVALID_SOCKET) continue;

		processAcceptedSocket(acceptedSocket);
	}
}

void TcpListener::stop()
{
	if (!_isRunning) return;
	_isRunning.store(false);
	disconnectAllClients();
	closesocket(_socket);
	WSACleanup();
	if (_pickingClientsThread.joinable()) _pickingClientsThread.join();
}

bool TcpListener::getPacket(ServerPacketIn* packet)
{
	_packetsInMutex.lock();
	if (_packetsIn.empty())
	{
		_packetsInMutex.unlock();
		return false;
	}
	ServerPacketIn serverPacketIn = _packetsIn.front();
	_packetsIn.pop();
	_packetsInMutex.unlock();
	memcpy(packet, &serverPacketIn, sizeof(ServerPacketIn));
	return true;
}

void TcpListener::sendPacket(const ServerPacketOut& packet)
{
	_clientsMutex.lock();
	for (auto& pair : _clients)
	{
		const GUID& guid = pair.first;
		Client* client = pair.second.get();
		if (packet.recipientsGuids.count(guid) > 0)
		{
			client->addPacketOut(packet.toPacket());
		}
	}
	_clientsMutex.unlock();
}

void TcpListener::setSocketOpenHandler(std::function<void(GUID guid)> socketOpenHandler)
{
	_onSocketOpened = socketOpenHandler;
}

void TcpListener::setSocketCloseHandler(std::function<void(GUID guid)> socketCloseHandler)
{
	_onSocketClosed = socketCloseHandler;
}

void TcpListener::disconnectClient(const GUID& guid)
{
	_clientsMutex.lock();
	if (_clients.count(guid) == 0)
	{
		_clientsMutex.unlock();
		return;
	}
	Client* client = _clients[guid].get();
	SOCKET socket = client->getSocket();
	client->close();
	_clients.erase(guid);
	_clientsMutex.unlock();
	if (_onSocketOpened != nullptr) _onSocketClosed(guid);
}

void TcpListener::disconnectAllClients()
{
	_clientsMutex.lock();
	std::vector<GUID> guids;
	for (auto& pair : _clients)
	{
		GUID guid = pair.first;
		guids.push_back(guid);
	}
	_clientsMutex.unlock();

	for (auto& guid : guids)
	{
		disconnectClient(guid);
	}
}

bool operator<(const GUID& guid1, const GUID& guid2)
{
	if (guid1.Data1 != guid2.Data1) return guid1.Data1 < guid2.Data1;
	if (guid1.Data2 != guid2.Data2) return guid1.Data2 < guid2.Data2;
	if (guid1.Data3 != guid2.Data3) return guid1.Data3 < guid2.Data3;
	for (int i = 0; i < 8; i++)
	{
		if (guid1.Data4[i] != guid2.Data4[i]) return guid1.Data4[i] < guid2.Data4[i];
	}
	return false;
}
