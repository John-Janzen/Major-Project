#include "Application.h"

Application::Application()
	: _state(INITIALIZING)
{	
	Timer::Instance().Init();
}

Application::~Application()
{
	m_task->Close();
	m_thread->Close();

	if (renderer != nullptr) delete renderer;
	if (input != nullptr) delete input;
	if (physics != nullptr) delete physics;
	if (test_system != nullptr) delete test_system;

	if (current_scene != nullptr) delete(current_scene);

	if (m_thread != nullptr) delete m_thread;
	if (m_task != nullptr) delete m_task;
	if (m_scene != nullptr) delete m_scene;

	SDL_Quit();
}

bool Application::InitApp(const std::size_t & num_of_threads)
{
	Timer::Instance().Start();
	n_threads = num_of_threads;
	m_task = new TaskManager(num_of_threads);
	m_thread = new ThreadManager(m_task->GetJobList(), num_of_threads);
	m_scene = new SceneManager();

	renderer = new Render(*m_task, *m_scene);
	input = new Input(*m_task, *m_scene);
	physics = new Physics(*m_task, *m_scene);
	test_system = new TestSystem(*m_task, *m_scene);

	Initialized = true;
	return true;
}

bool Application::LoadApp()
{
	bool check = true;

	check &= this->LoadScene(MAIN_SCENE);

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("Issue Initing Everything %s\n", SDL_GetError());
		return false;
	}

	check &= renderer->Load();

	int display_index = 0, mode_index = 0;
	SDL_DisplayMode mode = { SDL_PIXELFORMAT_UNKNOWN, 0, 0, 0, 0 };

	if (SDL_GetDisplayMode(display_index, mode_index, &mode) != 0) 
	{
		SDL_Log("SDL_GetDisplayMode failed: %s", SDL_GetError());
	}

	float time_lock = 1000.f / (float)mode.refresh_rate;
	Timer::Instance().SetTimeLock(time_lock);
	m_task->SetTimeLock(time_lock);

	check &= physics->Load();
	check &= input->Load();
	LoadedApp = true;
	return check;
}

void Application::StartNewFrame()
{
	//Timer::Instance().Start();
	while (m_thread->HasJobs() || m_task->HasJobs())
	{
		int num = m_task->ManageJobs();
		m_thread->AllocateJobs(num);
	}
	//Timer::Instance().Stop();

	if (m_thread->jobs_to_finish == 0)
	{
		switch (_state)
		{
		case INITIALIZING:
			_state = LOADING;
			break;
		case LOADING:
			_state = PLAYING;
			break;
		case PLAYING:
			break;
		default:
			break;
		}
	}

	Timer::Instance().WaitTime();
}

bool Application::CloseApp()
{
	game_running = false;
	return false;
}

bool Application::GameLoop()
{
	switch (_state)
	{
	case INITIALIZING:
		if (!Initialized)
			game_running = this->InitApp(6);
		break;
	case LOADING:
		if (!LoadedApp)
			game_running = this->LoadApp();
		break;
	case PLAYING:
	{
		//
		//input->Update(m_scene);

		SDL_Event sdl_event;
		while (SDL_PollEvent(&sdl_event))			// Polls events for SDL (Mouse, Keyboard, window, etc.)
		{
			switch (sdl_event.type)
			{
			case SDL_QUIT:
				_state = EXITING;
				break;
			case SDL_WINDOWEVENT:
				if (sdl_event.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					_state = EXITING;
					break;
				}
				break;
			default:
				break;
			}
		}
		
		m_task->RegisterJob(new Job(bind_function(&Render::Update, renderer), "Render_Update", nullptr, Job::JOB_RENDER_UPDATE), true);
		
		m_task->RegisterJob(new Job(bind_function(&Physics::Update, physics), "Physics_Update", &m_scene->GetComponents(SceneManager::PHYSICS), Job::JOB_PHYSICS_UPDATE));

		break;
	}
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

	this->StartNewFrame();

	return game_running;
}

bool Application::LoadScene(const SCENE_SELECTION type)
{
	switch (type)
	{
	case MAIN_SCENE:
		m_scene->LoadScene(new MainScene());
		break;
	default:
		break;
	}
	return true;
}
