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

	SDL_DestroyWindow(sdl_window);
	sdl_window = NULL;
	SDL_Quit();
}

bool Application::InitApp(const std::size_t & num_of_threads)
{
	Timer::Instance().Start();
	m_task = new TaskManager(num_of_threads);
	m_thread = new ThreadManager(m_task->GetJobList(), num_of_threads);

	renderer = new Render(*m_task, sdl_window, SCREEN_WIDTH, SCREEN_HEIGHT);
	input = new Input(*m_task);
	physics = new Physics(*m_task);
	test_system = new TestSystem(*m_task);
	_state = LOADING;
	return true;
}

bool Application::LoadApp()
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
				float time_lock = 1000.f / (float)mode.refresh_rate;
				Timer::Instance().SetTimeLock(time_lock);
				m_task->SetTimeLock(time_lock);
			}
		}
	}

	this->LoadScene(MAIN_SCENE);
	if (!(current_scene != nullptr && current_scene->Load()))
	{
		printf("Error Making current scene");
		return false;
	}
	
	Job * parent_job = new Job(bind_function(&Render::InitRenderComp, renderer), "Initialize_Render_Objects", current_scene->GetCompManager(), Job::JOB_RENDER_LOAD);
	m_task->RegisterJob(new Job(bind_function(&Render::Load, renderer), "Load_Render_System", sdl_context, Job::JOB_RENDER_LOAD), parent_job);

	
	m_task->RegisterJob(bind_function(&Physics::Load, physics), "Load_Physics_Sytem", current_scene);

	m_task->RegisterJob(parent_job, true);
	_state = PLAYING;

	return true;
}

bool Application::CloseApp()
{
	game_running = false;
	return false;
}

bool Application::GameLoop()
{
	Timer::Instance().WaitTime();

	switch (_state)
	{
	case INITIALIZING:
		this->InitApp(std::thread::hardware_concurrency() / 2);
		break;
	case LOADING:
		this->LoadApp();
		break;
	case PLAYING:
	{
		//timer->Start();
		input->Update(Timer::Instance().GetDeltaTime(), current_scene);

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
		m_task->RegisterJob(bind_function(&Physics::Update, physics), "Physics_Update", current_scene->GetCompManager(), Job::JOB_PHYSICS_UPDATE);

		m_task->RegisterJob(bind_function(&Render::UpdateLoop, renderer), "Render_Update", current_scene, Job::JOB_RENDER_UPDATE);
		//timer->Stop();
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

	while (Timer::Instance().CheckTimeLimit())
	{
		m_task->ManageJobs();
		m_thread->AllocateJobs();
	}

	return game_running;
}

bool Application::LoadScene(const SCENE_SELECTION type)
{
	switch (type)
	{
	case MAIN_SCENE:
		current_scene = new MainScene();
		break;
	default:
		break;
	}
	return true;
}
