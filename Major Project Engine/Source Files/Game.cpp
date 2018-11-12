#include "Game.h"

Game::Game(const size_t & num_of_threads) : Application(num_of_threads) {}

Game::~Game() {}

bool Game::Load()
{
	Application::Load(std::make_unique<MainScene>());
	
	player_one = entity_manager->create_entity<Player>();
	player_one->Load(component_manager);


	_state = PLAYING;
	end_time = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> work_time = end_time - start_time;
	printf("Time ended: %f\n", std::chrono::duration<double, std::milli>(work_time.count()));
	return true;
}

bool Game::Game_Loop()
{
	start_time = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> work_time = start_time - end_time;

	if (work_time.count() < frame_rate)
	{
		std::chrono::duration<double, std::milli> delta_ms(frame_rate - work_time.count());
		auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
		std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
	}
	
	end_time = std::chrono::system_clock::now();
	std::chrono::duration<double, std::milli> sleep_time = end_time - start_time;

	//printf("Time: %f\n", (work_time + sleep_time).count());

	switch (_state)
	{
	case LOADING:
		this->Load();
		break;
	case PLAYING:

		input->Update(sdl_event,
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
	end_time = std::chrono::system_clock::now();
	printf("Time ended: %f\n", std::chrono::duration<double, std::milli>(end_time - start_time).count());
	game_running = false;
}


