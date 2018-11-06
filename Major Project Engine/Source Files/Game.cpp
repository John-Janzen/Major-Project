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

	int entity_id1 = entity_manager->create_entity_id("256");
	int entity_id2 = entity_manager->create_entity_id("257");
	component_manager->add_component(entity_id1, std::make_shared<RenderComponent>());
	component_manager->add_component(entity_id2, std::make_shared<RenderComponent>());

	FileLoader::instance().ObjImporter(std::string("Assets/Quad.obj"), component_manager->get_component<RenderComponent>(entity_id1)->get_model());

	_state = PLAYING;
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
		for (auto it = entity_manager->retreive_list().begin(); it != entity_manager->retreive_list().end(); ++it)
		{
			std::shared_ptr<RenderComponent> rc;
			if (component_manager->get_component((*it).first, rc))
			{
				renderer->Update(*(*it).second, rc);
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


