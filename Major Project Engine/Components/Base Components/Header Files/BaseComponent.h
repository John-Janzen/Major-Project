#pragma once

#ifndef _BASECOMPONENT_H
#define _BASECOMPONENT_H

#include <memory>

class BaseComponent
{
public:
	BaseComponent() : death_flag(false) {}
	virtual ~BaseComponent() {}

	void set_death() { death_flag = true; }
private:
	bool death_flag;
};

#endif // !_BASECOMPONENT_H
