#pragma once

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "ThreadManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "Render.h"
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

	virtual bool Load(std::unique_ptr<Scene> newScene);
	virtual bool Game_Loop();
	virtual void Close();

	bool load_scene();

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
	renderer = std::make_unique<Render>();
	FileLoader::Instance().Init();


	if (!renderer->Load())
	{
		printf("Error Initializing Renderer");
		_state = EXITING;
	}
}

inline Application::~Application()
{
	thread_manager.reset();
	thread_manager = nullptr;
	entity_manager.reset();
	entity_manager = nullptr;
	component_manager.reset();
	component_manager = nullptr;
	renderer.reset();
	renderer = nullptr;
	FileLoader::Instance().Close();
}

inline bool Application::Load(std::unique_ptr<Scene> newScene)
{
	c_start = std::clock();
	t_start = std::chrono::high_resolution_clock::now();

	if (!newScene->Load(entity_manager, component_manager))
	{
		printf("Error Making current scene");
		return false;
	}
	current_scene = std::move(newScene);

	for (auto & element : component_manager->find_all_of_type<RenderComponent>())
	{
		renderer->init_render_component(element);
	}

	return true;
}

inline bool Application::Game_Loop()
{

	return true;
}

inline void Application::Close()
{

}

inline bool Application::load_scene()
{
	return false;
}

#endif // !_APPLICATION_H
