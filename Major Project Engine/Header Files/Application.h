#pragma once

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "Entity.h"
#include "ThreadManager.h"
#include "EntityManager.h"
#include "ComponentManager.h"
#include "Render.h"
#include "FileLoader.h"
#include "SceneHeaders.h"

#include <vector>
#include <ctime>
#include <chrono>

enum GAME_STATE
{
	NULL_STATE,
	LOADING,
	PLAYING,
	EXITING
};

class Application
{
public:
	explicit Application(const std::size_t & num_of_threads);
	~Application();

	virtual bool Load() = 0;
	virtual bool Game_Loop() = 0;
	virtual void Close() = 0;

protected:
	std::unique_ptr<ThreadManager> thread_manager;
	std::unique_ptr<EntityManager> entity_manager;
	std::unique_ptr<ComponentManager> component_manager;

	std::unique_ptr<Scene> current_scene;

	std::clock_t c_start;
	SDL_Event sdl_event;
	std::unique_ptr<Render> renderer;
	bool game_running = true;
	GAME_STATE _state;
	std::chrono::time_point<std::chrono::steady_clock> t_start, t_end;
};

inline Application::Application(const std::size_t & num_of_threads)
	: _state(LOADING)
{
	thread_manager = std::make_unique<ThreadManager>(num_of_threads);
	entity_manager = std::make_unique<EntityManager>();
	component_manager = std::make_unique<ComponentManager>();
}

inline Application::~Application()
{
	thread_manager.reset();
	thread_manager = nullptr;
	entity_manager.reset();
	entity_manager = nullptr;
	component_manager.reset();
	component_manager = nullptr;
}

#endif // !_APPLICATION_H
