#pragma once

#ifndef _BASECOMPONENT_H
#define _BASECOMPONENT_H

#include <cstdint>

/*
The Base Component that all component derive from.
Only has an ID to identify what Entity it belongs to.
*/
struct BaseComponent
{
public:
	BaseComponent(const std::uint16_t & id) : _id(id) {}
	virtual ~BaseComponent() {}

	std::uint16_t _id;
};

#endif // !_BASECOMPONENT_H
