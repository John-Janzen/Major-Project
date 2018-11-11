#include "Render.h"

Render::Render() 
	: System()
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
	projection_matrix = glm::perspective(glm::radians(60.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	look_matrix = glm::lookAtRH(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	return true;
}

void Render::InitUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	glm::mat4 model_matrix, rotation;
	rotation = glm::rotate(rotation, 0.0f, glm::vec3(0, 0, 1));
	rotation = glm::rotate(rotation, 0.0f, glm::vec3(1, 0, 0));
	rotation = glm::rotate(rotation, 0.0f, glm::vec3(0, 1, 0));
	model_matrix = glm::translate(rotation, glm::vec3(0.0f, 0.0f, -5.0f));

	projection_look_matrix = projection_matrix * (look_matrix * model_matrix);
}

void Render::Update(const std::shared_ptr<RenderComponent> & rc)
{
	if (rc->get_model() != nullptr)
	{
		glBindVertexArray(rc->get_vertex_array());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc->get_element_buffer());

		glUseProgram(rc->get_shader_program());

		glm::mat4 rotation = glm::mat4();
		rotation = glm::translate(rotation, glm::vec3(0.0f, 0.0f, 0.0f));
		rotation = glm::rotate(rotation, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		rotation = glm::rotate(rotation, 0.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 model_matrix = glm::rotate(rotation, test_rotate += 0.001, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(rc->get_render_proj_loc(), 1, GL_FALSE, glm::value_ptr(projection_look_matrix));
		glUniformMatrix4fv(rc->get_render_model_loc(), 1, GL_FALSE, glm::value_ptr(model_matrix));
		glDrawElements(GL_TRIANGLES, rc->get_model()->ISize, GL_UNSIGNED_INT, NULL);

		glBindVertexArray(0);
		glUseProgram(0);
	}
}

void Render::FinalUpdate()
{
	SDL_GL_SwapWindow(sdl_window);
}

void Render::Close()
{
	SDL_DestroyWindow(sdl_window);
	sdl_window = NULL;
	SDL_Quit();
}

void Render::init_render_component(std::shared_ptr<RenderComponent> & render_component)
{
	if (render_component->get_model() != nullptr)
	{
		glGenBuffers(1, &render_component->get_element_buffer());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_component->get_element_buffer());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			(sizeof(GLuint) * render_component->get_model()->ISize), 
			render_component->get_model()->getIndices(),
			GL_STATIC_DRAW);

		glGenVertexArrays(1, &render_component->get_vertex_array());
		glBindVertexArray(render_component->get_vertex_array());

		glGenBuffers(1, &render_component->get_vertex_buffer());
		glBindBuffer(GL_ARRAY_BUFFER, render_component->get_vertex_buffer());
		glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * render_component->get_model()->VSize), 
			render_component->get_model()->getVertices(), 
			GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(5 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	else
	{
		printf("Object skipped no available model\n");
	}
	if (render_component->get_v_shader() != nullptr && render_component->get_f_shader() != nullptr)
	{
		render_component->get_shader_program() = glCreateProgram();

		glAttachShader(render_component->get_shader_program(), render_component->get_v_shader()->getShaderID());
		glAttachShader(render_component->get_shader_program(), render_component->get_f_shader()->getShaderID());
		glLinkProgram(render_component->get_shader_program());

		GLint programSuccess = GL_FALSE;
		glGetProgramiv(render_component->get_shader_program(), GL_LINK_STATUS, &programSuccess);
		if (programSuccess != GL_TRUE)
		{
			printf("Error linking program %d!\n", render_component->get_shader_program());
		}
		else
		{
			glUseProgram(render_component->get_shader_program());

			render_component->get_render_model_loc() = glGetUniformLocation(render_component->get_shader_program(), "model_matrix");
			render_component->get_render_proj_loc() = glGetUniformLocation(render_component->get_shader_program(), "projection_matrix");
			
			render_component->get_render_color_loc() = glGetUniformLocation(render_component->get_shader_program(), "color_vec");
		}
	}
	else
	{
		printf("Object skipped no available shaders\n");
	}
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
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	return true;
}
