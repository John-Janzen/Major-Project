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

/*
The Render System uses both SDL and Opengl to render game object on the screen.
More complex rendering techniques could reduce the job time

Inherits System
*/
class Render : public System
{
public:
	Render(TaskManager & tm, SceneManager & sm);
	
	~Render();

	/*
	Initialize the system with a SDL_window that takes forever to open
	*/
	bool InitSystem(SDL_Window * window);

	/*
	Load the Renderer with the perspective and pass the opengl context over to the render thread
	For rendering on a different thread.
	*/
	bool Load();

	/*
	Close the Renderer
	*/
	void Close(void* content);

	/*
	Handle events as they come in
	*/
	void HandleEvent(const EventType & e, void * data);

	/*
	Update all render components and draw them on the screen
	*/
	JOB_RETURN Update(void * ptr);

private:

	/*
	Initialize the SDL components
	*/
	bool InitSDL();

	/*
	Initialize the OpenGL components
	*/
	bool InitGL();

	/*
	Give the OpenGL context to a seperate thread
	only one thread may have this context
	*/
	JOB_RETURN GiveThreadedContext(void * ptr);

	/*
	Clear color and depth bit and get camera perspective
	*/
	void InitUpdate();

	/*
	When a projectile is fired initialize the model, shader and texture in the Render System
	*/
	JOB_RETURN LoadSingleComponent(void * ptr);

	/*
	Load all the render components that are ready at the start of the program.
	This will load all the models, shaders, and textures.
	*/
	JOB_RETURN LoadComponents(void * ptr);

	/*
	Load the model file with the file loader
	*/
	JOB_RETURN ModelFileImport(void * ptr);

	/*
	Load the Shader file with the file loader
	*/
	JOB_RETURN ShaderFileImport(void * ptr);

	/*
	Load the Texuture file with the file loader
	*/
	JOB_RETURN TextureFileImport(void * ptr);

	/*
	Bind the model to the VBO and element arrays once they are done loading in
	*/
	JOB_RETURN BindModel(void * ptr);

	/*
	Bind the textures to the texture id and opengl textures
	*/
	JOB_RETURN BindTexture(void * ptr);

	/*
	bind the shader to the shader id and opengl glsl
	*/
	JOB_RETURN BindShader(void * ptr);

	/*
	Generate the Vertex Array Object based on the model of the render component
	*/
	void GenerateVAO(RenderComponent * const render);

private:

	glm::mat4 projection_look_matrix;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

	SDL_GLContext sdl_gl_context;

	SDL_Window * sdl_window;

	Storage<Model> _models;					// Storage for all models in game
	std::map<GLuint, std::vector<RenderComponent*>> model_listeners;		// this is for render components that share the same model, this was the only way i could think of binding the models
	Storage<Shader> _shaders;				// Storage for all shaders in game
	Storage<Texture> _textures;				// Storage for all textures in game

	int num_of_box_coll = 0;
	int num_of_bull_coll = 0;
};

#endif // !_RENDER_H
