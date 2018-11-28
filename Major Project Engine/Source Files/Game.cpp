#include "Game.h"

Game::Game(const size_t & num_of_threads) : Application(num_of_threads) {}

Game::~Game() {}

bool Game::Load()
{
	//Application::Load(std::make_unique<MainScene>());
	this->Load_Scene(MAIN_SCENE);
	
	timer->End();
	_state = PLAYING;
	return true;
}

bool Game::Game_Loop()
{
	timer->wait_time();
	switch (_state)
	{
	case LOADING:
		this->Load_App();
		this->Load();
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

		renderer->UpdateLoop(current_scene);
		//std::this_thread::sleep_for(std::chrono::milliseconds(2));
		break;
	}
	case EXITING:
		this->Close();
		break;
	default:
		break;
	}
	ThreadManager::Instance().allocate_jobs();			// Allocate jobs to the threads

	return game_running;
}

void Game::Close()
{
	ThreadManager::Instance().print_total_jobs();			// Print the stats

	game_running = false;
}

bool Game::Load_Scene(const SCENE_SELECTION & type)
{
	std::unique_ptr<Scene> newScene;
	switch (type)
	{
	case MAIN_SCENE:
		newScene = std::make_unique<MainScene>();
		break;
	default:
		break;
	}
	if (newScene != nullptr && newScene->Load())
	{
		current_scene = std::move(newScene);
		newScene.reset();
		newScene = nullptr;
	}
	else
	{
		printf("Error Making current scene");
		return false;
	}

	renderer->init_render_component(current_scene->get_comp_manager());

	return true;
}
