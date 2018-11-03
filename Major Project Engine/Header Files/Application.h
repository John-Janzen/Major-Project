#pragma once

#include "Entity.h"
#include "ThreadManager.h"
#include "EntityManager.h"

#include <vector>

class Application
{
public:
	Application();
	virtual ~Application() = 0;

	virtual bool Load() = 0;
	virtual void Game_Loop() = 0;
	virtual void Close() = 0;

protected:
	std::unique_ptr<ThreadManager> thread_manager;
	std::unique_ptr<EntityManager> entity_manager;
};

