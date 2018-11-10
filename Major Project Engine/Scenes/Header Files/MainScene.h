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
	bool Load(const std::unique_ptr<EntityManager> & e_manager, const std::unique_ptr<ComponentManager> & c_manager);
	bool Unload();
private:

};

#endif // !
