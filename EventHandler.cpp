#pragma once
#include "EventHandler.h"
#include "NextEventHandlerId.h"

template<typename T>
EventHandler<T>::EventHandler(const std::function<void(T)>& action)
{
	_id = nextEventHandlerId++;
	_action = action;
}

template<typename T>
EventHandler<T>::EventHandler(const EventHandler<T>& copy)
{
	_id = copy._id;
	_action = copy._action;
}

template<typename T>
void EventHandler<T>::operator()(T parameter) const
{
	_action(parameter);
}

template<typename T>
bool EventHandler<T>::operator==(const EventHandler<T>& other) const
{
	return _id == other._id;
}