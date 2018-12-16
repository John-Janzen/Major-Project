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

#include "Game.h"

/*
Main entry point of the program.
Initializes and runs everything in the engine.
*/
int main(int argc, char * args[])
{

	Application * game = new Game(std::thread::hardware_concurrency() / 2);
	
	while (game->GameLoop());

	delete(game);

	return 0;
}