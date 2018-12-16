#pragma once

#ifndef _COMPONENTHEADER_H
#define _COMPONENTHEADER_H

#include "ComponentHeaders.h"
#include "GameComponents.h"

#include <unordered_map>
#include <map>
#include <algorithm>
#include <memory>
#include <iostream>

typedef int EntityID;
typedef std::unordered_multimap <EntityID, BaseComponent*> ComponentStorage;

class ComponentManager
{
public:

	ComponentManager() 
	{
		_components = ComponentStorage();
	}

	~ComponentManager() 
	{
		std::cout << "Component Manager Destructor Called" << std::endl;
		std::unordered_multimap<EntityID, BaseComponent*>::iterator _components_it = _components.begin();
		while (_components_it != _components.end())
		{
			if ((*_components_it).second != nullptr)
				delete (*_components_it).second;
			_components_it = _components.erase(_components_it);
		}
	}

	template<class T>
	std::map<EntityID, T> FindAllTypes() const
	{
		std::map<EntityID, T> copy_list;
		for (auto it = _components.begin(); it != _components.end(); ++it)
		{
			if (dynamic_cast<T>(it->second) != nullptr)
			{
				copy_list.emplace(it->first, static_cast<T>(it->second));
			}
		}
		return copy_list;
	}

	template <class T>
	void AddComponent(const EntityID entity_id, T comp)
	{
		if (!HasComponent<T>(entity_id))
		{
			_components.emplace(entity_id, std::move(comp));
		}
		else
		{
			printf("Duplicate Components not allowed, ID: %d", entity_id);
		}
	}

	template <class T>
	void AddComponent(T comp)
	{
		if (!HasComponent<T>(0))
		{
			_components.emplace(0, std::move(comp));
		}
		else
		{
			printf("Duplicate Components not allowed, NO ID Available");
		}
	}

	template <class T>
	T GetComponent(const EntityID _id)
	{
		auto range = _components.equal_range(_id);
		for (auto it = range.first; it != range.second; ++it)
		{
			if (dynamic_cast<T>(it->second) != nullptr)
			{
				return static_cast<T>(it->second);
			}
		}
		return nullptr;
	}

	template <class T>
	bool GetComponent(const EntityID entity_id, T & base_comp)
	{
		bool found = false;
		auto range = _components.equal_range(entity_id);
		std::for_each(range.first, range.second, [&base_comp, &found](ComponentStorage::value_type & x)
		{
			if (dynamic_cast<T>(x.second) != nullptr)
			{
				base_comp = static_cast<T>(x.second);
				found = true;
			}
		});
		return found;
	}

	template <class T>
	bool HasComponent(const EntityID entity_id)
	{
		bool found = false;
		auto range = _components.equal_range(entity_id);
		std::for_each(range.first, range.second, [&found](ComponentStorage::value_type & x)
		{
			if (dynamic_cast<T>(x.second) != nullptr)
			{
				found = true;
			}
		});
		return found;
	}

private:
	ComponentStorage _components;
};

#endif // COMPONENTHEADER_H
