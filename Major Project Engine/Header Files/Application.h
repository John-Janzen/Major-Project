#pragma once

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "ThreadManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"

#include "Render.h"
#include "Input.h"
#include "TestSystem.h"

#include "SceneHeaders.h"

#include "Timer.h"

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
	virtual bool Game_Loop() = 0;
	virtual void Close() = 0;

	//bool load_scene();
	void init_managers(const std::size_t & size);
	void close_managers();

protected:

	std::unique_ptr<Scene> current_scene;
	std::unique_ptr<Timer> timer;

	bool game_running = true;
	GAME_STATE _state;
	
	std::unique_ptr<Render> renderer;
	std::unique_ptr<Input> input;
	std::unique_ptr<TestSystem> test_system;
	GLfloat frame_rate;

};

inline Application::Application(const std::size_t & num_of_threads)
	: _state(LOADING)
{
	this->init_managers(num_of_threads);
	renderer = std::make_unique<Render>();
	input = std::make_unique<Input>();
	timer = std::make_unique<Timer>();
	test_system = std::make_unique<TestSystem>();

	FileLoader::Instance().Init();

	test_system->Load();

	if (!renderer->Load())
	{
		printf("Error Initializing Renderer");
		_state = EXITING;
	}

	int display_index = 0, mode_index = 0;
	SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

	if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0) {
		SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
	}
	timer->set_time_lock((1000.0f / (float)mode.refresh_rate));
}

inline Application::~Application()
{
	renderer.reset();
	renderer = nullptr;
	input.reset();
	input = nullptr;
	timer.reset();
	timer = nullptr;
	test_system.reset();
	test_system = nullptr;
	this->close_managers();
}

inline bool Application::Load(std::unique_ptr<Scene> newScene)
{

	if (!newScene->Load())
	{
		printf("Error Making current scene");
		return false;
	}
	current_scene = std::move(newScene);

	for (auto & element : ComponentManager::Instance().find_all_of_type<RenderComponent>())
	{
		renderer->init_render_component(element);
	}

	return true;
}

inline void Application::init_managers(const std::size_t & size)
{
	ThreadManager::Instance().Init(size);
	ComponentManager::Instance().Init();
	EntityManager::Instance().Init();
	FileLoader::Instance().Init();
}

inline void Application::close_managers()
{
	ThreadManager::Instance().Close();
	ComponentManager::Instance().Close();
	EntityManager::Instance().Close();
	FileLoader::Instance().Close();
}

#endif // !_APPLICATION_H
