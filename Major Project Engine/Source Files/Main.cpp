#include "ThreadManager.h"

#include <SDL.h>
#include <cstdlib>
#include <iostream>
#include <ctime>

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
void standard_function(Content * ptr)
{
	InitialContent * IC = static_cast<InitialContent*>(ptr);
	int total = IC->num1 + IC->num2;
}

void sleepy_function(Content * ptr)
{
	InitialContent * IC = static_cast<InitialContent*>(ptr);
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	int total = IC->num1 + IC->num2;
}

void complex_function(Content * ptr)
{
	int total = 0;
	for (int i = 0; i < 100; i++)
	{
		total += i;
	}
	total /= 2;
}

void random_job_function(Content * ptr)
{
	int random_variable = 1 + std::rand() % 4;
	switch (random_variable)
	{
	case STANDARD_JOB:
		ThreadManager::Instance().register_job(
			std::make_unique<Job>(
				Job_Type::STANDARD_JOB, 
				&standard_function, 
				new InitialContent(2, random_variable)));
		break;
	case SLEEPY_JOB:
		ThreadManager::Instance().register_job(
			std::make_unique<Job>(
				Job_Type::SLEEPY_JOB,
				&sleepy_function,
				new InitialContent(2, random_variable)));
		break;
	case COMPLEX_JOB:
		ThreadManager::Instance().register_job(
			std::make_unique<Job>(
				Job_Type::COMPLEX_JOB,
				&complex_function));
		break;
	case RANDOM_JOB:
		ThreadManager::Instance().register_job(
			std::make_unique<Job>(
				Job_Type::RANDOM_JOB,
				&random_job_function));
		break;
	}
}

/*
* Main entry point of the program.
* Initializes and runs everything in the engine.
*/
int main(int argc, char * args[])
{
	//std::srand(std::time(NULL));
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
			for (int i = 0; i < 3; i++)
			{
				random_job_function(nullptr);
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