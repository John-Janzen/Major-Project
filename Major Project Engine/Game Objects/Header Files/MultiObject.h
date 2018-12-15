#pragma once
#include "Entity.h"
class MultiObject :
	public Entity
{
public:
	MultiObject(const std::string & name, EntityID id) : Entity(name, id) {}
	~MultiObject() {}
};
