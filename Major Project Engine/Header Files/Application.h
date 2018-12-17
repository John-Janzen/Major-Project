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
	explicit Application(const std::size_t & num_of_threads);
	~Application();

	virtual bool Load() = 0;
	bool LoadApplication();
	virtual bool GameLoop() = 0;
	virtual void Close() = 0;

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

	ThreadManager * _threadpool;

	bool game_running = true;
	GAME_STATE _state;
	
	Render * renderer;
	Input * input;
	TestSystem * test_system;
	Physics * physics;

	Scene * current_scene;

	GLfloat frame_rate;
	SDL_Window * sdl_window;
	SDL_GLContext sdl_context, sdl_init_context;

	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
};

inline Application::Application(const std::size_t & num_of_threads)
	: _state(INITIALIZING)
{
	_threadpool = new ThreadManager(num_of_threads);
	TaskManager::Instance().Init();
	Timer::Instance().Init();
}

inline Application::~Application()
{
	_threadpool->Close();

	if (_threadpool != nullptr) delete _threadpool;
	if (renderer != nullptr) delete renderer;
	if (input != nullptr) delete input;
	if (physics != nullptr) delete physics;
	if (test_system != nullptr) delete test_system;
	
	if (current_scene != nullptr) delete(current_scene);
	
	TaskManager::Instance().Close();

	SDL_DestroyWindow(sdl_window);
	sdl_window = NULL;
	SDL_Quit();
}

inline bool Application::LoadApplication()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Issue Initing Everything %s\n", SDL_GetError());
		return false;
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

		sdl_window = SDL_CreateWindow("Major Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
		if (sdl_window == NULL)
		{
			printf("Error creating window");
			return false;
		}
		else
		{
			sdl_context = SDL_GL_CreateContext(sdl_window);
			sdl_init_context = SDL_GL_CreateContext(sdl_window);
			if (sdl_init_context == NULL)
			{
				printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
				return false;
			}
			else
			{
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					printf("Error initializing GLEW! %s", glewGetErrorString(glewError));
					return false;
				}
				/*if (SDL_GL_SetSwapInterval(1) < 0)
				{
					printf("Warning: Unable to set VSync! SDL_Error: %s\n", SDL_GetError());
				}*/
				int display_index = 0, mode_index = 0;
				SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

				if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0) {
					SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
				}
				Timer::Instance().SetTimeLock(1000.f / (float)mode.refresh_rate);
				/*timer->SetTimeLock(1000.0f / (float)mode.refresh_rate);*/
			}
		}
	}

	renderer = new Render(sdl_window, SCREEN_WIDTH, SCREEN_HEIGHT);
	input = new Input();
	physics = new Physics();
	test_system = new TestSystem();

	Job * parent_job = new Job(bind_function(&Render::InitRenderComp, renderer), "Initialize_Render_Objects", current_scene->GetCompManager(), Job::RENDER_TYPE);
	TaskManager::Instance().RegisterJob(new Job(bind_function(&Render::Load, renderer), "Load_Render_System", sdl_context, Job::RENDER_TYPE), parent_job);
	TaskManager::Instance().RegisterJob(bind_function(&Physics::Load, physics), "Load_Physics_Sytem", current_scene);

	TaskManager::Instance().RegisterJob(parent_job, true);
	return true;
}

#endif // !_APPLICATION_H
