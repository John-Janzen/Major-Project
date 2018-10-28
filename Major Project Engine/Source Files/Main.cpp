#include "ThreadManager.h"

#include <SDL.h>

void standard_function()
{
	//printf("Testing Function");
}

int main(int argc, char * args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	bool GameOn = true;
	SDL_Event e;
	SDL_Window * window;
	
	window = SDL_CreateWindow("Major Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		printf("Window could not be created");
		return 1;
	}

	ThreadManager::Instance().Init(std::thread::hardware_concurrency() / 2);

	while (GameOn)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				GameOn = false;
				break;
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_CLOSE)
				{
					GameOn = false;
					break;
				}
			}
		}
		if (!ThreadManager::Instance().jobs_full())
		{
			for (int i = 0; i < 5; i++)
			{
				ThreadManager::Instance().register_job(std::make_unique<Job>(Job_Type::NULL_TYPE, &standard_function));
			}
		}
		ThreadManager::Instance().allocate_jobs();
	}
	ThreadManager::Instance().Close();
	ThreadManager::Instance().print_total_jobs();
	getchar();
	SDL_Quit();
	return 0;
}