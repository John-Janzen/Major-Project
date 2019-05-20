#pragma once

#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>

typedef std::uint16_t EntityID;

struct Entity
{
public:
	Entity(const std::string & name, EntityID id) 
		: _name(name), _id(id), death_flag(false) 
	{}
	~Entity() {}

	std::string _name;
	EntityID _id;
	bool death_flag;
};


#endif // !_ENTITY_H
