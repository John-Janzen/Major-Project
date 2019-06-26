/*===================================================================================*//**

Major Project Engine

A Prototype Engine made using the Thread-Pool design pattern for
multithreading. This will allow the game engine to utilize more of the
cpus within the hardware of the computer.

Copyright 2018 John Janzen. All rights reserved.
https://github.com/John-Janzen

@author John Janzen
@version 0.1.0
@file Main.cpp

*//*====================================================================================*/

#include "Application.h"

/*
Main entry point of the program.
Initializes and runs everything in the engine.
*/
int main(int argc, char * args[])
{
	int n_threads = std::thread::hardware_concurrency();		// Gets the number of cores on the computer running the program
	Application * game = new Application(n_threads - 1);
	
	while (game->RunApplication());			// loop through the applciation

	delete game;							// delete game when done
	return 0;
}