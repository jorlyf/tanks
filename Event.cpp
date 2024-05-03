#pragma once
#include "Event.h"
#include "utils.h"

template<typename T>
void Event<T>::invoke(T parameter)
{
	for (const EventHandler<T>& handler : _handlers)
	{
		handler(parameter);
	}
}

template<typename T>
void Event<T>::addListener(const EventHandler<T>& handler)
{
	_handlers.push_back(handler);
}

template<typename T>
void Event<T>::removeListener(const EventHandler<T>& handler)
{
	typename std::list<EventHandler<T>>::const_iterator it = std::find(_handlers.cbegin(), _handlers.cend(), handler);
	if (it != _handlers.cend())
	{
		_handlers.erase(it);
	}
}

template<typename T>
void Event<T>::operator+=(const EventHandler<T>& handler)
{
	addListener(handler);
}

template<typename T>
void Event<T>::operator-=(const EventHandler<T>& handler)
{
	removeListener(handler);
}