#include "ServerPacketOut.h"

Packet ServerPacketOut::toPacket() const
{
	Packet packet;
	packet.type = type;
	memcpy(packet.data, data, SIZE - sizeof(int));
	return packet;
}
