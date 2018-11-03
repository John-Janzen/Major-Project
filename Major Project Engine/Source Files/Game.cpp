#include "Game.h"

Game::Game(const size_t & num_of_threads) : Application(num_of_threads){}

Game::~Game() {}

bool Game::Load()
{
	c_start = std::clock();
	t_start = std::chrono::high_resolution_clock::now();

	return true;
}

bool Game::Game_Loop()
{
	while (SDL_PollEvent(&sdl_event))			// Polls events for SDL (Mouse, Keyboard, window, etc.)
	{
		switch (sdl_event.type)
		{
		case SDL_QUIT:
			game_running = false;
			break;
		case SDL_WINDOWEVENT:
			if (sdl_event.window.event == SDL_WINDOWEVENT_CLOSE)
			{
				game_running = false;
				break;
			}
		}
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

	if (std::chrono::duration<double, std::milli>(t_end - t_start).count() < 5000) 
		game_running = false;
	
	return game_running;
}

void Game::Close()
{
	thread_manager.get()->Close();						// Close the Manager
	thread_manager.get()->print_total_jobs();			// Print the stats
	printf("Time ended: %f\n", std::chrono::duration<double, std::milli>(t_end - t_start).count());
	getchar();
}


