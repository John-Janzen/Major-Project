#pragma once

#ifndef _SCENE_H
#define _SCENE_H

#include "GameHeaders.h"
#include "GameComponents.h"
#include "ComponentHeaders.h"

#include "iostream"

enum SCENE_SELECTION
{
	NULL_SCENE,
	MAIN_SCENE
};

class SceneManager;
class Scene
{
public:
	Scene() {}

	virtual ~Scene() {}

	virtual bool Load(SceneManager & sm) = 0;

	virtual bool Unload() = 0;

protected:

};

#endif // !_SCENE_H
