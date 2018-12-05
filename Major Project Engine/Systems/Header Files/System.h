#pragma once

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "ThreadManager.h"
#include "Scene.h"

enum SYSTEM_TYPE
{
	NULL_SYSTEM,
	RENDER_SYSTEM,
	INPUT_SYSTEM,
	TEST_SYSTEM
};

class System
{
public:
	System();
	virtual ~System() = 0;

	virtual bool Load(const Content * content) = 0;
	virtual void Close(const Content * content) = 0;

private:

};

inline System::System() {}

inline System::~System() {}

#endif // !_SYSTEM_H
