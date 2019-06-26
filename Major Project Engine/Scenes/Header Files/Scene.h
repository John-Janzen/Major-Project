#pragma once

#ifndef _SCENE_H
#define _SCENE_H

#include "Entity.h"
#include "GameComponents.h"
#include "ComponentHeaders.h"

enum SCENE_SELECTION
{
	NULL_SCENE,
	MAIN_SCENE
};

/*
Forward declaration of the Scene Manager object just so 
We can put all objects into it.
*/
class SceneManager;

/*
The Scene class is the base class of all scene classes.
*/
class Scene
{
public:
	Scene() {}

	virtual ~Scene() {}

	/*
	Load the scene
	*/
	virtual bool Load(SceneManager & sm) = 0;

	/*
	Unload the Scene
	Unused
	*/
	virtual bool Unload() = 0;

protected:

};

#endif // !_SCENE_H
