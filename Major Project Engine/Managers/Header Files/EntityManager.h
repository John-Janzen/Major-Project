#pragma once

#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include "GameHeaders.h"

#include <unordered_map>
#include <type_traits>
#include <memory>

typedef int EntityID;
typedef std::map<EntityID, Entity*> EntityStorage;

class EntityManager
{
public:

	EntityManager() : id_counter(0x100)
	{
		entities = EntityStorage();
	}
	~EntityManager() {}

	void Close()
	{
		std::for_each(entities.begin(), entities.end(), [] (EntityStorage::value_type& entity) {
			delete entity.second;
		});
		entities.clear();
	}

	Entity * create_entity()
	{
		auto inserted = entities.emplace(++id_counter, new Entity(std::string("Default" + ' ' + id_counter), id_counter));
		return static_cast<Entity*>(inserted.first->second);
	}

	template <class T>
	T * create_entity()
	{
		auto inserted = entities.emplace(++id_counter, new T(std::string("Default" + ' ' + id_counter), id_counter));
		return static_cast<T*>(inserted.first->second);
	}

	template <class T>
	T * create_entity(EntityID & id)
	{
		auto inserted = entities.emplace(++id_counter, new T(std::string("Default" + ' ' + id_counter), id_counter));
		id = inserted.first->first;
		return static_cast<T*>(inserted.first->second);
	}

	template <class T>
	T * create_entity(const std::string & name)
	{
		auto inserted = entities.emplace(++id_counter, new T(name, id_counter));
		return static_cast<T*>(inserted.first->second);
	}

	template <class T>
	const EntityID & create_entity(const std::string & name, T * & entity)
	{
		auto inserted = entities.emplace(++id_counter, new T(name, id_counter));
		entity = static_cast<T*>(inserted.first->second);
		return inserted.first->first;
	}

	template <class T>
	T * create_entity(const std::string & name, EntityID & entity_id)
	{
		auto inserted = entities.emplace(++id_counter, new T(name, id_counter));
		entity_id = inserted.first->first;
		return static_cast<T*>(inserted.first->second);
	}

	Entity * & find_entity(const EntityID _id)
	{
		return entities.find(_id)->second;
	}

	void flag_dead_entity(const EntityID entity_id)
	{
		entities.find(entity_id)->second->set_death();
	}

	void flag_dead_entity(Entity * & entity)
	{
		entity->set_death();
	}

	void destroy_dead_entities()
	{
		for (auto it = entities.begin(); it != entities.end(); ++it)
		{
			if ((*it).second->get_death())
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