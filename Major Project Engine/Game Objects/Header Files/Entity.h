#pragma once

#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>

typedef int EntityID;

class Entity
{
public:
	Entity(const std::string & name, EntityID id) : _name(name), _id(id), death_flag(false) {}
	~Entity() {}

	EntityID GetID() const { return _id; }
	std::string GetName() const { return _name; }
	void SetDeathFlag() { death_flag = true; }
	bool GetDeathFlag() { return death_flag; }

protected:
	std::string _name;
	EntityID _id;
	bool death_flag;
};


#endif // !_ENTITY_H
