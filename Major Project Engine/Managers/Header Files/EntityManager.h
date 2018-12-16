#pragma once

#ifndef _ENTITYMANAGER_H
#define _ENTITYMANAGER_H

#include "GameHeaders.h"

#include <list>

typedef std::list<Entity*> EntityStorage;

class EntityManager
{
public:

	EntityManager() : id_counter(0x100)
	{
	}

	~EntityManager() 
	{
		
	}

	template <class T>
	T * CreateEntity()
	{
		_entities.emplace_back(new T(std::string("Default" + ' ' + id_counter), id_counter));
		id_counter++;
		return static_cast<T*>(_entities.back());
	}

	template <class T>
	void CreateEntity(EntityID & id)
	{
		_entities.emplace_back(new T(std::string("Default" + ' ' + id_counter), id_counter));
		id = id_counter;
		id_counter++; 
		//return static_cast<T*>(_entities.back());
	}


	template <class T>
	T * CreateEntity(const std::string name)
	{
		_entities.emplace_back(new T(name, id_counter));
		id_counter++;
		return static_cast<T*>(_entities.back());
	}

	template <class T>
	const EntityID & CreateEntity(const std::string name, T * & entity)
	{
		_entities.emplace_back(new T(name, id_counter));
		entity = static_cast<T*>(_entities.back());
		id_counter++;
		return entity->GetID();
	}

	template <class T>
	T * CreateEntity(const std::string name, EntityID & entity_id)
	{
		_entities.emplace_back(new T(name, id_counter));
		entity_id = id_counter;
		id_counter++;
		return static_cast<T*>(_entities.back());
	}

	template <class T>
	T * FindEntity()
	{
		for (auto entity : _entities)
		{
			if (dynamic_cast<T*>(entity) != nullptr)
			{
				return static_cast<T*>(entity);
			}
		}
	}

	void FlagDeath(const EntityID entity_id)
	{
		for (auto entity : _entities)
		{
			if (entity->GetID() == entity_id)
			{
				entity->SetDeathFlag();
			}
		}
	}

	void FlagDeath(Entity * & entity)
	{
		entity->SetDeathFlag();
	}

	EntityStorage GetEntities()
	{
		return _entities;
	}

private:
	EntityStorage _entities;
	EntityID id_counter;

};

#endif // !_ENTITYMANAGER_H