#pragma once

#ifndef _RENDER_H
#define _RENDER_H

#include "System.h"

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
	void InitUpdate(const std::shared_ptr<CameraComponent> & c_cp, const std::shared_ptr<Transform> & tran);
	void UpdateLoop(const std::unique_ptr<Scene> & current_scene);
	void ComponentUpdate(GLfloat * project_value,
		const std::shared_ptr<RenderComponent> & rc,
		const std::shared_ptr<Transform> & transform);
	void FinalUpdate();
	void Close();

	void init_render_component(const std::unique_ptr<ComponentManager> & c_manager);

	bool init_SDL();
	bool init_GL();

private:
	SDL_Window * sdl_window;
	SDL_GLContext sdl_context;

	GLfloat * project_value_ptr;

	const int SCREEN_WIDTH = 1280;
	const int SCREEN_HEIGHT = 720;
};

#endif // !_RENDER_H
