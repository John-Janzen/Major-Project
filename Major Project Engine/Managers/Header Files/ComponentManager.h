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
		components = ComponentStorage();
	}
	~ComponentManager() {}

	void Close() 
	{
		std::for_each(components.begin(), components.end(), [](ComponentStorage::value_type & comp) {
			delete comp.second;
		});
		components.clear();
	}

	template<class T>
	std::list<T> find_all_of_type() const
	{
		std::list<T> full_list;
		for (auto element : components)
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
			components.emplace(entity_id, std::move(comp));
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
			components.emplace(0, std::move(comp));
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
		auto range = components.equal_range(entity_id);
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
		for (ComponentStorage::iterator it = components.begin(); it != components.end(); ++it)
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
		auto range = components.equal_range(entity_id);
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
		auto range = components.equal_range(entity_id);
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

	ComponentStorage components;
};

#endif // COMPONENTHEADER_H
