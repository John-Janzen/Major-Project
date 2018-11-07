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
	void Update(const std::shared_ptr<RenderComponent> & rc);
	void FinalUpdate();
	void Close();

	void init_render_component(std::shared_ptr<RenderComponent> & render_component);

	bool init_SDL();
	bool init_GL();

private:
	SDL_Window * sdl_window;
	SDL_GLContext sdl_context;

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;

};

