#pragma once
#include <set>
#include <guiddef.h>
#include "Packet.h"

struct ServerPacketOut
{
	const static unsigned int SIZE = Packet::SIZE;
	int type;
	char data[SIZE - sizeof(int)];
	std::set<GUID> recipientsGuids;

	Packet toPacket() const;
};
