#include "Application.h"

Application::Application(const std::size_t & num_of_threads)
	: _state(INITIALIZING),
	n_threads(num_of_threads),
	m_task(num_of_threads),
	m_thread(num_of_threads, m_task.GetJobList())
{	
	
	Timer::Instance().Init();
	h_event.SubscribeEvent(OPEN_DEBUGGER, this);
	h_event.SubscribeEvent(GAME_CLOSED, this);
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

bool Application::RunApplication()
{
	while ((m_thread.HasJobs() || m_task.HasJobs()))
	{
		int num = m_task.ManageJobs();
		m_thread.AllocateJobs(num);
	}

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
		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			m_task.RegisterJob(new Job(bind_function(&Input::WindowControls, input), "Window_Controls", new SDL_Event(e), Job::JOB_INPUT_DEFAULT), false);
		}
		m_task.RegisterJob(new Job(bind_function(&Application::GameLoop, this), "Application_Update", nullptr, Job::JOB_APPLICATION_UPDATE), false);
	}
		break;
	case DEBUG_LOAD:
		m_thread.ShowDebugger();
		_state = DEBUG_RUN;
		break;

	case DEBUG_RUN:
	{
		SDL_Event sdl_event;
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
				}
			}
		}
		m_thread.RenderDebugger();
		break;
	}
	case DEBUG_CLOSE:
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

	Timer::Instance().WaitTime();

	if (_state != DEBUG_LOAD && _state != DEBUG_CLOSE && _state != DEBUG_RUN)
		m_thread.NewFrame();

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

	renderer = new Render(m_task, m_scene, h_event);
	input = new Input(m_task, m_scene, h_event);
	physics = new Physics(m_task, m_scene, h_event);
	test_system = new TestSystem(m_task, m_scene, h_event);

	renderer->InitSystem(this->CreateWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT));

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

JOB_RETURN Application::GameLoop(void * ptr)
{
	//SDL_Event sdl_event;
	m_task.RegisterJob(new Job(bind_function(&Render::Update, renderer), "Render_Update", &m_scene.GetComponents(SceneManager::RENDER), Job::JOB_RENDER_UPDATE), true);
		
	m_task.RegisterJob(new Job(bind_function(&Physics::Update, physics), "Physics_Update", &m_scene.GetComponents(SceneManager::PHYSICS), Job::JOB_PHYSICS_UPDATE), false);

	m_task.RegisterJob(new Job(bind_function(&Input::Update, input), "Input_Update", &m_scene.GetComponents(SceneManager::CONTROLLER), Job::JOB_INPUT_UPDATE), false);

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
