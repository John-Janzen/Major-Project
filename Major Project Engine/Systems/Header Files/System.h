#pragma once

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "Timer.h"
#include "EventHandler.h"
#include "TaskManager.h"
#include "ComponentHeaders.h"
#include "SceneManager.h"

#include <cassert>

class System : public EventListener
{
public:
	System(TaskManager & tm, SceneManager & sm, EventHandler & eh);
	virtual ~System() = 0;

	virtual JOB_RETURN Update(void * ptr) = 0;
	virtual bool Load() = 0;
	virtual void Close(void* content) = 0;

	virtual void HandleEvent() {};
protected:
	TaskManager & m_task;
	SceneManager & m_scene;

	EventHandler & h_event;
};

inline System::System(TaskManager & tm, SceneManager & sm, EventHandler & eh)
	: m_task(tm), m_scene(sm), h_event(eh)
{}

inline System::~System() {}

#endif // !_SYSTEM_H
