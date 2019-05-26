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

	m_thread.MainThreadJob(new Job(bind_function(&Application::GameLoop, this), "Application_Update", nullptr, Job::JOB_APPLICATION_UPDATE));
	m_thread.AllocateJobs(0);
}

Application::~Application()
{
	m_task.Close();
	m_thread.Close();

	if (renderer != nullptr) delete renderer;
	if (input != nullptr) delete input;
	if (physics != nullptr) delete physics;
	if (test_system != nullptr) delete test_system;

	if (current_scene != nullptr) delete(current_scene);

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
		if (_state == DEBUG_LOAD && _state == DEBUG_RUN)
			_state = DEBUG_CLOSE;
		else
			_state = DEBUG_LOAD;
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
			if (!m_thread.HasJobs())
				_state = LOADING;

		break;
	case LOADING:
		if (!LoadedApp)
			this->LoadApp();
		else
			if (!m_thread.HasJobs())
				_state = PLAYING;

		break;
	case PLAYING:
	{
		while (SDL_PollEvent(&sdl_event))
		{
			switch (sdl_event.type)
			{
			case SDL_QUIT:
				//EventHandler::Instance().SendEvent(EventType::GAME_CLOSED);
				_state = EXITING;
				break;
			case SDL_WINDOWEVENT:
				if (sdl_event.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					//EventHandler::Instance().SendEvent(EventType::GAME_CLOSED);
					_state = EXITING;
					break;
				}
				break;
			case SDL_KEYDOWN:
				switch (sdl_event.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					if (SDL_GetRelativeMouseMode())
						SDL_SetRelativeMouseMode(SDL_FALSE);
					else
						SDL_SetRelativeMouseMode(SDL_TRUE);
					break;
				case SDL_SCANCODE_T:
					//EventHandler::Instance().SendEvent(EventType::OPEN_DEBUGGER);
					_state = DEBUG_LOAD;
					break;
				default:
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
		_state = DEBUG_RUN;
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
						_state = DEBUG_CLOSE;
						break;
					}
					break;
				case SDL_MOUSEBUTTONDOWN:
					m_thread.CheckDebugMouseLoc(sdl_event.motion);
					break;
				case SDL_KEYDOWN:
					{
						switch (sdl_event.key.keysym.scancode)
						{
						case SDL_SCANCODE_T:
							_state = DEBUG_CLOSE;
							break;
						default:
							break;
						}
					}
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
		_state = PLAYING;
		break;
	case PAUSED:
		break;
	case DELOAD:
		break;
	case EXITING:
		this->CloseApp();
		break;
	default:
		break;
	}

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

	renderer = new Render(m_task, m_scene);
	input = new Input(m_task, m_scene);
	physics = new Physics(m_task, m_scene);
	test_system = new TestSystem(m_task, m_scene);

	renderer->InitSystem(this->CreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT));
	SDL_SetRelativeMouseMode(SDL_TRUE);
	//auto s = SDL_GetDefaultCursor();
	//SDL_ShowCursor(0);
	this->LoadScene(MAIN_SCENE);

	Initialized = true;

	return true;
}

bool Application::LoadApp()
{
	//Timer::Instance().Start();
	bool check = true;

	check &= renderer->Load();
	check &= physics->Load();
	check &= input->Load();

	m_thread.LoadDebugger(refresh_rate, n_threads);
	
	LoadedApp = true;
	return check;
}

bool Application::CloseApp()
{
	game_running = false;
	return false;
}

JOB_RETURN Application::GameLoop(void * ptr)
{
	if (_state == PLAYING)
	{
		m_task.RegisterJob(new Job(bind_function(&Render::Update, renderer), "Render_Update", &m_scene.GetComponents(SceneManager::RENDER), Job::JOB_RENDER_UPDATE), true);

		m_task.RegisterJob(new Job(bind_function(&Physics::Update, physics), "Physics_Update", &m_scene.GetComponents(SceneManager::PHYSICS), Job::JOB_PHYSICS_UPDATE), true);

		m_task.RegisterJob(new Job(bind_function(&Input::Update, input), "Input_Update", &m_scene.GetComponents(SceneManager::CONTROLLER), Job::JOB_INPUT_UPDATE), false);
	}
	
	do 
	{
		int num = m_task.ManageJobs();
		m_thread.AllocateJobs(num);
	} while (Timer::Instance().CheckTimeLimit() && (m_thread.HasJobs() || m_task.HasJobs()));

	m_thread.MainThreadJob(new Job(bind_function(&Application::WaitTillNextFrame, this), "Wait_Time_For_Frame", nullptr, Job::JOB_TILL_NEXT_FRAME));
	m_thread.AllocateJobs(1);

	return JOB_COMPLETED;
}

JOB_RETURN Application::WaitTillNextFrame(void * ptr)
{
	Timer::Instance().WaitTime();
	can_start = true;
	start_frame.notify_one();

	if (_state != DEBUG_LOAD && _state != DEBUG_CLOSE && _state != DEBUG_RUN)
		m_thread.NewFrame();

	m_thread.MainThreadJob(new Job(bind_function(&Application::GameLoop, this), "Application_Update", nullptr, Job::JOB_APPLICATION_UPDATE));
	m_thread.AllocateJobs(1);

	return JOB_COMPLETED;
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
