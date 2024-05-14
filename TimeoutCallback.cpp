#include "TimeoutCallback.h"
#include "DeltaTime.h"

TimeoutCallback::TimeoutCallback(const std::function<void(void)>& callback, const float delay)
{
	_callback = callback;
	_delay = delay;
}

void TimeoutCallback::update()
{
	_elapsedMs += DeltaTime::getDt() * 1000;
	if (_elapsedMs >= _delay)
	{
		_callback();
		_isInvoked = true;
	}
}

bool TimeoutCallback::isInvoked() const
{
	return _isInvoked;
}
