#include "Render.h"

Render::Render(SDL_Window * window, const int width, const int height)
	: sdl_window(window), screen_width(width), screen_height(height)
{

}

Render::~Render() 
{
	std::cout << "Render Destructor called" << std::endl;
	sdl_window = nullptr;

	std::unordered_map<std::string, Model*>::iterator model_it = _models.begin();
	while (model_it != _models.end())
	{
		if ((*model_it).second != nullptr)
			delete (*model_it).second;
		model_it = _models.erase(model_it);
	}

	std::unordered_map<std::string, Shader*>::iterator shader_it = _shaders.begin();
	while (shader_it != _shaders.end())
	{
		if ((*shader_it).second != nullptr)
			delete (*shader_it).second;
		shader_it = _shaders.erase(shader_it);
	}
	
	std::unordered_map<std::string, Texture*>::iterator texture_it = _textures.begin();
	while (texture_it != _textures.end())
	{
		if ((*texture_it).second != nullptr)
			delete (*texture_it).second;
		texture_it = _textures.erase(texture_it);
	}
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

void Render::InitUpdate(CameraComponent * c_cp, const Transform * tran)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	glm::mat4 model_matrix, rotation;
	rotation = glm::rotate(rotation, tran->get_rot().z, glm::vec3(0, 0, 1));
	rotation = glm::rotate(rotation, tran->get_rot().x, glm::vec3(1, 0, 0));
	rotation = glm::rotate(rotation, (Y_rotation += 0.001f), glm::vec3(0, 1, 0));
	model_matrix = glm::translate(rotation, tran->get_pos());

	project_value_ptr = c_cp->set_project_look(model_matrix);
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
	const Transform * transform
)
{
	if (rc->get_model() != nullptr)
	{
		glBindVertexArray(rc->get_v_array());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc->get_e_buffer());

		glUseProgram(*rc->get_shader_prog());

		if (rc->get_texture() != nullptr && rc->get_texture()->TextureID != 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, rc->get_texture()->TextureID);
			glUniform4f(rc->get_color_loc(), rc->get_color().x, rc->get_color().y, rc->get_color().z, rc->get_color().w);
		}
		else
		{
			glm::vec4 _color = glm::vec4(1.0f, 0.411f, 0.705f, 1.0f);
			glUniform4f(rc->get_color_loc(), _color.x, _color.y, _color.z, _color.w);
		}

		glUniform1i(rc->r_text_unit, 0);
		glUniform4f(rc->r_text_color, 1.0f, 1.0f, 1.0f, 1.0f);

		glm::mat4 rotation = glm::mat4();
		rotation = glm::translate(rotation, transform->get_pos());
		rotation = glm::rotate(rotation, transform->get_rot().z, glm::vec3(0.0f, 0.0f, 1.0f));
		rotation = glm::rotate(rotation, transform->get_rot().x, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 model_matrix = glm::rotate(rotation, transform->get_rot().y, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(rc->get_proj_loc(), 1, GL_FALSE, project_value);
		glUniformMatrix4fv(rc->get_model_loc(), 1, GL_FALSE, glm::value_ptr(model_matrix));
		glDrawElements(GL_TRIANGLES, rc->get_model()->ISize, GL_UNSIGNED_INT, NULL);

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
	std::unordered_map<std::string, Model*>::iterator model_it;
	if ((model_it = _models.find(rc->getModelPath())) != _models.end())
	{
		rc->set_model(model_it->second);
	}
	else
	{
		Model * model = load_obj_file(rc->getModelPath());
		if (model != nullptr)
		{
			auto loc = _models.emplace(rc->getModelPath(), model);
			rc->set_model(model);
		}
	}
	return true;
}

bool Render::LoadShader(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	std::unordered_map<std::string, Shader*>::iterator shader_it;
	if ((shader_it = _shaders.find(std::string(rc->getVShaderPath() + rc->getFShaderPath()))) != _shaders.end())
	{
		rc->set_shader(shader_it->second);
	}
	else
	{
		Shader * shader = load_shader(rc->getVShaderPath(), rc->getFShaderPath());
		if (shader != nullptr)
		{
			auto loc = _shaders.emplace(std::string(rc->getVShaderPath() + rc->getFShaderPath()), shader);
			rc->set_shader(shader);
		}
	}
	return true;
}

bool Render::LoadTexture(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	std::unordered_map<std::string, Texture*>::iterator texture_it;
	if ((texture_it = _textures.find(rc->getTexturePath())) != _textures.end())
	{
		rc->set_texture(texture_it->second);
	}
	else
	{
		Texture * texture = load_texture(rc->getTexturePath());
		if (texture != nullptr)
		{
			auto loc = _textures.emplace(rc->getTexturePath(), texture);
			rc->set_texture(texture);
		}
	}
	return true;
}

bool Render::BindModel(void * ptr)
{
	RenderComponent * rc_cp = static_cast<RenderComponent*>(ptr);
	if (rc_cp->get_model() != nullptr)
	{
		glGenBuffers(1, &rc_cp->get_e_buffer());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc_cp->get_e_buffer());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			(sizeof(GLuint) * rc_cp->get_model()->ISize),
			rc_cp->get_model()->getIndices(),
			GL_STATIC_DRAW);

		glGenVertexArrays(1, &rc_cp->get_v_array());
		glBindVertexArray(rc_cp->get_v_array());

		glGenBuffers(1, &rc_cp->get_v_buffer());
		glBindBuffer(GL_ARRAY_BUFFER, rc_cp->get_v_buffer());
		glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * rc_cp->get_model()->VSize),
			rc_cp->get_model()->getVertices(),
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
	if (rc_cp->get_texture() != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &rc_cp->get_texture()->TextureID);
		glBindTexture(GL_TEXTURE_2D, rc_cp->get_texture()->TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
			rc_cp->get_texture()->texWidth,
			rc_cp->get_texture()->texWidth,
			0, GL_RGBA, GL_UNSIGNED_BYTE,
			rc_cp->get_texture()->get_data());

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
	if (rc_cp->get_shader() != nullptr)
	{
		const GLuint *program = rc_cp->set_shader_prog(glCreateProgram());

		glAttachShader(*program, rc_cp->get_shader()->getVertexShader());
		glAttachShader(*program, rc_cp->get_shader()->getFragmentShader());
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

			if (rc_cp->get_texture() != nullptr && rc_cp->get_texture()->TextureID != 0)
			{
				glUniform1i(glGetUniformLocation(*program, "tex_available"), 1);
				glUniform1f(rc_cp->r_text_adj_w, rc_cp->get_texture()->imgWidth / (GLfloat)rc_cp->get_texture()->texWidth);
				glUniform1f(rc_cp->r_text_adj_h, rc_cp->get_texture()->imgHeight / (GLfloat)rc_cp->get_texture()->texHeight);
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

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	return true;
}
