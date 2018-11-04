#include "Render.h"

Render::Render(Game & parent_app) 
	: System(parent_app)
{

}

Render::~Render() 
{

}

bool Render::Load()
{
	if (!init_SDL())
	{
		printf("SDL Initialization failed, see function Load()");
		return false;
	}
	if (!init_GL())
	{
		printf("GL Initialization failed, see function Load()");
		return false;
	}
	return true;
}

void Render::Update()
{

}

void Render::Close()
{
	SDL_DestroyWindow(sdl_window);
	sdl_window = NULL;
	SDL_Quit();
}

bool Render::init_SDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL INIT VIDEO failed! SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		sdl_window = SDL_CreateWindow("Major Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
		if (sdl_window == NULL)
		{
			printf("Error creating window");
			return false;
		}
		else
		{
			sdl_context = SDL_GL_CreateContext(sdl_window);
			if (sdl_context == NULL)
			{
				printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
				return false;
			}
			else
			{
				glewExperimental = GL_TRUE;
				GLenum glewError = glewInit();
				if (glewError != GLEW_OK)
				{
					printf("Error initializing GLEW! %s", glewGetErrorString(glewError));
					return false;
				}
			}
		}
	}
	return true;
}

bool Render::init_GL()
{

	return true;
}
