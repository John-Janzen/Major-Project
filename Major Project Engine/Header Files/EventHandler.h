#pragma once

#ifndef _EVENTHANDLER_H
#define _EVENTHANDLET_H

#include <map>
#include <list>

static std::uint16_t id_counter = 0;

enum EventType
{
	NULL_TYPE,
	GAME_CLOSED,
	OPEN_DEBUGGER,
	FIND_TRANSFORM,
	GET_COMPONENT_LIST
};

class EventListener
{
public:
	EventListener()
	{
		_id = id_counter++;
	}

	virtual void HandleEvent(const EventType & e, void * data) = 0;
	virtual ~EventListener() {}

	bool operator==(const EventListener & other)
	{
		return (this->_id == other._id);
	}

private:
	std::uint16_t _id;
};

class EventHandler
{
public:
	EventHandler() {}
	~EventHandler() {}

	void SendEvent(const EventType & e, void * data = nullptr)
	{
		for (auto & listener : *event_map[e])
		{
			if (listener != nullptr)
				listener->HandleEvent(e, data);
		}
	}

	void SubscribeEvent(const EventType & e, EventListener * listener)
	{
		if (event_map.find(e) == event_map.end())
			event_map.emplace(e, new std::list<EventListener*>());

		event_map[e]->emplace_back(listener);
	}

	void UnSubscribeEvent(const EventType & e, EventListener * listener)
	{
		for (auto it : *event_map[e])
		{
			if ((*it) == *listener)
			{
				event_map[e]->remove(it);
			}
		}
	}

private:
	typedef std::list<EventListener*> * Listeners;
	std::map<EventType, Listeners> event_map;
};

#endif // !_EVENTHANDLER_H
