#pragma once

#ifndef _BASECOMPONENT_H
#define _BASECOMPONENT_H

#include <cstdint>

struct BaseComponent
{
public:
	BaseComponent(const std::uint16_t & id) : _id(id), death_flag(false) {}
	virtual ~BaseComponent() {}
	bool death_flag;

	std::uint16_t _id;
};

#endif // !_BASECOMPONENT_H
