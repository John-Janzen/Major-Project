#pragma once

#ifndef _SYSTEM_H
#define _SYSTEM_H

class System
{
public:
	System();
	virtual ~System() = 0;

	virtual bool Load() = 0;
	virtual void Close() = 0;

private:
};

inline System::System() {}

inline System::~System() {}

#endif // !_SYSTEM_H