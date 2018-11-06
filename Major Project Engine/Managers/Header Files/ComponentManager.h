#pragma once
#include "ComponentHeaders.h"
#include "Entity.h"

#include <unordered_map>
#include <algorithm>
#include <memory>

typedef int EntityID;
typedef std::unordered_multimap <EntityID, std::shared_ptr<BaseComponent>> ComponentStorage;

class ComponentManager
{
public:

	template<class T>
	void add_component(const std::shared_ptr<Entity> & entity, std::shared_ptr<T> comp)
	{
		if (!has_component<T>(entity))
		{
			components.emplace(entity->get_id(), comp);
		}
		else
		{
			printf("Duplicate Components not allowed, %s", entity->get_name().c_str());
		}
	}

	template<class T>
	void add_component(const int & entity_id, std::shared_ptr<T> comp)
	{
		if (!has_component<T>(entity_id))
		{
			components.emplace(entity_id, comp);
		}
		else
		{
			printf("Duplicate Components not allowed, %d", entity_id);
		}
	}

	template <class T>
	bool get_component(const std::shared_ptr<Entity> & entity, std::shared_ptr<T> & base_comp)
	{
		bool found = false;
		auto range = components.equal_range(entity->get_id());
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

	template<class T>
	std::shared_ptr<T> get_component(const int & entity_id)
	{
		std::shared_ptr<T> temp;
		auto range = components.equal_range(entity_id);
		std::for_each(range.first, range.second, [&temp](ComponentStorage::value_type & x)
		{
			if (std::dynamic_pointer_cast<T>(x.second) != nullptr)
			{
				temp = std::static_pointer_cast<T>(x.second);
			}
		});
		return temp;
	}

	template<class T>
	bool has_component(const std::shared_ptr<Entity> & entity)
	{
		bool found = false;
		auto range = components.equal_range(entity->get_id());
		std::for_each(range.first, range.second, [&found](ComponentStorage::value_type & x)
		{
			if (std::dynamic_pointer_cast<T>(x.second) != nullptr)
			{
				found = true;
			}
		});
		return found;
	}

	template<class T>
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