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
#include <SDL_ttf.h>

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

private:

	bool InitSDL();
	bool InitGL();

	JOB_RETURN GiveThreadedContext(void * ptr);

	void InitUpdate();

	JOB_RETURN LoadSingleComponent(void * ptr);
	JOB_RETURN LoadComponents(void * ptr);

	JOB_RETURN ModelFileImport(void * ptr);
	JOB_RETURN ShaderFileImport(void * ptr);
	JOB_RETURN TextureFileImport(void * ptr);

	JOB_RETURN BindModel(void * ptr);
	JOB_RETURN BindTexture(void * ptr);
	JOB_RETURN BindShader(void * ptr);

	void GenerateVAO(RenderComponent * const render);

private:

	glm::mat4 projection_look_matrix;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	SDL_GLContext sdl_gl_context;

	SDL_Window * sdl_window;

	Storage<Model> _models;
	std::map<GLuint, std::vector<RenderComponent*>> model_listeners;
	Storage<Shader> _shaders;
	Storage<Texture> _textures;

	int num_of_box_coll = 0;
	int num_of_bull_coll = 0;

	TTF_Font * sans_font;
};

#endif // !_RENDER_H
