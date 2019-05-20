#pragma once

#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include <array>
#include <vector>

#include "SceneHeaders.h"
#include "EventHandler.h"

class SceneManager : public EventListener
{
public:

	static const std::size_t MAX_ENT = 1024;

	enum CompTypes
	{
		TRANSFORM,
		PHYSICS,
		RENDER,
		CONTROLLER,
		COUNT
	};

	SceneManager()
	{
		EventHandler::Instance().SubscribeEvent(LEFT_MOUSE_BUTTON, this);
	}

	~SceneManager();

	void HandleEvent(const EventType & e, void * data)
	{
		switch (e)
		{
		case LEFT_MOUSE_BUTTON:
		{
			Entity * & project = this->CreateEntity("Projectile");
			this->AddComponent(SceneManager::TRANSFORM, new Transform(project->_id, btVector3(0.f, 10.f, 0.f)));

			this->AddComponent(SceneManager::RENDER, new SphereRenderComponent(project->_id));
			EventHandler::Instance().SendEvent(EventType::RENDER_NEW_OBJECT, this->FindComponent(SceneManager::RENDER, project->_id));

			this->AddComponent(SceneManager::PHYSICS, new SpherePhysicsComponent(project->_id));
			EventHandler::Instance().SendEvent(EventType::PHYSICS_NEW_OBJECT, this->FindComponent(SceneManager::PHYSICS, project->_id));
		}
			break;
		default:
			break;
		}
	}

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
				default:
					break;
				}
				return comp;
			}
		}
		return nullptr;
	}

	Entity * & CreateEntity(const std::string & name)
	{
		_entities[count_ent] = new Entity(name, id_count++);
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
