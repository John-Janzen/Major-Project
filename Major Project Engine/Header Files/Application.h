#pragma once

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "ComponentManager.h"
#include "EntityManager.h"
#include "TaskManager.h"
#include "ThreadManager.h"

#include "Render.h"
#include "Input.h"
#include "Physics.h"
#include "TestSystem.h"

#include "SceneHeaders.h"

#include "Timer.h"

#include <vector>
#include <ctime>
#include <chrono>



class Application
{
public:
	Application();
	~Application();

	bool InitApp(const std::size_t & num_of_threads);

	bool LoadApp();

	bool GameLoop();

	bool LoadScene(const SCENE_SELECTION type);
	
	bool CloseApp();

private:

	Render * renderer = nullptr;
	Input * input = nullptr;
	TestSystem * test_system = nullptr;
	Physics * physics = nullptr;

	ThreadManager * m_thread = nullptr;
	TaskManager * m_task = nullptr;

protected:

	enum GAME_STATE
	{
		NULL_STATE,
		INITIALIZING,
		LOADING,
		PLAYING,
		PAUSED,
		DELOAD,
		EXITING
	};

	bool game_running = true;
	GAME_STATE _state;

	Scene * current_scene;

	GLfloat frame_rate;
	SDL_Window * sdl_window;
	SDL_GLContext sdl_context, sdl_init_context;

	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
};

#endif // !_APPLICATION_H
