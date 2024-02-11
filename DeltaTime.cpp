#include "DeltaTime.h"

std::clock_t DeltaTime::_prevClock = 0L;
float DeltaTime::_dt = 0.f;

void DeltaTime::update()
{
	std::clock_t now = std::clock();
	_dt = float(now - _prevClock) / CLOCKS_PER_SEC;
	_prevClock = now;
}

float DeltaTime::getDt()
{
	return _dt;
}
