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
private:
	EntityID _player1_ID;
};

#endif // !
