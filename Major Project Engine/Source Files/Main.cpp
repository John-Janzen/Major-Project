#include "ThreadManager.h"

#include <SDL.h>

/*===================================================================================*//**

Major Project Engine

A Prototype Engine made using the Thread-Pool design pattern for
multithreading. This should allow the game to scale with higher level cpu's.

Copyright 2018 John Janzen. All rights reserved.
https://github.com/John-Janzen

@author John Janzen
@version 0.0.02
@file Main.cpp

*//*====================================================================================*/

/*
* Standard testing function for Jobs
*/
void standard_function()
{
	//printf("Testing Function");
}

/*
* Main entry point of the program.
* Initializes and runs everything in the engine.
*/
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

	ThreadManager::Instance().Init(std::thread::hardware_concurrency() / 2);		// Checks the computers for how many threads that it can have
																					// Based on haredware limitation
	while (GameOn)
	{
		while (SDL_PollEvent(&e))			// Polls events for SDL (Mouse, Keyboard, window, etc.)
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
		if (!ThreadManager::Instance().jobs_full())			// As long as the list of jobs is not full 
		{													// Add more dummy jobs
			for (int i = 0; i < 5; i++)
			{
				ThreadManager::Instance().register_job(std::make_unique<Job>(Job_Type::NULL_TYPE, &standard_function));
			}
		}
		ThreadManager::Instance().allocate_jobs();			// Allocate jobs to the threads
	}
	ThreadManager::Instance().Close();						// Close the Manager
	ThreadManager::Instance().print_total_jobs();			// Print the stats
	getchar();												// This is so I can see the stats when printed
	SDL_Quit();
	return 0;
}