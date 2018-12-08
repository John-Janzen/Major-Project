#pragma once

#ifndef _RENDER_H
#define _RENDER_H

#include "System.h"

#include <SDL.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <GL\glew.h>
#include <iostream>
#include <gtc\matrix_transform.hpp>
#include <gtx\euler_angles.hpp>
#include <gtc\type_ptr.hpp>

class Render : public System
{
public:
	Render(SDL_Window * & sdl_window, const int width, const int height);
	~Render();

	bool Load(void* content);
	void Close(void* content);

	void InitUpdate(CameraComponent * & c_cp, const Transform * tran);
	void UpdateLoop(void * ptr);
	void ComponentUpdate(GLfloat * project_value,
		RenderComponent * & rc,
		const Transform * transform);
	void FinalUpdate();

	bool init_render_component(void * ptr);

	bool init_SDL(SDL_GLContext context);
	bool init_GL();

private:
	SDL_Window * sdl_window;

	GLfloat * project_value_ptr;

	int screen_width, screen_height;
};

#endif // !_RENDER_H
