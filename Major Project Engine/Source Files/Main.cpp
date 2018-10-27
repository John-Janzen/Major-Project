#include "ThreadManager.h"

#include <SDL.h>

void standard_function()
{

}

int main(int argc, char * args[])
{
	SDL_Init(SDL_INIT_EVERYTHING);
	ThreadManager::Instance().Init(std::thread::hardware_concurrency() / 2);

	SDL_Quit();
	return 0;
}