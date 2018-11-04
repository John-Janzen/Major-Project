#pragma once
#include "Entity.h"

#include <unordered_map>

typedef int EntityID;
typedef std::map<EntityID, std::unique_ptr<Entity>> EntityStorage;

class EntityManager
{
public:
	EntityManager() : id_counter(0x100) {}

	Entity & create_entity(const std::string & name)
	{
		EntityID id = id_counter++;
		auto inserted = entities.emplace(id, std::make_unique<Entity>(name, id));
		return *(*inserted.first).second;
	}

	~EntityManager() {}

private:
	EntityStorage entities;
	EntityID id_counter;
};