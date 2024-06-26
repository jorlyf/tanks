#pragma once
#include <vcruntime_string.h>

struct Packet
{
	const static unsigned int SIZE = 4096;
	int type;
	char data[SIZE - sizeof(int)];

	Packet();
	Packet(int type, const void* data);
};
