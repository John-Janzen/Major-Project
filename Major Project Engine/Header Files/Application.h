#pragma once

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "SceneManager.h"
#include "TaskManager.h"
#include "ThreadManager.h"

#include "Render.h"
#include "Input.h"
#include "Physics.h"
#include "TestSystem.h"

#include "Timer.h"

#include <vector>
#include <ctime>
#include <chrono>
#include <condition_variable>

static const Uint16 DEFAULT_WIDTH = 1280;
static const Uint16 DEFAULT_HEIGHT = 720;

class Application : public EventListener
{
public:
	Application(const std::size_t & num_of_threads);
	~Application();

	bool RunApplication();

	bool LoadScene(const SCENE_SELECTION type);
	
	bool CloseApp();

	void HandleEvent(const EventType & e, void * data);

private:

	bool InitApp();

	bool LoadApp();

	JOB_RETURN GameLoop(void * ptr);

	JOB_RETURN WaitTillNextFrame(void * ptr);

	SDL_Window * CreateWindow(const std::size_t & width, const std::size_t & height)
	{
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

		return SDL_CreateWindow("Major Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
	}

	Render * renderer = nullptr;
	Input * input = nullptr;
	TestSystem * test_system = nullptr;
	Physics * physics = nullptr;

	TaskManager m_task;
	ThreadManager m_thread;
	SceneManager m_scene;

protected:

	enum GAME_STATE
	{
		NULL_STATE,
		INITIALIZING,
		LOADING,
		PLAYING,
		PAUSED,
		DELOAD,
		DEBUG_LOAD,
		DEBUG_RUN,
		DEBUG_CLOSE,
		EXITING
	};

	bool game_running = true;
	GAME_STATE _state;

	float refresh_rate = 1000 / 60;

	Scene * current_scene = nullptr;

	GLfloat frame_rate;
	std::size_t n_threads;

	std::unique_ptr<std::thread> main_thread;
	std::condition_variable start_frame;
	std::mutex start;
	bool can_start = false;

	bool LoadedApp = false, Initialized = false;
};

#endif // !_APPLICATION_H
