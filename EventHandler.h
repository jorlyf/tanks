#pragma once
#include <functional>

template<typename T>
class EventHandler
{
	size_t _id;
	std::function<void(T)> _action;

public:
	EventHandler(const std::function<void(T)>& action);
	EventHandler(const EventHandler& copy);

	void operator()(T) const;
	bool operator==(const EventHandler<T>& other) const;
};

#include "EventHandler.cpp"
