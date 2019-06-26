#pragma once

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "SceneManager.h"
#include "TaskManager.h"
#include "ThreadManager.h"

#include "Render.h"
#include "Input.h"
#include "Physics.h"

#include "Timer.h"

static const Uint16 DEFAULT_WIDTH = 1280;
static const Uint16 DEFAULT_HEIGHT = 720;

/*
The Application Class is the entry point of the program.
Everything comes and goes through this object.
It controls all the systems and managers.

Inherits Event Listener
*/
class Application : public EventListener
{
public:
	
	Application(const std::size_t & num_of_threads);

	~Application();

	/*
	This is the entry function where the main thread lives.
	It controls the sdl window events.
	*/
	bool RunApplication();

	/*
	Loads the specified scene into the scene manager.
	*/
	bool LoadScene(const SCENE_SELECTION type);
	
	/*
	Closes the Application.
	Which include the Thread Manager and Task Manager.
	*/
	bool CloseApp();

	/*
	Handles events that are posted to from the Event Handler.
	*/
	void HandleEvent(const EventType & e, void * data);

private:

	/*
	System types for when they are stored.
	It's to keep track of them when in abstract form.
	*/
	enum SYSTEM_TYPE
	{
		RENDER,
		PHYSICS,
		INPUT,
		COUNT
	};

	/*
	Initializes the application.
	Including SDL, Scene and Systems.
	*/
	bool InitApp();

	/*
	Loads all the systems and starts the first thread
	To loop through the job loop.
	*/
	bool LoadApp();
	
	/*
	This function distributes other jobs through the Task Manager and the Thread Manager.
	It goes until there are no more jobs available.
	Job Function
	*/
	JOB_RETURN GameLoop(void * ptr);

	/*
	This function has the Alternate Thread wait until time for the frame is up.
	Usually is 16.6667 milliseconds.
	Job Function
	*/
	JOB_RETURN WaitTillNextFrame(void * ptr);

	/*
	Changes the game state to the one passed in.
	*/
	void ChangeGameState(const GAME_STATE & gs);
	
	/*
	Encapsulated function for the SDL_CreateWindow Function.
	*/
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

private:

	/*
	Systems that are available in the application.
	Render, Physics, Input
	*/
	std::array<System*, SYSTEM_TYPE::COUNT> _systems = { nullptr };

	TaskManager m_task;
	ThreadManager m_thread;
	SceneManager m_scene;

protected:

	bool game_running = true;						// Flag for application still running
	GAME_STATE _state;								// State of the game (Loading, Playing, etc.)

	float refresh_rate = 1000.f / 60.f;				// Default Refresh Rate of game (16.6667 milliseconds)

	std::size_t n_threads;							// Number of threads

	std::condition_variable start_frame;			// Locks the main thread from listening to the SDL_Event too much
	std::mutex start;								// Provided mutex for condition variable
	bool can_start = false;							// Flag for the condition variable

	bool LoadedApp = false, Initialized = false;	// Flags for states of the game
};													// So we don't over initialize or load the game

#endif // !_APPLICATION_H
