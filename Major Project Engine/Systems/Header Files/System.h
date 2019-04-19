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
	System(TaskManager & tm);
	virtual ~System() = 0;

	virtual void Update(SceneManager * & sm) = 0;
	virtual bool Load(SceneManager * & sm) = 0;
	virtual void Close(void* content) = 0;

protected:
	TaskManager & m_task;
};

inline System::System(TaskManager & tm)
	: m_task(tm)
{}

inline System::~System() {}

#endif // !_SYSTEM_H
