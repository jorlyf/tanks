#include "TcpClient.h"
#include "Registration.h"
#include <combaseapi.h>

void TcpClient::receiving()
{
	while (_isRunning.load())
	{
		Packet packet{};
		if (receivePacket(&packet))
		{
			_packetsInMutex.lock();
			_packetsIn.push(packet);
			_packetsInMutex.unlock();
		}
		else
		{
			disconnect();
			std::cout << "TcpListener::Client::receiving() error. Disconnect" << std::endl;
		}
	}
}

void TcpClient::sending()
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
		_sendPacket(packet);
	}
}

bool TcpClient::receivePacket(Packet* packet)
{
	if (recv(_socket, (char*)packet, sizeof(Packet), MSG_WAITALL) > 0)
	{
		return true;
	}
	return false;
}

int TcpClient::_sendPacket(const Packet& packet)
{
	return send(_socket, (const char*)&packet, sizeof(Packet), 0);
}

void TcpClient::initWSA()
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

void TcpClient::connectSocket()
{
	sockaddr_in sockAddress{};
	sockAddress.sin_family = AF_INET;
	sockAddress.sin_port = htons(_port);
	inet_pton(AF_INET, _ip.c_str(), &sockAddress.sin_addr.S_un.S_addr);

	if (connect(_socket, (SOCKADDR*)&sockAddress, sizeof(sockAddress)) == SOCKET_ERROR)
	{
		std::cout << "connect() failed: " << WSAGetLastError() << std::endl;
		throw std::exception("connect() failed: " + WSAGetLastError());
	}

	registrate();
}

void TcpClient::registrate()
{
	Packet packet;
	packet.type = REGISTRATION_PACKET_TYPE;
	memcpy(packet.data, (const void*)&_guid, sizeof(GUID));

	sendPacket(packet);
	_registrated.store(true);
	if (_onSocketOpened != nullptr) _onSocketOpened();
}

TcpClient::TcpClient(const std::string& ip, const unsigned short port)
{
	CoCreateGuid(&_guid);
	_socket = 0L;
	_ip = ip;
	_port = port;
	_isRunning = false;
	_registrated.store(false);
}

TcpClient::~TcpClient()
{
	stop();
}

void TcpClient::run()
{
	_isRunning.store(true);
	try
	{
		initWSA();
		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		connectSocket();
		_receivingThread = std::thread(&TcpClient::receiving, this);
		_sendingThread = std::thread(&TcpClient::sending, this);
	}
	catch (const std::exception&)
	{
		std::cout << "TcpClient::run() exception" << std::endl;
		stop();
	}
}

void TcpClient::stop()
{
	_isRunning.store(false);
	disconnect();
	if (_receivingThread.joinable()) _receivingThread.join();
	if (_sendingThread.joinable())_sendingThread.join();
	WSACleanup();
}

GUID TcpClient::getGuid() const
{
	return _guid;
}

bool TcpClient::getPacket(Packet* packet)
{
	_packetsInMutex.lock();
	if (_packetsIn.empty())
	{
		_packetsInMutex.unlock();
		return false;
	}
	Packet packetIn = _packetsIn.front();
	_packetsIn.pop();
	_packetsInMutex.unlock();
	memcpy(packet, &packetIn, sizeof(Packet));
	return true;
}

void TcpClient::sendPacket(const Packet& packet)
{
	_packetsOutMutex.lock();
	_packetsOut.push(packet);
	_packetsOutMutex.unlock();
}

void TcpClient::setSocketOpenHandler(std::function<void()> socketOpenHandler)
{
	_onSocketOpened = socketOpenHandler;
}

void TcpClient::setSocketCloseHandler(std::function<void()> socketCloseHandler)
{
	_onSocketClosed = socketCloseHandler;
}

void TcpClient::disconnect()
{
	_isRunning.store(false);
	closesocket(_socket);
	if (_onSocketClosed != nullptr) _onSocketClosed();
}

bool TcpClient::isRegistrated()
{
	return _registrated.load();
}
