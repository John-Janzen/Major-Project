#pragma once

#ifndef _COMPONENTHEADER_H
#define _COMPONENTHEADER_H

#include "ComponentHeaders.h"

#include <unordered_map>
#include <algorithm>
#include <memory>

typedef int EntityID;
typedef std::unordered_multimap <EntityID, std::shared_ptr<BaseComponent>> ComponentStorage;

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
			comp.second.reset();
			comp.second = nullptr;
		});
		components.clear();
	}

	template<class T>
	std::list<std::shared_ptr<T>> find_all_of_type()
	{
		std::list<std::shared_ptr<T>> full_list;
		for (auto const & element : components)
		{
			if (std::dynamic_pointer_cast<T>(element.second) != nullptr)
			{
				full_list.push_back(std::static_pointer_cast<T>(element.second));
			}
		}
		return full_list;
	}

	template <class T>
	void add_component(const int & entity_id, std::shared_ptr<T> comp)
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
	void add_component(std::shared_ptr<T> comp)
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
	bool get_component(const int & entity_id, std::shared_ptr<T> & base_comp)
	{
		bool found = false;
		auto range = components.equal_range(entity_id);
		std::for_each(range.first, range.second, [&base_comp, &found](ComponentStorage::value_type & x)
		{
			if (std::dynamic_pointer_cast<T>(x.second) != nullptr)
			{
				base_comp = std::static_pointer_cast<T>(x.second);
				found = true;
			}
		});
		return found;
	}

	template <class T>
	std::shared_ptr<T> & get_component()
	{
		for (auto & element : components)
		{
			if (std::dynamic_pointer_cast<T>(element.second) != nullptr)
			{
				return std::static_pointer_cast<T>(element.second);
			}
		}
	}

	template <class T>
	std::shared_ptr<T> & get_component(const int & entity_id)
	{
		std::shared_ptr<T> * temp = nullptr;
		auto range = components.equal_range(entity_id);
		std::for_each(range.first, range.second, [&temp](ComponentStorage::value_type & x)
		{
			if (std::dynamic_pointer_cast<T>(x.second) != nullptr)
			{
				temp = &std::static_pointer_cast<T>(x.second);
			}
		});
		return temp;
	}

	template <class T>
	bool has_component(const int & entity_id)
	{
		bool found = false;
		auto range = components.equal_range(entity_id);
		std::for_each(range.first, range.second, [&found](ComponentStorage::value_type & x)
		{
			if (std::dynamic_pointer_cast<T>(x.second) != nullptr)
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
