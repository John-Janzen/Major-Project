#include "Game.h"

Game::Game(const size_t & num_of_threads) : Application(num_of_threads) {}

Game::~Game() {}

bool Game::Load()
{
	Application::Load(std::make_unique<MainScene>());
	
	player_one = entity_manager->create_entity<Player>();
	player_one->Load(component_manager);

	_timer->End();
	_state = PLAYING;
	return true;
}

bool Game::Game_Loop()
{
	_timer->wait_time();

	switch (_state)
	{
	case LOADING:
		this->Load();
		break;
	case PLAYING:

		input->Update(_timer->get_delta_time(), sdl_event,
			component_manager->get_component<PlayerControllerComponent>(player_one->get_id()),
			component_manager->get_component<Transform>(player_one->get_id()));

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

		renderer->InitUpdate(
			component_manager->get_component<CameraComponent>(player_one->get_id()), 
			component_manager->get_component<Transform>(player_one->get_id()));

		for (auto & element : entity_manager->retreive_list())
		{
			if (component_manager->get_component<RenderComponent>(element.first) != nullptr)
			{
				renderer->Update(
					component_manager->get_component<CameraComponent>(player_one->get_id())->get_project_value(),
					component_manager->get_component<RenderComponent>(element.first),
					component_manager->get_component<Transform>(element.first));
			}
		}
		renderer->FinalUpdate();

		break;
	case EXITING:
		this->Close();
		break;
	default:
		break;
	}
	
	//if (!thread_manager.get()->jobs_full())			// As long as the list of jobs is not full 
	//{													// Add more dummy jobs
	//	for (int i = 0; i < 3; i++)
	//	{
	//		random_job_function(nullptr);
	//	}
	//}
	//ThreadManager::Instance().allocate_jobs();			// Allocate jobs to the threads

	/*if (std::chrono::duration<double, std::milli>(t_end - t_start).count() > 5000)
		_state = EXITING;*/
	
	return game_running;
}

void Game::Close()
{
	thread_manager.get()->Close();						// Close the Manager
	renderer.get()->Close();
	thread_manager.get()->print_total_jobs();			// Print the stats
	_timer->End();
	game_running = false;
}


