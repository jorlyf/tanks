#pragma once
#include "Event.h"
#include "utils.h"

template<typename ...args>
void Event<args ...>::invoke(const args& ... parameters)
{
	for (const EventHandler<args ...>& handler : _handlers)
	{
		handler(parameters...);
	}
}

template<typename ...args>
void Event<args ...>::addListener(const EventHandler<args ...>& handler)
{
	_handlers.push_back(handler);
}

template<typename ...args>
void Event<args ...>::removeListener(const EventHandler<args ...>& handler)
{
	typename std::list<EventHandler<args ...>>::const_iterator it = std::find(_handlers.cbegin(), _handlers.cend(), handler);
	if (it != _handlers.cend())
	{
		_handlers.erase(it);
	}
}

template<typename ...args>
void Event<args ...>::operator+=(const EventHandler<args ...>& handler)
{
	addListener(handler);
}

template<typename ...args>
void Event<args ...>::operator-=(const EventHandler<args ...>& handler)
{
	removeListener(handler);
}