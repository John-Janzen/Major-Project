#pragma once

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "ThreadManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"

#include "Render.h"
#include "Input.h"
#include "Physics.h"
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

	virtual bool Load() = 0;
	bool Load_App();
	virtual bool Game_Loop() = 0;
	virtual void Close() = 0;

	//bool load_scene();
	void init_managers(const std::size_t & size);
	void close_managers();

protected:

	std::shared_ptr<Timer> timer;

	bool game_running = true;
	GAME_STATE _state;
	
	Render * renderer;
	Input * input;
	TestSystem * test_system;
	Physics * physics;

	Scene * current_scene;

	GLfloat frame_rate;
};

inline Application::Application(const std::size_t & num_of_threads)
	: _state(LOADING)
{
	this->init_managers(num_of_threads);
	renderer = new Render();
	input = new Input();
	physics = new Physics();
	timer = std::make_shared<Timer>();
	test_system = new TestSystem();
}

inline Application::~Application()
{
	delete renderer;
	delete input;
	delete physics;
	delete test_system;
	timer.reset();
	timer = nullptr;
	
	if (current_scene != nullptr)
	{
		delete(current_scene);
		current_scene = nullptr;
	}
	this->close_managers();
}

inline bool Application::Load_App()
{
	//FileLoader::Instance().Init();
	
	ThreadManager::Instance().register_job(bind_function(&TestSystem::Load, test_system));
	ThreadManager::Instance().register_job(bind_function(&Physics::Load, physics));
	ThreadManager::Instance().register_job(bind_function(&Render::Load, renderer), nullptr, RENDER_TYPE);

	int display_index = 0, mode_index = 0;
	SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

	if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0) {
		SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
	}
	timer->set_time_lock(1000.0f / (float)mode.refresh_rate);

	return true;
}

inline void Application::init_managers(const std::size_t & size)
{
	ThreadManager::Instance().Init(size);
	FileLoader::Instance().Init();
}

inline void Application::close_managers()
{
	ThreadManager::Instance().Close();
	FileLoader::Instance().Close();
}

#endif // !_APPLICATION_H
