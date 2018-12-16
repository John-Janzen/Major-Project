#pragma once

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "TaskManager.h"
#include "Scene.h"
#include "Content.h"

class System
{
public:
	System();
	virtual ~System() = 0;

	virtual JOB_RETURN Load(void* content) = 0;
	virtual void Close(void* content) = 0;

private:

};

inline System::System() {}

inline System::~System() {}

#endif // !_SYSTEM_H
