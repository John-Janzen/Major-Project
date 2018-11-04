#pragma once
#include "Entity.h"

#include <unordered_map>

typedef int EntityID;
typedef std::map<EntityID, std::shared_ptr<Entity>> EntityStorage;

class EntityManager
{
public:
	EntityManager() : id_counter(0x100) {}

	std::shared_ptr<Entity> & create_entity(const std::string & name)
	{
		EntityID id = id_counter++;
		auto inserted = entities.emplace(id, std::make_shared<Entity>(name, id));
		return (*inserted.first).second;
	}

	const int & create_entity_id(const std::string & name)
	{
		EntityID id = id_counter++;
		auto inserted = entities.emplace(id, std::make_shared<Entity>(name, id));
		return inserted.first->first;
	}

	void flag_dead_entity(const EntityID & entity_id)
	{
		entities.find(entity_id)->second.get()->set_death();
	}

	void flag_dead_entity(std::unique_ptr<Entity> & entity)
	{
		entity.get()->set_death();
	}

	void destroy_dead_entities()
	{
		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			if ((*it).second.get()->get_death() && (*it).second.unique())
			{
				it = entities.erase(it);
			}
		}
	}

	const EntityStorage & retreive_list()
	{
		return entities;
	}

	~EntityManager() {}

private:
	EntityStorage entities;
	EntityID id_counter;
};