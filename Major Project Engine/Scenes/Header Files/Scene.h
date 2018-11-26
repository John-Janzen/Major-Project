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

	const std::unique_ptr<ComponentManager> & get_comp_manager() const { return component_manager; }
	const std::unique_ptr<EntityManager> & get_ent_manager() const { return entity_manager; }

	const EntityID & get_player_id() const { return _player1_ID; }
	const EntityID & get_camera_id() const { return _camera; }

protected:
	std::unique_ptr<EntityManager> entity_manager;
	std::unique_ptr<ComponentManager> component_manager;

	EntityID _player1_ID;
	EntityID _camera;
};

inline Scene::Scene() 
{
	entity_manager = std::make_unique<EntityManager>();
	component_manager = std::make_unique<ComponentManager>();
	entity_manager->create_entity<Camera>(_camera)->Load(component_manager);
}

inline Scene::~Scene() 
{
	entity_manager->Close();
	component_manager->Close();
	entity_manager.reset();
	entity_manager = nullptr;
	component_manager.reset();
	component_manager = nullptr;
}

#endif // !_SCENE_H
