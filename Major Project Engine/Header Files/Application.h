#pragma once

#include "Entity.h"
#include "ThreadManager.h"
#include "EntityManager.h"

#include <SDL.h>
#include <vector>
#include <ctime>
#include <chrono>

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

	std::clock_t c_start;
	SDL_Event sdl_event;
	bool game_running = true;
	std::chrono::time_point<std::chrono::steady_clock> t_start, t_end;
};

inline Application::Application(const std::size_t & num_of_threads)
{
	thread_manager = std::make_unique<ThreadManager>(num_of_threads);
	entity_manager = std::make_unique<EntityManager>();
}

inline Application::~Application()
{
	thread_manager.reset();
	thread_manager = nullptr;
	entity_manager.reset();
	entity_manager = nullptr;
}

