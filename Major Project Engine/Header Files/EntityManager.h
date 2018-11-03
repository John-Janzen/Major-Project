#pragma once
#include "Entity.h"

#include <unordered_map>

using EntityID = int;

class EntityManager
{
public:
	EntityManager() : id_counter(0) {}

	Entity & create_entity(const std::string & name)
	{
		EntityID id = id_counter;
		++id_counter;
		auto inserted = entities.emplace(id, std::make_unique<Entity>());
		return *(*inserted.first).second;
	}

	~EntityManager() {}

private:
	std::unordered_map<EntityID, std::unique_ptr<Entity>> entities;
	EntityID id_counter;
};