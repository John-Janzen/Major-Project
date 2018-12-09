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

	ComponentManager * get_comp_manager() const { return component_manager; }
	EntityManager * get_ent_manager() const { return entity_manager; }

	const EntityID & get_player_id() const { return _player1_ID; }
	const EntityID & get_camera_id() const { return _camera; }

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
	entity_manager->create_entity<Camera>(_camera)->Load(component_manager);
}

inline Scene::~Scene() 
{
	entity_manager->Close();
	component_manager->Close();
	if (entity_manager != nullptr)
		delete entity_manager;
	if (component_manager != nullptr)
		delete component_manager;
}

#endif // !_SCENE_H
