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

		input->Update(timer->get_delta_time(), sdl_event,
			current_scene->get_comp_manager()->get_component<PlayerControllerComponent>(_player1_ID),
			current_scene->get_comp_manager()->get_component<Transform>(_player1_ID));

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
			current_scene->get_comp_manager()->get_component<CameraComponent>(player_one->get_id()),
			current_scene->get_comp_manager()->get_component<Transform>(player_one->get_id()));

		for (auto & element : current_scene->get_ent_manager()->retreive_list())
		{
			if (current_scene->get_comp_manager()->get_component<RenderComponent>(element.first) != nullptr)
			{
				renderer->Update(
					current_scene->get_comp_manager()->get_component<CameraComponent>(player_one->get_id())->get_project_value(),
					current_scene->get_comp_manager()->get_component<RenderComponent>(element.first),
					current_scene->get_comp_manager()->get_component<Transform>(element.first));
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
	if (newScene == nullptr && !newScene->Load())
	{
		printf("Error Making current scene");
		return false;
	}
	else
	{
		current_scene = std::move(newScene);
		newScene.reset();
		newScene = nullptr;
	}

	for (auto & element : current_scene->get_comp_manager()->find_all_of_type<RenderComponent>())
	{
		renderer->init_render_component(element);
	}

	return true;
}
