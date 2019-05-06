#pragma once

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "SceneManager.h"
#include "TaskManager.h"
#include "ThreadManager.h"
#include "ThreadDebugger.h"

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

	bool GameLoop();

	bool LoadScene(const SCENE_SELECTION type);
	
	bool CloseApp();

private:

	bool InitApp(const std::size_t & num_of_threads);

	bool LoadApp();

	void StartNewFrame();

	Render * renderer = nullptr;
	Input * input = nullptr;
	TestSystem * test_system = nullptr;
	Physics * physics = nullptr;

	ThreadManager * m_thread = nullptr;
	TaskManager * m_task = nullptr;
	SceneManager * m_scene = nullptr;

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

	bool LoadedApp = false, Initialized = false;
};

#endif // !_APPLICATION_H
