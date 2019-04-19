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

static const std::size_t DEFAULT_WIDTH = 1280;
static const std::size_t DEFAULT_HEIGHT = 720;

class Render : public System
{
public:
	Render(TaskManager & tm);
	~Render();

	bool Load(SceneManager * & sm);

	void Close(void* content);

	void Update(SceneManager * & sm);

	JOB_RETURN InitRenderComp(void * ptr);

private:

	void InitUpdate();

	JOB_RETURN ComponentUpdate(void * ptr);

	void FinalUpdate();

	bool InitSDL();

	bool InitGL();

	JOB_RETURN GiveThreadedContext(void * ptr);

	JOB_RETURN LoadModel(void * ptr);
	JOB_RETURN LoadShader(void * ptr);
	JOB_RETURN LoadTexture(void * ptr);

	JOB_RETURN BindModel(void * ptr);
	JOB_RETURN BindTexture(void * ptr);
	JOB_RETURN BindShader(void * ptr);

	glm::mat4 projection_look_matrix;
	glm::mat4 projection_matrix;
	glm::mat4 look_matrix;

	GLfloat _fov = 60.0f;
	GLfloat _near = 0.1f, _far = 1000.0f;
	std::size_t SCREEN_WIDTH = DEFAULT_WIDTH;
	std::size_t SCREEN_HEIGHT = DEFAULT_HEIGHT;

	SDL_GLContext sdl_gl_context;

	SDL_Window * sdl_window;

	GLfloat * project_value_ptr;

	int screen_width, screen_height;

	GLfloat Y_rotation = 0.0f;

	Storage<Model> * _models;
	Storage<Shader> * _shaders;
	Storage<Texture> * _textures;
};

#endif // !_RENDER_H
