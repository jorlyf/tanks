#pragma once
#include <set>
#include <guiddef.h>
#include "Packet.h"

struct ServerPacketOut
{
	const static unsigned int SIZE = 1024;
	int type;
	char data[SIZE - sizeof(int)];
	std::set<GUID> recipientsGuids;

	Packet toPacket() const;
};
