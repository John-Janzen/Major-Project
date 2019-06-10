#pragma once

#ifndef _MAINSCENE_H
#define _MAINSCENE_H

#include "Scene.h"

#include <iostream>

class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();
	bool Load(SceneManager & sm);

	bool Unload();

	static const int NUM_MULTI_OBJECTS = 6;

private:
	bool LoadBullet(SceneManager & sm);

	bool LoadWalls(SceneManager & sm);

	bool LoadQuad(SceneManager & sm);

	bool LoadFloor(SceneManager & sm);

	bool LoadPlayer(SceneManager & sm);

	bool LoadMultiObject(SceneManager & sm, const float x, const float y, const float z);

};

#endif // !
