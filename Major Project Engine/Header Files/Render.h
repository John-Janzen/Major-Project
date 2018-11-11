#pragma once

#ifndef _RENDER_H
#define _RENDER_H

#include "System.h"
#include "Entity.h"
#include "RenderComponent.h"

#include <SDL.h>
#include <glew.h>
#include <iostream>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
#include <gtc\type_ptr.hpp>

class Render : public System
{
public:
	Render();
	~Render();

	bool Load();
	void InitUpdate();
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

	GLfloat test_rotate = 0.0f;

	glm::mat4 projection_look_matrix;
	glm::mat4 projection_matrix;
	glm::mat4 look_matrix;
};

#endif // !_RENDER_H
