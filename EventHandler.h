#pragma once
#include <functional>

template<typename ...args>
class EventHandler
{
	size_t _id;
	std::function<void(args ...)> _action;

public:
	EventHandler(const std::function<void(args ...)>& action);
	EventHandler(const EventHandler& copy);

	void operator()(const args& ... parameters) const;
	bool operator==(const EventHandler<args ...>& other) const;
};

#include "EventHandler.cpp"
