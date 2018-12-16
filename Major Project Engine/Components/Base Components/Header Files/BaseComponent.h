#pragma once

#ifndef _BASECOMPONENT_H
#define _BASECOMPONENT_H

typedef int EntityID;

class BaseComponent
{
public:
	BaseComponent() : death_flag(false) {}
	virtual ~BaseComponent() {}

	void SetDeathFlag() { death_flag = true; }

private:
	bool death_flag;
};

#endif // !_BASECOMPONENT_H
