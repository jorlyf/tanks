#pragma once
#include "EventHandler.h"
#include "NextEventHandlerId.h"

template<typename ...args>
EventHandler<args ...>::EventHandler(const std::function<void(args ...)>& action)
{
	_id = nextEventHandlerId++;
	_action = action;
}

template<typename ...args>
EventHandler<args ...>::EventHandler(const EventHandler<args ...>& copy)
{
	_id = copy._id;
	_action = copy._action;
}

template<typename ...args>
void EventHandler<args ...>::operator()(const args &... parameters) const
{
	_action(parameters...);
}

template<typename ...args>
bool EventHandler<args ...>::operator==(const EventHandler<args ...>& other) const
{
	return _id == other._id;
}