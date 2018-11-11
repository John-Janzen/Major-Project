#pragma once

#ifndef _SCENE_H
#define _SCENE_H

#include "EntityManager.h"
#include "ComponentManager.h"
#include "ComponentHeaders.h"
#include "GameHeaders.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual bool Load(const std::unique_ptr<EntityManager> & e_manager, const std::unique_ptr<ComponentManager> & c_manager) = 0;

	virtual bool Unload() = 0;

private:

};

inline Scene::Scene() {}

inline Scene::~Scene() {}

#endif // !_SCENE_H
