#pragma once
#include <list>
#include "EventHandler.h"

template <typename T>
class Event
{
	std::list<EventHandler<T>> _handlers;

public:
	void invoke(T parameter);

	void addListener(const EventHandler<T>& handler);
	void removeListener(const EventHandler<T>& handler);

	void operator+=(const EventHandler<T>& handler);
	void operator-=(const EventHandler<T>& handler);
};

#include "Event.cpp"
