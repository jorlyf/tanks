#pragma once
#include <string>
#include <guiddef.h>

class GUIDUtils
{
public:
	static std::string guidToString(const GUID& guid);
};

