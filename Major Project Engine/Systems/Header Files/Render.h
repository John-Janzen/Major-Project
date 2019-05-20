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
#include <LinearMath\btTransform.h>

class Render : public System
{
public:
	Render(TaskManager & tm, SceneManager & sm);
	
	~Render();

	bool InitSystem(SDL_Window * window);

	bool Load();

	void Close(void* content);

	void HandleEvent(const EventType & e, void * data);

	JOB_RETURN Update(void * ptr);



	void SwapBuffers() { SDL_GL_SwapWindow(sdl_window); }

private:

	void InitUpdate();

	void ComponentUpdate(RenderComponent * rc, const Transform & trans);

	JOB_RETURN LoadSingleComponent(void * ptr);

	JOB_RETURN LoadComponents(void * ptr);

	bool InitSDL();

	bool InitGL();

	JOB_RETURN GiveThreadedContext(void * ptr);

	JOB_RETURN ModelFileImport(void * ptr);
	JOB_RETURN ShaderFileImport(void * ptr);
	JOB_RETURN TextureFileImport(void * ptr);

	JOB_RETURN BindModel(void * ptr);
	void GenerateVAO(RenderComponent * const render);
	JOB_RETURN BindTexture(void * ptr);
	JOB_RETURN BindShader(void * ptr);

	glm::mat4 projection_look_matrix;
	glm::mat4 projection_matrix;
	glm::mat4 look_matrix;

	GLfloat _fov = 60.0f;
	GLfloat _near = 0.1f, _far = 1000.0f;
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

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
