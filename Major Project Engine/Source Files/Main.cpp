#include "ThreadManager.h"

#include <SDL.h>

void standard_function()
{

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
		/*std::this_thread::sleep_for(std::chrono::milliseconds(500));*/
		//printf("Main Running...");
	}
	SDL_Quit();
	return 0;
}