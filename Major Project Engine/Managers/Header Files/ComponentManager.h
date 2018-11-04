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

	void add_component(const Entity & entity, std::shared_ptr<BaseComponent> comp)
	{
		components.emplace(entity.get_id(), comp);
	}

	template <class T>
	bool get_component(const Entity & entity, T & base_comp)
	{
		bool found = false;
		auto range = components.equal_range(entity.get_id());
		std::for_each(range.first, range.second, [&base_comp, &found](ComponentStorage::value_type & x)
		{
			if (&dynamic_cast<T&>(*x.second) != nullptr)
			{
				base_comp = dynamic_cast<T&>(*x.second);
				found = true;
			}
		});
		return found;
	}

private:
	ComponentStorage components;
};