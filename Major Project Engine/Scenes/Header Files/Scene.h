#pragma once

#ifndef _SCENE_H
#define _SCENE_H

#include "EntityManager.h"
#include "ComponentManager.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();

	virtual bool Load() = 0;

	virtual bool Unload() = 0;

private:

};

inline Scene::Scene() {}

inline Scene::~Scene() {}

#endif // !_SCENE_H
