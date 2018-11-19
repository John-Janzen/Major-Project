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

	return true;
}

void Render::InitUpdate(const std::shared_ptr<CameraComponent> & c_cp, const std::shared_ptr<Transform> & tran)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	glm::mat4 model_matrix, rotation;
	rotation = glm::rotate(rotation, tran->get_rot().z, glm::vec3(0, 0, 1));
	rotation = glm::rotate(rotation, tran->get_rot().x, glm::vec3(1, 0, 0));
	rotation = glm::rotate(rotation, tran->get_rot().y, glm::vec3(0, 1, 0));
	model_matrix = glm::translate(rotation, tran->get_pos());

	project_value_ptr = c_cp->set_project_look(model_matrix);
}

void Render::UpdateLoop
(
	const std::unique_ptr<ComponentManager> & c_manager,
	const std::unique_ptr<EntityManager> & e_manager,
	EntityID _player
)
{
	this->InitUpdate(c_manager->get_component<CameraComponent>(_player), e_manager->find_entity(_player)->get_transform());

	for (auto & entity : e_manager->retreive_list())
	{
		if (c_manager->get_component<RenderComponent>(entity.first) != nullptr)
		{
			this->ComponentUpdate(
				project_value_ptr,
				c_manager->get_component<RenderComponent>(entity.first),
				entity.second->get_transform());
		}
	}

	this->FinalUpdate();
}

void Render::ComponentUpdate
(
	GLfloat * project_value,
	const std::shared_ptr<RenderComponent> & rc,
	const std::shared_ptr<Transform> & transform
)
{
	if (rc->get_model() != nullptr)
	{
		glBindVertexArray(rc->get_v_array());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc->get_e_buffer());

		glUseProgram(rc->get_shader_prog());

		glm::mat4 rotation = glm::mat4();
		rotation = glm::translate(rotation, transform->get_pos());
		rotation = glm::rotate(rotation, transform->get_rot().z, glm::vec3(0.0f, 0.0f, 1.0f));
		rotation = glm::rotate(rotation, transform->get_rot().x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 model_matrix = glm::rotate(rotation, transform->get_rot().y, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(rc->get_proj_loc(), 1, GL_FALSE, project_value);
		glUniformMatrix4fv(rc->get_model_loc(), 1, GL_FALSE, glm::value_ptr(model_matrix));
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

void Render::init_render_component(const std::unique_ptr<ComponentManager> & c_manager)
{
	for (auto & render_component : c_manager->find_all_of_type<RenderComponent>())
	{
		if (render_component->get_model() != nullptr)
		{
			glGenBuffers(1, &render_component->get_e_buffer());
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_component->get_e_buffer());
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				(sizeof(GLuint) * render_component->get_model()->ISize),
				render_component->get_model()->getIndices(),
				GL_STATIC_DRAW);

			glGenVertexArrays(1, &render_component->get_v_array());
			glBindVertexArray(render_component->get_v_array());

			glGenBuffers(1, &render_component->get_v_buffer());
			glBindBuffer(GL_ARRAY_BUFFER, render_component->get_v_buffer());
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
			render_component->set_shader_prog(glCreateProgram());

			glAttachShader(render_component->get_shader_prog(), render_component->get_v_shader()->getShaderID());
			glAttachShader(render_component->get_shader_prog(), render_component->get_f_shader()->getShaderID());
			glLinkProgram(render_component->get_shader_prog());

			GLint programSuccess = GL_FALSE;
			glGetProgramiv(render_component->get_shader_prog(), GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", render_component->get_shader_prog());
			}
			else
			{
				glUseProgram(render_component->get_shader_prog());

				render_component->set_model_loc(glGetUniformLocation(render_component->get_shader_prog(), "model_matrix"));
				render_component->set_proj_loc(glGetUniformLocation(render_component->get_shader_prog(), "projection_matrix"));
				render_component->set_color_loc(glGetUniformLocation(render_component->get_shader_prog(), "color_vec"));
			}
		}
		else
		{
			printf("Object skipped no available shaders\n");
		}
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
		sdl_window = SDL_CreateWindow("Major Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
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
