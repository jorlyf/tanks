#include "Packet.h"

Packet::Packet()
{
	type = 0;
	memset(data, 0, SIZE - sizeof(int));
}

Packet::Packet(int type, const void* data)
{
	this->type = type;
	memcpy(this->data, data, SIZE - sizeof(int));
}
