#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <ctime>

static class DeltaTime
{
	static std::clock_t _prevClock;
	static float _dt;

public:
	static void update();
	static float getDt();
};
