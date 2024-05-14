#pragma once
#include <list>
#include "EventHandler.h"

template <typename ...args>
class Event
{
	std::list<EventHandler<args ...>> _handlers;

public:
	void invoke(const args& ... parameters);

	void addListener(const EventHandler<args ...>& handler);
	void removeListener(const EventHandler<args ...>& handler);

	void operator+=(const EventHandler<args ...>& handler);
	void operator-=(const EventHandler<args ...>& handler);
};

#include "Event.cpp"
