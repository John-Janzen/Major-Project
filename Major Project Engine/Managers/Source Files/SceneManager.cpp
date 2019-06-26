#include "SceneManager.h"

int SceneManager::id_count = 0;

SceneManager::SceneManager()
{
	_entities.reserve(DEFAULT_MAX_ENT);
}

SceneManager::~SceneManager() {}

void SceneManager::LoadScene(Scene *&& scene)
{
	scene->Load(*this);
}

Entity * SceneManager::FindEntity(const Entity::EntityID & id)
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

BaseComponent * SceneManager::AddComponent(const CompTypes & type, BaseComponent *&& base)
{
	_components[type].emplace_back(base);
	return _components[type].back();
}

std::vector<BaseComponent*>& SceneManager::GetComponents(const CompTypes & type)
{
	return _components[type];
}

BaseComponent * SceneManager::FindComponent(const CompTypes & type, const Entity::EntityID & id)
{
	for (const auto & comp : _components[type])
	{
		if (comp->_id == id)
		{
			switch (type)
			{
			case TRANSFORM:
				assert(dynamic_cast<Transform*>(comp));			// Just a check to make sure no components have been mixed up
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

Entity *& SceneManager::CreateEntity(const std::string & name, const Entity::EntityType & type)
{
	_entities.emplace_back(new Entity(name, id_count++, type));
	entity_count++;
	return _entities.back();
}
