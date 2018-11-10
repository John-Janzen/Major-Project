#pragma once

#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include "Entity.h"
#include "Quad.h"

#include <unordered_map>
#include <type_traits>

typedef int EntityID;
typedef std::map<EntityID, std::shared_ptr<Entity>> EntityStorage;

class EntityManager
{
public:
	EntityManager() : id_counter(0x100) {}

	~EntityManager() { entities.clear(); }

	std::shared_ptr<Entity> & create_entity(const std::string & name)
	{
		auto inserted = entities.emplace(++id_counter, std::make_shared<Entity>(name, id_counter));
		return (*inserted.first).second;
	}

	const int & create_entity_id(const std::string & name)
	{
		auto inserted = entities.emplace(++id_counter, std::make_shared<Entity>(name, id_counter));
		return inserted.first->first;
	}

	template <class T>
	const int & add_entity(const std::string & name)
	{
		auto inserted = entities.emplace(++id_counter, std::make_shared<T>(name, id_counter));
		return inserted.first->first;
	}

	void flag_dead_entity(const EntityID & entity_id)
	{
		entities.find(entity_id)->second.get()->set_death();
	}

	void flag_dead_entity(std::shared_ptr<Entity> & entity)
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

private:
	EntityStorage entities;
	EntityID id_counter;
};

#endif // !_ENTITYMANAGER_H