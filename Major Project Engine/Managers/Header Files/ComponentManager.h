#pragma once

#ifndef _COMPONENTHEADER_H
#define _COMPONENTHEADER_H

#include "ComponentHeaders.h"
#include "GameComponents.h"

#include <unordered_map>
#include <algorithm>
#include <memory>

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
		std::unordered_multimap<EntityID, BaseComponent*>::iterator _components_it = _components.begin();
		while (_components_it != _components.end())
		{
			if ((*_components_it).second != nullptr)
				delete (*_components_it).second;
			_components_it = _components.erase(_components_it);
		}
	}

	template<class T>
	std::list<T> find_all_of_type() const
	{
		std::list<T> full_list;
		for (auto element : _components)
		{
			if (dynamic_cast<T>(element.second) != nullptr)
			{
				full_list.push_back(static_cast<T>(element.second));
			}
		}
		return full_list;
	}

	template <class T>
	void add_component(const EntityID entity_id, T comp)
	{
		if (!has_component<T>(entity_id))
		{
			_components.emplace(entity_id, std::move(comp));
		}
		else
		{
			printf("Duplicate Components not allowed, ID: %d", entity_id);
		}
	}

	template <class T>
	void add_component(T comp)
	{
		if (!has_component<T>(0))
		{
			_components.emplace(0, std::move(comp));
		}
		else
		{
			printf("Duplicate Components not allowed, NO ID Available");
		}
	}

	template <class T>
	bool get_component(const EntityID entity_id, T & base_comp)
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
	T * & get_component()
	{
		for (ComponentStorage::iterator it = _components.begin(); it != _components.end(); ++it)
		{
			if (dynamic_cast<T>(it->second) != nullptr)
			{
				return static_cast<T>(it->second);
			}
		}
		return nullptr;
	}

	template <class T>
	T & get_component(const EntityID entity_id)
	{
		T temp = nullptr;
		auto range = _components.equal_range(entity_id);
		std::for_each(range.first, range.second, [&temp](ComponentStorage::value_type & x)
		{
			if (dynamic_cast<T>(x.second) != nullptr)
			{
				temp = static_cast<T>(x.second);
			}
		});
		return temp;
	}

	template <class T>
	bool has_component(const EntityID entity_id)
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
