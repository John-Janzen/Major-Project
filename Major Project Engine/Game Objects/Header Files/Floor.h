#pragma once
#include "Entity.h"
class Floor :
	public Entity
{
public:
	Floor(const std::string name, const EntityID id) : Entity(name , id){}
	~Floor() {}

};