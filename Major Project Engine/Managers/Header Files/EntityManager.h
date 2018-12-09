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
		_entities = EntityStorage();
	}
	~EntityManager() 
	{
		std::map<EntityID, Entity*>::iterator entity_it = _entities.begin();
		while (entity_it != _entities.end())
		{
			if ((*entity_it).second != nullptr)
				delete (*entity_it).second;
			entity_it = _entities.erase(entity_it);
		}
	}

	Entity * create_entity()
	{
		auto inserted = _entities.emplace(id_counter, new Entity(std::string("Default" + ' ' + id_counter), id_counter));
		id_counter++;
		return static_cast<Entity*>(inserted.first->second);
	}

	template <class T>
	T * create_entity()
	{
		auto inserted = _entities.emplace(id_counter, new T(std::string("Default" + ' ' + id_counter), id_counter));
		id_counter++;
		return static_cast<T*>(inserted.first->second);
	}

	template <class T>
	T * create_entity(EntityID & id)
	{
		auto inserted = _entities.emplace(id_counter, new T(std::string("Default" + ' ' + id_counter), id_counter));
		id = inserted.first->first;
		id_counter++;
		return static_cast<T*>(inserted.first->second);
	}

	template <class T>
	T * create_entity(const std::string & name)
	{
		auto inserted = _entities.emplace(id_counter, new T(name, id_counter));
		id_counter++;
		return static_cast<T*>(inserted.first->second);
	}

	template <class T>
	const EntityID & create_entity(const std::string & name, T * & entity)
	{
		auto inserted = _entities.emplace(id_counter, new T(name, id_counter));
		entity = static_cast<T*>(inserted.first->second);
		id_counter++;
		return inserted.first->first;
	}

	template <class T>
	T * create_entity(const std::string & name, EntityID & entity_id)
	{
		auto inserted = _entities.emplace(id_counter, new T(name, id_counter));
		entity_id = inserted.first->first;
		id_counter++;
		return static_cast<T*>(inserted.first->second);
	}

	Entity * & find_entity(const EntityID _id)
	{
		return _entities.find(_id)->second;
	}

	void flag_dead_entity(const EntityID entity_id)
	{
		_entities.find(entity_id)->second->set_death();
	}

	void flag_dead_entity(Entity * & entity)
	{
		entity->set_death();
	}

	void destroy_dead__entities()
	{
		for (auto it = _entities.begin(); it != _entities.end(); ++it)
		{
			if ((*it).second->get_death())
			{
				it = _entities.erase(it);
			}
		}
	}

	const EntityStorage & retreive_list()
	{
		return _entities;
	}

private:

	EntityStorage _entities;
	EntityID id_counter;
};

#endif // !_ENTITYMANAGER_H