#pragma once
#include <guiddef.h>
#include "Packet.h"

struct ServerPacketIn
{
	const static unsigned int SIZE = 1024;
	int type;
	char data[SIZE - sizeof(int)];
	GUID senderGuid;

	Packet toPacket() const;
};
