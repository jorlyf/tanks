#pragma once
#include <string>

class GameNetwork
{
public:
	virtual void init(const std::string& ip = "0.0.0.0", const unsigned int port = 5025) = 0;
	virtual void update() = 0;
	virtual void run() = 0;
	virtual void stop() = 0;
};
