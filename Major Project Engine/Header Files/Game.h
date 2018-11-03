#pragma once
#include "Application.h"

class Game : public Application
{
public:
	explicit Game(const size_t & num_of_threads);
	~Game();

	bool Load();
	bool Game_Loop();
	void Close();

};

