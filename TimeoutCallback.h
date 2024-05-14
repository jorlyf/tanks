#pragma once
#include <functional>

class TimeoutCallback
{
	std::function<void(void)> _callback;
	float _delay;
	float _elapsedMs = 0;
	bool _isInvoked = false;

public:
	TimeoutCallback(const std::function<void(void)>& callback, const float delay);

	void update();
	bool isInvoked() const;
};
