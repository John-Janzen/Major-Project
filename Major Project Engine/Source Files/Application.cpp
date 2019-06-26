#include "Application.h"

Application::Application(const std::size_t & num_of_threads)
	: _state(INITIALIZING),
	n_threads(num_of_threads),
	m_task(num_of_threads),
	m_thread(num_of_threads, m_task.GetJobList())
{	
	Timer::Instance().Init();
	EventHandler::Instance().SubscribeEvent(OPEN_DEBUGGER, this);
	EventHandler::Instance().SubscribeEvent(GAME_CLOSED, this);
	EventHandler::Instance().SubscribeEvent(NEW_FRAME, this);
	EventHandler::Instance().SubscribeEvent(T_BUTTON_PRESSED, this);
	EventHandler::Instance().SubscribeEvent(DEBUG_FINISHED_LOAD, this);
	EventHandler::Instance().SubscribeEvent(LEFT_MOUSE_BUTTON, this);
	EventHandler::Instance().SubscribeEvent(PAUSED_BUTTON, this);
	EventHandler::Instance().SubscribeEvent(FULL_PAUSED_BUTTON, this);
}

Application::~Application()
{
	for (auto system : _systems)
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
	case EventType::GAME_CLOSED:
		_state = EXITING;
		break;

	case EventType::OPEN_DEBUGGER:
		if (_state == DEBUG_LOAD || _state == DEBUG_RUN)
			this->ChangeGameState(GAME_STATE::DEBUG_CLOSE);
		else
			this->ChangeGameState(GAME_STATE::DEBUG_LOAD);
		break;

	case EventType::NEW_FRAME:
		can_start = true;
		start_frame.notify_one();
		break;

	case EventType::PAUSED_BUTTON:
		if (this->_state == GAME_STATE::PLAYING)
			this->ChangeGameState(GAME_STATE::PAUSED);
		else
			this->ChangeGameState(GAME_STATE::PLAYING);
		break;

	case EventType::FULL_PAUSED_BUTTON:
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
	case EventType::T_BUTTON_PRESSED:
		this->ChangeGameState(GAME_STATE::DEBUG_LOAD);
		break;

	case EventType::DEBUG_FINISHED_LOAD:
		this->ChangeGameState(GAME_STATE::DEBUG_RUN);
		break;

	case LEFT_MOUSE_BUTTON:
	{
		Entity * & project = m_scene.CreateEntity("Projectile", EntityType::BULLET);
		auto player = static_cast<BaseComponent*>(data);
		auto transform = static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, player->_id))->_transform;
		m_scene.AddComponent(SceneManager::TRANSFORM, new Transform(project->_id, transform * btVector3(0.f, 2.f, -2.f)));

		auto c_render = m_scene.AddComponent(SceneManager::RENDER, new SphereRenderComponent(project->_id));
		EventHandler::Instance().SendEvent(EventType::RENDER_NEW_OBJECT, c_render);

		auto physics = static_cast<PhysicsComponent*>(m_scene.AddComponent(SceneManager::PHYSICS, new SpherePhysicsComponent(project->_id)));
		btRigidBody::upcast(physics->coll_object)->setLinearVelocity(transform.getBasis() * btRigidBody::upcast(physics->coll_object)->getLinearVelocity());
		EventHandler::Instance().SendEvent(EventType::PHYSICS_NEW_OBJECT, physics);
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
		if (!Initialized)
			this->InitApp();
		else
			if (!m_thread.HasJobs() && !m_task.HasJobs())
				this->ChangeGameState(GAME_STATE::LOADING);

		break;
	case LOADING:
		if (!LoadedApp)
			this->LoadApp();
		else
			if (!m_thread.HasJobs() && !m_task.HasJobs())
				this->ChangeGameState(GAME_STATE::PLAYING);

		break;
	case PAUSED:
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
		m_thread.ShowDebugger();
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
					m_thread.CheckDebugMouseLoc(sdl_event.motion);
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
		m_thread.RenderDebugger();
		break;
	}
	case DEBUG_CLOSE:
		SDL_SetRelativeMouseMode(SDL_TRUE);
		m_thread.HideDebugger();
		this->ChangeGameState(GAME_STATE::PLAYING);
		break;
	case DELOAD:
		break;
	case EXITING:
		this->CloseApp();
		break;
	default:
		break;
	}

	if (_state == PLAYING || _state == DEBUG_RUN || _state == DEBUG_LOAD) 
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
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Issue Initing Everything %s\n", SDL_GetError());
		return false;
	}

	int display_index = 0, mode_index = 0;
	SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

	if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0)
	{
		SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
		return false;
	}

	refresh_rate = 1000.f / (float)mode.refresh_rate;

	Timer::Instance().SetTimeLock(refresh_rate);
	m_task.SetTimeLock(refresh_rate);

	_systems[SYSTEM_TYPE::RENDER] = new Render(m_task, m_scene);
	_systems[SYSTEM_TYPE::PHYSICS] = new Physics(m_task, m_scene);
	_systems[SYSTEM_TYPE::INPUT] = new Input(m_task, m_scene);

	static_cast<Render*>(_systems[SYSTEM_TYPE::RENDER])->InitSystem(this->CreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	SDL_SetRelativeMouseMode(SDL_TRUE);
	this->LoadScene(MAIN_SCENE);

	Initialized = true;

	return true;
}

bool Application::LoadApp()
{
	bool check = true;

	m_task.MainThreadJob(new Job(bind_function(&Application::GameLoop, this), "Application_Update", nullptr, job::JOB_APPLICATION_UPDATE));
	m_thread.AllocateJobs(0);

	m_thread.LoadDebugger(refresh_rate, n_threads);
	m_task.SetTimeLock(refresh_rate);

	for (auto system : _systems)
		check &= system->Load();
	
	LoadedApp = true;
	return check;
}

bool Application::CloseApp()
{
	game_running = false;
	m_thread.Close();
	m_task.Close();
	return false;
}

JOB_RETURN Application::GameLoop(void * ptr)
{	
	do 
	{
		if (!game_running) break;

		int num = m_task.ManageJobs();
		m_thread.AllocateJobs(num);
	} while ((m_thread.HasJobs() || m_task.HasJobs()));

	m_task.MainThreadJob(new Job(bind_function(&Application::WaitTillNextFrame, this), "Wait_Time_For_Frame", nullptr, job::JOB_TILL_NEXT_FRAME));
	m_thread.AllocateJobs(1);

	return JOB_COMPLETED;
}

JOB_RETURN Application::WaitTillNextFrame(void * ptr)
{
	Timer::Instance().WaitTime();

	EventHandler::Instance().SendEvent(EventType::NEW_FRAME);

	m_task.MainThreadJob(new Job(bind_function(&Application::GameLoop, this), "Application_Update", nullptr, job::JOB_APPLICATION_UPDATE));
	m_thread.AllocateJobs(1);

	return JOB_COMPLETED;
}

void Application::ChangeGameState(const GAME_STATE & gs)
{
	this->_state = gs;
	EventHandler::Instance().SendEvent(STATE_CHANGE, &_state);
}

bool Application::LoadScene(const SCENE_SELECTION type)
{
	switch (type)
	{
	case MAIN_SCENE:
		m_scene.LoadScene(new MainScene());
		break;
	default:
		break;
	}
	return true;
}
