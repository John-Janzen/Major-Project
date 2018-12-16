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

	ComponentManager * GetCompManager() const { return m_components; }
	EntityManager * GetEntityManager() const { return m_entities; }

	const EntityID & GetPlayerID() const { return _player1_ID; }
	const EntityID & GetCameraID() const { return _camera; }

protected:
	EntityManager * m_entities;
	ComponentManager * m_components;

	EntityID _player1_ID;
	EntityID _camera;
};

inline Scene::Scene() 
{
	m_entities = new EntityManager();
	m_components = new ComponentManager();
}

inline Scene::~Scene() 
{
	if (m_entities != nullptr)
		delete m_entities;
	if (m_components != nullptr)
		delete m_components;
}

#endif // !_SCENE_H
