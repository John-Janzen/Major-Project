#pragma once

#ifndef _GAME_H
#define _GAME_H

#include "Application.h"
#include "GameHeaders.h"

class Game : public Application
{
public:
	explicit Game(const size_t & num_of_threads);
	~Game();

	bool Load();
	bool Game_Loop();
	void Close();
};

#endif // !_GAME_H
