#pragma once

#ifndef _MAINSCENE_H
#define _MAINSCENE_H

#include "Scene.h"
class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();
	bool Load();
	bool Unload();

	static const int NUM_MULTI_OBJECTS = 3;

private:

	bool LoadCamera();
	bool LoadQuad();
	bool LoadFloor();
	bool LoadPlayer();
	bool LoadMultiObject(const float x, const float y, const float z);

};

#endif // !
