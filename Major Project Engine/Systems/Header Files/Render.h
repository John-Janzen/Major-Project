#pragma once

#ifndef _RENDER_H
#define _RENDER_H

#include "System.h"
#include "Entity.h"
#include "RenderComponent.h"

#include <SDL.h>
#include <GL/glew.h>
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
	void InitUpdate(const std::shared_ptr<CameraComponent>& camera, const std::shared_ptr<Transform>& transform);
	void Update(const GLfloat * project_value, const std::shared_ptr<RenderComponent>& rc, const std::shared_ptr<Transform>& transform);
	void FinalUpdate();
	void Close();

	void init_render_component(const std::shared_ptr<RenderComponent> & render_component);

	bool init_SDL();
	bool init_GL();

private:
	SDL_Window * sdl_window;
	SDL_GLContext sdl_context;

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
};

#endif // !_RENDER_H
