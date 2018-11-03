/*===================================================================================*//**

Major Project Engine

A Prototype Engine made using the Thread-Pool design pattern for
multithreading. This will allow the game engine to utilize more of the
cpus within the hardware of the computer.

Copyright 2018 John Janzen. All rights reserved.
https://github.com/John-Janzen

@author John Janzen
@version 0.0.02
@file Main.cpp

*//*====================================================================================*/

#include <SDL.h>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <iostream>

/*
* Main entry point of the program.
* Initializes and runs everything in the engine.
*/
int main(int argc, char * args[])
{
	//std::srand(std::time(NULL));
	std::clock_t c_start = std::clock();
	auto t_start = std::chrono::high_resolution_clock::now();
	SDL_Init(SDL_INIT_EVERYTHING);
	bool GameOn = true;
	SDL_Event e;
	int count = 0;
	
	SDL_Window * sdl_window = SDL_CreateWindow("Major Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

	ThreadManager::Instance().Init(std::thread::hardware_concurrency() / 2);		// Checks the computers for how many threads that it can have
																					// Based on haredware limitation
	std::chrono::time_point<std::chrono::steady_clock> t_end;
	do {
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
			for (int i = 0; i < 3; i++)
			{
				random_job_function(nullptr);
			}
		}
		ThreadManager::Instance().allocate_jobs();			// Allocate jobs to the threads
		t_end = std::chrono::high_resolution_clock::now();
	} while (GameOn && std::chrono::duration<double, std::milli>(t_end - t_start).count() < 5000);

	printf("Time ended: %f\n", std::chrono::duration<double, std::milli>(t_end - t_start).count());

	ThreadManager::Instance().Close();						// Close the Manager
	ThreadManager::Instance().print_total_jobs();			// Print the stats
	getchar();												// This is so I can see the stats when printed
	SDL_Quit();
	return 0;
}