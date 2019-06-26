#pragma once

#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include <array>
#include <vector>

#include "SceneHeaders.h"

class SceneManager
{
public:

	static const std::size_t DEFAULT_MAX_ENT = 1024;

	enum CompTypes
	{
		TRANSFORM,
		PHYSICS,
		RENDER,
		CONTROLLER,
		CAMERA,
		COUNT
	};

	SceneManager() { _entities.reserve(DEFAULT_MAX_ENT); }

	~SceneManager();

	void LoadScene(Scene * && scene)
	{
		scene->Load(*this);
	}

	Entity * FindEntity(const EntityID & id)
	{
		for (auto & ent : _entities)
		{
			if (ent->_id == id)
			{
				return ent;
			}
		}
		return nullptr;
	}

	BaseComponent * AddComponent(const CompTypes & type, BaseComponent * && base)
	{
		_components[type].emplace_back(base);
		return _components[type].back();
	}

	std::vector<BaseComponent*> & GetComponents(const CompTypes & type)
	{
		return _components[type];
	}

	BaseComponent * FindComponent(const CompTypes & type, const EntityID & id)
	{
		for (const auto & comp : _components[type])
		{
			if (comp->_id == id)
			{
				switch (type)
				{
				case TRANSFORM:
					assert(dynamic_cast<Transform*>(comp));
					break;
				case RENDER:
					assert(dynamic_cast<RenderComponent*>(comp));
					break;
				case PHYSICS:
					assert(dynamic_cast<PhysicsComponent*>(comp));
					break;
				case CONTROLLER:
					assert(dynamic_cast<PlayerControllerComponent*>(comp));
					break;
				case CAMERA:
					assert(dynamic_cast<CameraComponent*>(comp));
					break;
				default:
					break;
				}
				return comp;
			}
		}
		return nullptr;
	}

	Entity * & CreateEntity(const std::string & name, const EntityType & type)
	{
		_entities.emplace_back(new Entity(name, id_count++, type));
		entity_count++;
		return _entities.back();
	}

private:
	static int id_count;
	
	std::uint16_t entity_count = 0;
	std::array<std::vector<BaseComponent*>, COUNT> _components;
	std::vector<Entity*> _entities;
};


#endif // !_SCENEMANAGER_H
