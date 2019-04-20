#pragma once

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "TaskManager.h"
#include "SceneManager.h"
#include "Content.h"
#include "Timer.h"

class System
{
public:
	System(TaskManager & tm, SceneManager & sm);
	virtual ~System() = 0;

	virtual JOB_RETURN Update(void * ptr) = 0;
	virtual bool Load() = 0;
	virtual void Close(void* content) = 0;

protected:
	TaskManager & m_task;
	SceneManager & m_scene;
};

inline System::System(TaskManager & tm, SceneManager & sm)
	: m_task(tm), m_scene(sm)
{}

inline System::~System() {}

#endif // !_SYSTEM_H
