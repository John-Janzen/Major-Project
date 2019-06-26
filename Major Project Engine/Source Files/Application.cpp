#include "Application.h"

Application::Application(const std::size_t & num_of_threads)
	: _state(INITIALIZING),
	n_threads(num_of_threads),
	m_task(num_of_threads),
	m_thread(num_of_threads, m_task.GetJobList())
{	
	Timer::Instance().Init();

	/*
	Register for events that the class would like to listen for.
	There is probably a better way around it but for now this will work.
	*/
	{
		EventHandler::Instance().SubscribeEvent(OPEN_DEBUGGER, this);
		EventHandler::Instance().SubscribeEvent(GAME_CLOSED, this);
		EventHandler::Instance().SubscribeEvent(NEW_FRAME, this);
		EventHandler::Instance().SubscribeEvent(T_BUTTON_PRESSED, this);
		EventHandler::Instance().SubscribeEvent(DEBUG_FINISHED_LOAD, this);
		EventHandler::Instance().SubscribeEvent(LEFT_MOUSE_BUTTON, this);
		EventHandler::Instance().SubscribeEvent(PAUSED_BUTTON, this);
		EventHandler::Instance().SubscribeEvent(FULL_PAUSED_BUTTON, this);
	}
}

Application::~Application()
{
	for (auto system : _systems) // delete all systems
	{
		delete system;
		system = nullptr;
	}

	SDL_Quit();
}

void Application::HandleEvent(const EventType & e, void * data)
{
	switch (e)
	{
	case EventType::GAME_CLOSED:								// Event to Close the Game
		_state = EXITING;
		break;

	case EventType::OPEN_DEBUGGER:								// Event to Open the Debugger
		if (_state == DEBUG_LOAD || _state == DEBUG_RUN)
			this->ChangeGameState(GAME_STATE::DEBUG_CLOSE);		// Switch state to Close Debugger
		else
			this->ChangeGameState(GAME_STATE::DEBUG_LOAD);		// Switch state to Load Debugger
		break;

	case EventType::NEW_FRAME:									// Event to indicate Next frame has started
		can_start = true;
		start_frame.notify_one();
		break;

	case EventType::PAUSED_BUTTON:								// Event to Pause the game
		if (this->_state == GAME_STATE::PLAYING)
			this->ChangeGameState(GAME_STATE::PAUSED);
		else
			this->ChangeGameState(GAME_STATE::PLAYING);
		break;

	case EventType::FULL_PAUSED_BUTTON:							// Event to Full Pause Game
		if (this->_state == GAME_STATE::PLAYING)
		{
			this->ChangeGameState(GAME_STATE::FULL_PAUSE);
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
		else if (this->_state == GAME_STATE::FULL_PAUSE)
		{
			this->ChangeGameState(GAME_STATE::PLAYING);
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
		break;
	case EventType::T_BUTTON_PRESSED:							// Event to open debugger
		this->ChangeGameState(GAME_STATE::DEBUG_LOAD);
		break;

	case EventType::DEBUG_FINISHED_LOAD:						// Event to state that the debugger has finished loading
		this->ChangeGameState(GAME_STATE::DEBUG_RUN);
		break;

	case LEFT_MOUSE_BUTTON:		// Player Shoots
	{
		Entity * & project = m_scene.CreateEntity("Projectile", Entity::EntityType::BULLET);	// Spawn entity as projectile
		auto player = static_cast<BaseComponent*>(data);										// Player data provided through event
		auto transform = static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, player->_id))->_transform;	// Find transform of player
		m_scene.AddComponent(SceneManager::TRANSFORM, new Transform(project->_id, transform * btVector3(0.f, 2.f, -2.f)));	// Create Transform based on player transform

		auto c_render = m_scene.AddComponent(SceneManager::RENDER, new SphereRenderComponent(project->_id));	// Create Render Component for projectile
		EventHandler::Instance().SendEvent(EventType::RENDER_NEW_OBJECT, c_render);								// Send Render Component for loading in Render System

		auto physics = static_cast<PhysicsComponent*>(m_scene.AddComponent(SceneManager::PHYSICS, new SpherePhysicsComponent(project->_id)));		// Create Physics Component for Projectile
		btRigidBody::upcast(physics->coll_object)->setLinearVelocity(transform.getBasis() * btRigidBody::upcast(physics->coll_object)->getLinearVelocity());	// Set the velocity and change it by transform of player
		EventHandler::Instance().SendEvent(EventType::PHYSICS_NEW_OBJECT, physics);		// Send Physics Component for loading in Physics System
	}
		break;
	default:
		break;
	}
}

bool Application::RunApplication()
{
	SDL_Event sdl_event;
	switch (_state)
	{
	case INITIALIZING:
		if (!Initialized)				// If we haven't initialized yet
			this->InitApp();			// Initialize Application
		else
			if (!m_thread.HasJobs() && !m_task.HasJobs() && Initialized)	// Else if there are no jobs for this state
				this->ChangeGameState(GAME_STATE::LOADING);	// Change state to loading

		break;
	case LOADING:
		if (!LoadedApp)					// If we haven't loaded yet
			this->LoadApp();			// Load Application
		else
			if (!m_thread.HasJobs() && !m_task.HasJobs() && LoadedApp)	// Else if there are no jobs for this state
				this->ChangeGameState(GAME_STATE::PLAYING);	// Change State to playing
		break;
	case PAUSED:			// All three still read the SDL Window input
	case FULL_PAUSE:
	case PLAYING:
	{
		while (SDL_PollEvent(&sdl_event))
		{
			switch (sdl_event.type)
			{
			case SDL_QUIT:
				this->ChangeGameState(GAME_STATE::EXITING);
				break;
			case SDL_WINDOWEVENT:
				if (sdl_event.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					this->ChangeGameState(GAME_STATE::EXITING);
					break;
				}
				break;
			default:
				break;
			}
		}
	}
		break;
	case DEBUG_LOAD:
		SDL_SetRelativeMouseMode(SDL_FALSE);
		m_thread.ShowDebugger();				// Display the debugger
		break;

	case DEBUG_RUN:
	{
		while (SDL_PollEvent(&sdl_event))
		{
			if (sdl_event.window.windowID == m_thread.GetDebugWindowID())
			{
				switch (sdl_event.type)
				{
				case SDL_WINDOWEVENT:
					if (sdl_event.window.event == SDL_WINDOWEVENT_CLOSE)
					{
						this->ChangeGameState(GAME_STATE::DEBUG_CLOSE);
						break;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					m_thread.CheckDebugMouseLoc(sdl_event.motion);				// When player clicks check to see if mouse lands on item
					break;
				case SDL_MOUSEMOTION:
					{
						int x, y;
						Uint32 mouse_state = SDL_GetRelativeMouseState(&x, &y);
						break;
					}
				}
			}
		}
		m_thread.RenderDebugger();			// Render the debugger
		break;
	}
	case DEBUG_CLOSE:
		SDL_SetRelativeMouseMode(SDL_TRUE);
		m_thread.HideDebugger();			// Hide the window
		this->ChangeGameState(GAME_STATE::PLAYING);		// Switch state to playing
		break;
	case DELOAD:
		break;
	case EXITING:
		this->CloseApp();		// Close the game
		break;
	default:
		break;
	}

	if (_state == PLAYING || _state == DEBUG_RUN || _state == DEBUG_LOAD) // Stall the main thread so it matches the frame time limit
	{
		std::unique_lock<std::mutex> lock(start);
		start_frame.wait(lock, [this]
		{
			return this->can_start;
		});
		can_start = false;
	}

	return game_running;
}

bool Application::InitApp()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)			// Initialize SDL
	{
		printf("Issue Initing Everything %s\n", SDL_GetError());
		return false;
	}

	int display_index = 0, mode_index = 0;
	SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

	if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0)		// Get the display of the monitor
	{
		SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
		return false;
	}

	refresh_rate = 1000.f / (float)mode.refresh_rate;					// Calculate the refresh rate (default: 16.6667 milliseconds)

	Timer::Instance().SetTimeLock(refresh_rate);						// Load Timer Singleton with refresh rate
	m_task.SetTimeLock(refresh_rate);									// load Task Manager with refresh rate

	_systems[SYSTEM_TYPE::RENDER] = new Render(m_task, m_scene);		// Create Renderer
	_systems[SYSTEM_TYPE::PHYSICS] = new Physics(m_task, m_scene);		// Create Physics
	_systems[SYSTEM_TYPE::INPUT] = new Input(m_task, m_scene);			// Craete Input

	static_cast<Render*>(_systems[SYSTEM_TYPE::RENDER])->InitSystem(this->CreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT));		// Create window for renderer
	SDL_SetRelativeMouseMode(SDL_TRUE);									// Lock mouse to middle of window
	this->LoadScene(MAIN_SCENE);										// Load the Main Scene

	Initialized = true;

	return true;
}

bool Application::LoadApp()
{
	bool check = true;

	m_task.MainThreadJob(new Job(bind_function(&Application::GameLoop, this), "Application_Update", nullptr, job::JOB_APPLICATION_UPDATE));		// Start the new Main thread to distribute jobs
	m_thread.AllocateJobs(0);		// Allocate 0 jobs so it doesn't screw with the numbers

	m_thread.LoadDebugger(refresh_rate, n_threads);		// Load the debugger with the number of threads and refresh rate

	for (auto system : _systems)
		check &= system->Load();						// Load each system
	
	LoadedApp = true;
	return check;
}

bool Application::CloseApp()
{
	game_running = false;
	m_thread.Close();		// Close the thread manager
	m_task.Close();			// Close the task manager
	return false;
}

JOB_RETURN Application::GameLoop(void * ptr)
{	
	do				// Complete the loop at least once
	{
		if (!game_running) break;	// if game is not running STOP

		int num = m_task.ManageJobs();	// Task Manager take care of your jobs and transfer to thread manager
		m_thread.AllocateJobs(num);		// Thread Manager Distribute jobs to threads
	} while ((m_thread.HasJobs() || m_task.HasJobs()));		// As long as we have jobs continue 
															// Should include the timer check time limit, bugs unfortunatly

	m_task.MainThreadJob(new Job(bind_function(&Application::WaitTillNextFrame, this), "Wait_Time_For_Frame", nullptr, job::JOB_TILL_NEXT_FRAME));	// Load job to wait for next frame
	m_thread.AllocateJobs(1);	// Allocate 1 job for this game loop job

	return JOB_COMPLETED;
}

JOB_RETURN Application::WaitTillNextFrame(void * ptr)
{
	Timer::Instance().WaitTime();	// Timer wait until time is up

	EventHandler::Instance().SendEvent(EventType::NEW_FRAME);	// Alert all who are listening NEW FRAME TIME!!

	m_task.MainThreadJob(new Job(bind_function(&Application::GameLoop, this), "Application_Update", nullptr, job::JOB_APPLICATION_UPDATE));	// Go back to game loop to distribute jobs
	m_thread.AllocateJobs(1);	// Allocate 1 job for the waittillnextframe job

	return JOB_COMPLETED;
}

void Application::ChangeGameState(const GAME_STATE & gs)
{
	this->_state = gs;
	EventHandler::Instance().SendEvent(STATE_CHANGE, &_state);		// Alert game state has changed
}

bool Application::LoadScene(const SCENE_SELECTION type)
{
	switch (type)
	{
	case MAIN_SCENE:
		m_scene.LoadScene(new MainScene());			// Load the Main Scene, only scene available
		break;
	default:
		break;
	}
	return true;
}
