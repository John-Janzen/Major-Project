#pragma once

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "Timer.h"
#include "EventHandler.h"
#include "TaskManager.h"
#include "ComponentHeaders.h"
#include "SceneManager.h"
#include "GameStates.h"

#include <cassert>

class System : public EventListener
{
public:
	System(TaskManager & tm, SceneManager & sm);
	virtual ~System() = 0;

	virtual JOB_RETURN Update(void * ptr) = 0;
	virtual bool Load() = 0;
	virtual void Close(void* content) = 0;

	virtual void HandleEvent() {};
protected:
	//bool paused = true;
	GAME_STATE gs = GAME_STATE::INITIALIZING;

	TaskManager & m_task;
	SceneManager & m_scene;
};

inline System::System(TaskManager & tm, SceneManager & sm)
	: m_task(tm), m_scene(sm)
{}

inline System::~System() {}

#endif // !_SYSTEM_H
