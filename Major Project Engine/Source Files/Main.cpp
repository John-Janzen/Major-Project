#include "ThreadManager.h"

#include <SDL.h>

void standard_function()
{

}

int main(int argc, char * args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	ThreadManager::Instance().Init(std::thread::hardware_concurrency() / 2);
	bool GameOn = true;
	SDL_Event e;
	while (GameOn)
	{
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				GameOn = false;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		printf("Main Running...");
	}

	SDL_Quit();
	return 0;
}