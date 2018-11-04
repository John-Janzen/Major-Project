#pragma once
#include "System.h"

#include <SDL.h>
#include <glew.h>
#include <iostream>

class Render : public System
{
public:
	Render(Game & parent_app);
	~Render();

	bool Load();
	void Update();
	void Close();

	bool init_SDL();
	bool init_GL();

private:
	SDL_Window * sdl_window;
	SDL_GLContext sdl_context;

};

