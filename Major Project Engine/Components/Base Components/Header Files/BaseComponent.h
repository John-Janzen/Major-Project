#pragma once

#ifndef _BASECOMPONENT_H
#define _BASECOMPONENT_H

struct BaseComponent
{
public:
	BaseComponent() : death_flag(false) {}
	virtual ~BaseComponent() {}
	bool death_flag;
};

#endif // !_BASECOMPONENT_H
