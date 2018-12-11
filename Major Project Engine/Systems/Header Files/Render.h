#pragma once

#ifndef _RENDER_H
#define _RENDER_H

#include "System.h"
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

	JOB_RETURN Load(void* content);
	void Close(void* content);

	void InitUpdate(CameraComponent * c_cp, const btTransform tran);
	
	JOB_RETURN UpdateLoop(void * ptr);
	
	void ComponentUpdate(GLfloat * project_value, RenderComponent * & rc, const btTransform transform);

	void FinalUpdate();

	JOB_RETURN InitRenderComp(void * ptr);

	JOB_RETURN LoadModel(void * ptr);

	JOB_RETURN LoadShader(void * ptr);

	JOB_RETURN LoadTexture(void * ptr);

	JOB_RETURN BindModel(void * ptr);

	JOB_RETURN BindTexture(void * ptr);

	JOB_RETURN BindShader(void * ptr);

	bool InitSDL(SDL_GLContext context);

	bool InitGL();

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
