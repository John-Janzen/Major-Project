#pragma once

#ifndef _SCENE_H
#define _SCENE_H

#include "EntityManager.h"
#include "ComponentManager.h"

enum SCENE_SELECTION
{
	NULL_SCENE,
	MAIN_SCENE
};

class Scene
{
public:
	Scene();

	virtual ~Scene();

	virtual bool Load() = 0;

	virtual bool Unload() = 0;

	ComponentManager * GetCompManager() const { return component_manager; }
	EntityManager * GetEntityManager() const { return entity_manager; }

	const EntityID & GetPlayerID() const { return _player1_ID; }
	const EntityID & GetCameraID() const { return _camera; }

protected:
	EntityManager * entity_manager;
	ComponentManager * component_manager;

	EntityID _player1_ID;
	EntityID _camera;
};

inline Scene::Scene() 
{
	entity_manager = new EntityManager();
	component_manager = new ComponentManager();
	entity_manager->CreateEntity<Camera>(_camera)->Load(component_manager);
}

inline Scene::~Scene() 
{
	if (entity_manager != nullptr)
		delete entity_manager;
	if (component_manager != nullptr)
		delete component_manager;
}

#endif // !_SCENE_H
