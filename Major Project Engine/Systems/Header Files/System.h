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

/*
The System Class is the base class for all systems.
It comes with a load update and close function.
It has to take a reference to the task manager and scene manager.
*/
class System : public EventListener
{
public:
	System(TaskManager & tm, SceneManager & sm) : m_scene(sm), m_task(tm) {}
	virtual ~System() {};

	/*
	Generic Update that is a job funtion
	*/
	virtual JOB_RETURN Update(void * ptr) = 0;

	/*
	Load the system when called
	*/
	virtual bool Load() = 0;

	/*
	Close the system when called
	*/
	virtual void Close(void* content) = 0;

	/*
	Handle the event when one is sent
	*/
	virtual void HandleEvent() {};
protected:
	//bool paused = true;
	GAME_STATE gs = GAME_STATE::INITIALIZING;

	TaskManager & m_task;		// Allow any system to post a job to the queue
	SceneManager & m_scene;		// This is only here so that the systems have easy access to the trasform components
};

#endif // !_SYSTEM_H
