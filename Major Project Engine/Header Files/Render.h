#pragma once
#include "System.h"
#include "Entity.h"
#include "RenderComponent.h"

#include <SDL.h>
#include <glew.h>
#include <iostream>

class Render : public System
{
public:
	Render(Game & parent_app);
	~Render();

	bool Load();
	void Update(const Entity & entity, const RenderComponent & rc);
	void FinalUpdate();
	void Close();

	bool init_SDL();
	bool init_GL();

private:
	SDL_Window * sdl_window;
	SDL_GLContext sdl_context;

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

};

