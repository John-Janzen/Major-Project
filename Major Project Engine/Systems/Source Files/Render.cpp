#include "Render.h"

glm::mat4 getGLMMatrix4(const btScalar * matrix)
{
	return glm::mat4(matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15]);
}

Render::Render(SDL_Window * window, const int width, const int height)
	: sdl_window(window), screen_width(width), screen_height(height)
{
	_models = new Storage<Model>();
	_shaders = new Storage<Shader>();
	_textures = new Storage<Texture>();
}

Render::~Render() 
{
	std::cout << "Render Destructor called" << std::endl;
	sdl_window = nullptr;

	delete _models;
	delete _textures;
	delete _shaders;
}

bool Render::Load(void* content)
{
	if (!init_SDL(static_cast<SDL_GLContext>(content)))
	{
		printf("SDL Initialization failed, see function Load()");
	}
	if (!init_GL())
	{
		printf("GL Initialization failed, see function Load()");
	}
	return true;
}

void Render::InitUpdate(CameraComponent * c_cp, const btTransform tran)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	btScalar matrix[16];

	tran.getOpenGLMatrix(matrix);

	project_value_ptr = c_cp->set_project_look(getGLMMatrix4(matrix));
}

bool Render::UpdateLoop
(
	void * ptr
)
{
	Scene * current_scene = static_cast<Scene*>(ptr);
	this->InitUpdate(current_scene->get_comp_manager()->get_component<CameraComponent*>(current_scene->get_camera_id()),
		current_scene->get_ent_manager()->find_entity(current_scene->get_camera_id())->get_transform());

	RenderComponent * rc;
	for (auto & entity : current_scene->get_ent_manager()->retreive_list())
	{
		if ((rc = current_scene->get_comp_manager()->get_component<RenderComponent*>(entity.first)) != nullptr)
		{
			this->ComponentUpdate( project_value_ptr, rc, entity.second->get_transform());
		}
	}

	this->FinalUpdate();
	return true;
}

void Render::ComponentUpdate
(
	GLfloat * project_value,
	RenderComponent * & rc,
	const btTransform transform
)
{
	if (rc->GetModel() != nullptr)
	{
		glBindVertexArray(rc->get_v_array());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc->get_e_buffer());

		glUseProgram(*rc->get_shader_prog());

		if (rc->GetTexture() != nullptr && rc->GetTexture()->TextureID != 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, rc->GetTexture()->TextureID);
			glUniform4f(rc->get_color_loc(), rc->get_color().x, rc->get_color().y, rc->get_color().z, rc->get_color().w);
		}
		else
		{
			glm::vec4 _color = glm::vec4(1.0f, 0.411f, 0.705f, 1.0f);
			glUniform4f(rc->get_color_loc(), _color.x, _color.y, _color.z, _color.w);
		}

		glUniform1i(rc->r_text_unit, 0);
		glUniform4f(rc->r_text_color, 1.0f, 1.0f, 1.0f, 1.0f);

		btScalar matrix[16];
		transform.getOpenGLMatrix(matrix);

		glUniformMatrix4fv(rc->get_proj_loc(), 1, GL_FALSE, project_value);
		glUniformMatrix4fv(rc->get_model_loc(), 1, GL_FALSE, glm::value_ptr(getGLMMatrix4(matrix)));
		glDrawElements(GL_TRIANGLES, rc->GetModel()->ISize, GL_UNSIGNED_INT, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	}
}

void Render::FinalUpdate()
{
	SDL_GL_SwapWindow(sdl_window);
}

void Render::Close(void* content)
{
}

bool Render::init_render_component(void * ptr)
{
	ComponentManager * c_manager = static_cast<ComponentManager*>(ptr);

	for (auto & rc_cp : c_manager->find_all_of_type<RenderComponent*>())
	{
		// Loading and binding model jobs
		Job * bind_model_job = new Job(bind_function(&Render::BindModel, this), "Bind_Model", rc_cp, RENDER_TYPE);
		TaskManager::Instance().register_job(new Job(bind_function(&Render::LoadModel, this), "Load_Model", rc_cp), bind_model_job);
		TaskManager::Instance().register_job(bind_model_job, true);

		// Loading and binding shader jobs
		Job * bind_shader_job = new Job(bind_function(&Render::BindShader, this), "Bind_Shader", rc_cp, RENDER_TYPE);
		TaskManager::Instance().register_job(new Job(bind_function(&Render::LoadShader, this), "Load_Shader", rc_cp, RENDER_TYPE), bind_shader_job);
		TaskManager::Instance().register_job(bind_shader_job, true);

		// Loading and binding texture jobs
		Job * bind_texture_job = new Job(bind_function(&Render::BindTexture, this), "Bind_Texture", rc_cp, RENDER_TYPE);
 		TaskManager::Instance().register_job(new Job(bind_function(&Render::LoadTexture, this), "Load_Texture", rc_cp), bind_texture_job);
		TaskManager::Instance().register_job(bind_texture_job, true);
	}
	return true;
}

bool Render::LoadModel(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	if (!_models->HasItem(rc->getModelPath(), rc->GetModelAdd()))
	{
		LoadOBJModelFile(rc->getModelPath(), rc->GetModelAdd());
	}
	return true;
}

bool Render::LoadShader(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	if (!_shaders->HasItem(rc->getShaderPath(), rc->GetShaderAdd()))
	{
		LoadShaderFile(rc->getVShaderPath(), rc->getFShaderPath(), rc->GetShaderAdd());
	}
	return true;
}

bool Render::LoadTexture(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	if (!_textures->HasItem(rc->getTexturePath(), rc->GetTextureAdd()))
	{
		LoadTextureFile(rc->getTexturePath(), rc->GetTextureAdd());
	}
	return true;
}

bool Render::BindModel(void * ptr)
{
	RenderComponent * rc_cp = static_cast<RenderComponent*>(ptr);
	if (rc_cp->GetModel() != nullptr)
	{
		glGenBuffers(1, &rc_cp->get_e_buffer());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc_cp->get_e_buffer());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			(sizeof(GLuint) * rc_cp->GetModel()->ISize),
			rc_cp->GetModel()->_indices,
			GL_STATIC_DRAW);

		glGenVertexArrays(1, &rc_cp->get_v_array());
		glBindVertexArray(rc_cp->get_v_array());

		glGenBuffers(1, &rc_cp->get_v_buffer());
		glBindBuffer(GL_ARRAY_BUFFER, rc_cp->get_v_buffer());
		glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * rc_cp->GetModel()->VSize),
			rc_cp->GetModel()->_vertices,
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
	return true;
}

bool Render::BindTexture(void * ptr)
{
	RenderComponent * rc_cp = static_cast<RenderComponent*>(ptr);
	if (rc_cp->GetTexture() != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &rc_cp->GetTexture()->TextureID);
		glBindTexture(GL_TEXTURE_2D, rc_cp->GetTexture()->TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			rc_cp->GetTexture()->texWidth,
			rc_cp->GetTexture()->texWidth,
			0, GL_RGBA, GL_UNSIGNED_BYTE,
			rc_cp->GetTexture()->_texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		printf("Object skipped no available texture\n");
	}
	return true;
}

bool Render::BindShader(void * ptr)
{
	RenderComponent * rc_cp = static_cast<RenderComponent*>(ptr);
	if (rc_cp->GetShader() != nullptr)
	{
		const GLuint *program = rc_cp->set_shader_prog(glCreateProgram());

		glAttachShader(*program, rc_cp->GetShader()->_shaderID_Vert);
		glAttachShader(*program, rc_cp->GetShader()->_shaderID_Frag);
		glLinkProgram(*program);

		GLint programSuccess = GL_FALSE;
		glGetProgramiv(*program, GL_LINK_STATUS, &programSuccess);
		if (programSuccess != GL_TRUE)
		{
			printf("Error linking program %d!\n", glGetError());
		}
		else
		{
			glUseProgram(*program);

			rc_cp->set_model_loc(glGetUniformLocation(*program, "model_matrix"));
			rc_cp->set_proj_loc(glGetUniformLocation(*program, "projection_matrix"));
			rc_cp->set_color_loc(glGetUniformLocation(*program, "color_vec"));

			rc_cp->r_text_adj_w = glGetUniformLocation(*program, "texture_width_adj");
			rc_cp->r_text_adj_h = glGetUniformLocation(*program, "texture_height_adj");

			if (rc_cp->GetTexture() != nullptr && rc_cp->GetTexture()->TextureID != 0)
			{
				glUniform1i(glGetUniformLocation(*program, "tex_available"), 1);
				glUniform1f(rc_cp->r_text_adj_w, rc_cp->GetTexture()->imgWidth / (GLfloat)rc_cp->GetTexture()->texWidth);
				glUniform1f(rc_cp->r_text_adj_h, rc_cp->GetTexture()->imgHeight / (GLfloat)rc_cp->GetTexture()->texHeight);
			}
			else
			{
				glUniform1i(glGetUniformLocation(*program, "tex_available"), 0);
			}

			rc_cp->r_text_color = glGetUniformLocation(*program, "tex_color");
			rc_cp->r_text_unit = glGetUniformLocation(*program, "tex_unit");
		}
	}
	else
	{
		printf("Object skipped no available shaders\n");
	}
	return true;
}

bool Render::init_SDL(SDL_GLContext context)
{
	Uint32 subsystem_init = SDL_WasInit(SDL_INIT_VIDEO);
	if (!(subsystem_init & SDL_INIT_VIDEO))
	{
		printf("SDL INIT VIDEO failed! SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		SDL_GL_MakeCurrent(sdl_window, context);
	}
	return true;
}

bool Render::init_GL()
{
	glViewport(0, 0, screen_width, screen_height);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	return true;
}
