#pragma once

#ifndef _SCENEMANAGER_H
#define _SCENEMANAGER_H

#include "SceneHeaders.h"

#include <array>
#include <vector>

/*
The Scene Manager class controls everything that is contained within the scene.
Usually the components and the entities.
*/
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

	SceneManager();

	~SceneManager();

	/*
	Redirector function that calls the scenes load function
	*/
	void LoadScene(Scene * && scene);

	/*
	Find the entity by the id of the object
	*/
	Entity * FindEntity(const Entity::EntityID & id);
	

	/*
	Add a component to the list by the type of component and the 
	component itself.
	*/
	BaseComponent * AddComponent(const CompTypes & type, BaseComponent * && base);


	/*
	Get the list of all components by the type.
	*/
	std::vector<BaseComponent*> & GetComponents(const CompTypes & type);


	/*
	Find a single component by the type and the id of the component.
	*/
	BaseComponent * FindComponent(const CompTypes & type, const Entity::EntityID & id);

	/*
	Create a new Entity to use for the components.
	*/
	Entity * & CreateEntity(const std::string & name, const Entity::EntityType & type);

private:
	static int id_count;
	
	std::uint16_t entity_count = 0;
	std::array<std::vector<BaseComponent*>, COUNT> _components;
	std::vector<Entity*> _entities;
};

#endif // !_SCENEMANAGER_H
