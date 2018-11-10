#include "Game.h"

Game::Game(const size_t & num_of_threads) : Application(num_of_threads) {}

Game::~Game() {}

bool Game::Load()
{
	c_start = std::clock();
	t_start = std::chrono::high_resolution_clock::now();

	renderer = std::make_unique<Render>(*this);
	if (!renderer.get()->Load())
	{
		printf("Error Initializing Renderer");
		_state = EXITING;
		return false;
	}
	current_scene = std::make_unique<MainScene>();
	current_scene->Load(entity_manager, component_manager);

	for (auto & element : component_manager->find_all_of_type<RenderComponent>())
	{
		renderer->init_render_component(element);
	}
	_state = PLAYING;
	t_end = std::chrono::high_resolution_clock::now();
	printf("Time ended: %f\n", std::chrono::duration<double, std::milli>(t_end - t_start).count());
	return true;
}

bool Game::Game_Loop()
{
	switch (_state)
	{
	case LOADING:
		this->Load();
		break;
	case PLAYING:
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
			}
		}

		for (auto & element : component_manager->find_all_of_type<RenderComponent>())
		{
			renderer->Update(element);
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
	t_end = std::chrono::high_resolution_clock::now();

	/*if (std::chrono::duration<double, std::milli>(t_end - t_start).count() > 5000)
		_state = EXITING;*/
	
	return game_running;
}

void Game::Close()
{
	thread_manager.get()->Close();						// Close the Manager
	renderer.get()->Close();
	thread_manager.get()->print_total_jobs();			// Print the stats
	printf("Time ended: %f\n", std::chrono::duration<double, std::milli>(t_end - t_start).count());
	game_running = false;
	getchar();
}


