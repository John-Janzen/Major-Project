#include "Game.h"

Game::Game(const size_t & num_of_threads) : Application(num_of_threads) {}

Game::~Game() {}

bool Game::Load()
{
	//Application::Load(std::make_unique<MainScene>());
	Timer::Instance().Start();
	this->Load_Scene(MAIN_SCENE);
	this->LoadApplication();
	_state = LOADING;
	return true;
}

bool Game::GameLoop()
{
	Timer::Instance().WaitTime();
	if (TaskManager::Instance().FrameStart())
	{
		if (_state == LOADING)
		{
			_state = PLAYING;
			Timer::Instance().Stop();
		}
	}
	TaskManager::Instance().ThreadPoolAlloc();

	switch (_state)
	{
	case INITIALIZING:
		this->Load();
		break;
	case LOADING:
		break;
	case PLAYING:
	{
		//timer->Start();
		input->Update(Timer::Instance().GetDeltaTime(), current_scene);

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
		TaskManager::Instance().RegisterJob(bind_function(&Physics::Update, physics), "Physics_Update", current_scene->GetCompManager(), job::JOB_PHYSICS_UPDATE);

		TaskManager::Instance().RegisterJob(bind_function(&Render::UpdateLoop, renderer), "Render_Update", current_scene, job::JOB_RENDER_UPDATE);
		//timer->Stop();
		break;
	}
	case PAUSED:
		break;
	case DELOAD:
		break;
	case EXITING:
		this->Close();
		break;
	default:
		break;
	}
	while (TaskManager::Instance().HasJobs())
	{
		TaskManager::Instance().ManageJobs();

		if (Timer::Instance().CheckTimeLimit())
			break;
	}
	return game_running;
}

void Game::Close()
{
	game_running = false;
}

bool Game::Load_Scene(const SCENE_SELECTION type)
{
	switch (type)
	{
	case MAIN_SCENE:
		current_scene = new MainScene();
		break;
	default:
		break;
	}
	if (!(current_scene != nullptr && current_scene->Load()))
	{
		printf("Error Making current scene");
		return false;
	}
	return true;
}
