#include "Game.h"

Game::Game(const size_t & num_of_threads) : Application(num_of_threads) {}

Game::~Game() {}

bool Game::Load()
{
	//Application::Load(std::make_unique<MainScene>());
	timer->Start();
	this->Load_Scene(MAIN_SCENE);
	this->Load_App();
	_state = LOADING;
	return true;
}

bool Game::Game_Loop()
{
	timer->wait_time();
	if (TaskManager::Instance().frame_start())
	{
		if (_state == LOADING)
		{
			_state = PLAYING;
			timer->Stop();
		}	
	}

	switch (_state)
	{
	case INITIALIZING:
		this->Load();
		break;
	case LOADING:
		break;
	case PLAYING:
	{
		input->Update(timer->get_delta_time(), current_scene);

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

		physics->Update();
		TaskManager::Instance().register_job(bind_function(&Render::UpdateLoop, renderer), "Render_Update", current_scene, RENDER_TYPE);
		//renderer->UpdateLoop(current_scene);
		break;
	}
	case EXITING:
		this->Close();
		break;
	default:
		break;
	}
	_threadpool->allocate_jobs();			// Allocate jobs to the threads
	return game_running;
}

void Game::Close()
{
	_threadpool->print_total_jobs();			// Print the stats
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
