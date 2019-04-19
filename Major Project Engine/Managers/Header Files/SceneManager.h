#pragma once

#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include <array>
#include <vector>

#include "SceneHeaders.h"

class SceneManager
{
public:

	static const std::size_t MAX_ENT = 16;

	enum CompTypes
	{
		TRANSFORM,
		PHYSICS,
		RENDER,
		COUNT
	};

	SceneManager();
	~SceneManager();

	void LoadScene(Scene * && scene)
	{
		p_scene = scene;
		scene->Load(*this);
	}

	void AddComponent(const CompTypes & type, BaseComponent * && base)
	{
		_components[type].emplace_back(base);
	}

	std::vector<BaseComponent*> & GetComponents(const CompTypes & type)
	{
		return _components[type];
	}

	BaseComponent * FindComponent(const CompTypes & type, const EntityID & id)
	{
		for (auto & comp : _components[type])
		{
			if (comp->_id == id)
			{
				return comp;
			}
		}
		return nullptr;
	}

	Entity * & CreateEntity(const std::string & name)
	{
		_entities[count_ent] = new Entity(name, id_count);
		count_ent++;
		return _entities[count_ent - 1];
	}

private:
	static int id_count;

	Scene * p_scene;
	
	std::uint16_t count_ent = 0;
	std::array<std::vector<BaseComponent*>, COUNT> _components;
	std::array<Entity*, MAX_ENT> _entities = { nullptr };
};


#endif // !_SCENEMANAGER_H
