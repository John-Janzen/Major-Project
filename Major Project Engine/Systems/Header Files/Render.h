#pragma once

#ifndef _RENDER_H
#define _RENDER_H

#include "System.h"
#include "FileLoader.h"
#include "RenderStorage.h"

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
	Render(SDL_Window * sdl_window, const int width, const int height);
	~Render();

	bool Load(void* content);
	void Close(void* content);

	void InitUpdate(CameraComponent * c_cp, const btTransform tran);
	bool UpdateLoop(void * ptr);
	void ComponentUpdate(GLfloat * project_value,
		RenderComponent * & rc,
		const btTransform transform);
	void FinalUpdate();

	bool init_render_component(void * ptr);

	bool LoadModel(void * ptr);

	bool LoadShader(void * ptr);

	bool LoadTexture(void * ptr);

	bool BindModel(void * ptr);

	bool BindTexture(void * ptr);

	bool BindShader(void * ptr);

	bool init_SDL(SDL_GLContext context);

	bool init_GL();

private:
	SDL_Window * sdl_window;

	GLfloat * project_value_ptr;

	int screen_width, screen_height;

	GLfloat Y_rotation = 0.0f;

	Storage<Model> * _models;
	Storage<Shader> * _shaders;
	Storage<Texture> * _textures;
};

#endif // !_RENDER_H
