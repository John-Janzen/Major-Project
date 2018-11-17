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

protected:
	std::unique_ptr<EntityManager> entity_manager;
	std::unique_ptr<ComponentManager> component_manager;
};

inline Scene::Scene() {}

inline Scene::~Scene() {}

#endif // !_SCENE_H
