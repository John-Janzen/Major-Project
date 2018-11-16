#include "Game.h"

Game::Game(const size_t & num_of_threads) : Application(num_of_threads) {}

Game::~Game() {}

bool Game::Load()
{
	Application::Load(std::make_unique<MainScene>());
	
	player_one = EntityManager::Instance().create_entity<Player>();
	player_one->Load();

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
		this->Load();
		break;
	case PLAYING:

		input->Update(timer->get_delta_time(), sdl_event,
			ComponentManager::Instance().get_component<PlayerControllerComponent>(player_one->get_id()),
			ComponentManager::Instance().get_component<Transform>(player_one->get_id()));

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
			ComponentManager::Instance().get_component<CameraComponent>(player_one->get_id()),
			ComponentManager::Instance().get_component<Transform>(player_one->get_id()));

		for (auto & element : EntityManager::Instance().retreive_list())
		{
			if (ComponentManager::Instance().get_component<RenderComponent>(element.first) != nullptr)
			{
				renderer->Update(
					ComponentManager::Instance().get_component<CameraComponent>(player_one->get_id())->get_project_value(),
					ComponentManager::Instance().get_component<RenderComponent>(element.first),
					ComponentManager::Instance().get_component<Transform>(element.first));
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
	ThreadManager::Instance().allocate_jobs();			// Allocate jobs to the threads

	/*if (std::chrono::duration<double, std::milli>(t_end - t_start).count() > 5000)
		_state = EXITING;*/
	
	return game_running;
}

void Game::Close()
{
	ThreadManager::Instance().print_total_jobs();			// Print the stats

	game_running = false;
}


