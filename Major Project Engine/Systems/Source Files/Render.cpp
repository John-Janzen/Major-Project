#include "Render.h"
#include "FileLoader.h"

glm::mat4 getGLMMatrix4(const btScalar * matrix)
{
	return glm::mat4(matrix[0], matrix[1], matrix[2], matrix[3],
		matrix[4], matrix[5], matrix[6], matrix[7],
		matrix[8], matrix[9], matrix[10], matrix[11],
		matrix[12], matrix[13], matrix[14], matrix[15]);
}

Render::Render(TaskManager & tm, SceneManager & sm)
	: System(tm, sm)
{
	_models = new Storage<Model>();
	_shaders = new Storage<Shader>();
	_textures = new Storage<Texture>();

	EventHandler::Instance().SubscribeEvent(EventType::RENDER_NEW_OBJECT, this);

	// Jobs that need to wait on other jobs go here
	{
		m_task.dictionary.emplace(Job::JOB_RENDER_LOAD_SINGLE, std::vector<Job::JOB_ID>());
		m_task.dictionary[Job::JOB_RENDER_LOAD_SINGLE].emplace_back(Job::JOB_RENDER_UPDATE);

		m_task.dictionary.emplace(Job::JOB_LOAD_MODEL, std::vector<Job::JOB_ID>());
		m_task.dictionary[Job::JOB_LOAD_MODEL].emplace_back(Job::JOB_RENDER_UPDATE);

		m_task.dictionary.emplace(Job::JOB_LOAD_TEXTURE, std::vector<Job::JOB_ID>());
		m_task.dictionary[Job::JOB_LOAD_TEXTURE].emplace_back(Job::JOB_RENDER_UPDATE);

		m_task.dictionary.emplace(Job::JOB_LOAD_SHADER, std::vector<Job::JOB_ID>());
		m_task.dictionary[Job::JOB_LOAD_SHADER].emplace_back(Job::JOB_RENDER_UPDATE);
	}
}

bool Render::InitSystem(SDL_Window * window)
{
	if (window == NULL)
	{
		printf("Error creating window");
		return false;
	}

	sdl_window = window;
	SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	if (!InitSDL())
	{
		printf("SDL Initialization failed, see function Load()");
		return false;
	}
	if (!InitGL())
	{
		printf("GL Initialization failed, see function Load()");
		return false;
	}
	return true;
}

Render::~Render() 
{
	std::cout << "Render Destructor called" << std::endl;
	sdl_window = nullptr;

	delete _models;
	delete _textures;
	delete _shaders;

	SDL_DestroyWindow(sdl_window);
}

bool Render::Load()
{
	/*projection_matrix = glm::perspective(glm::radians(_fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, _near, _far);
	look_matrix = glm::lookAtRH(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));*/
	
	// Initialize Camera
	{
		for (auto camera : m_scene.GetComponents(SceneManager::CAMERA))
		{
			auto obj = static_cast<CameraComponent*>(camera);
			{
				obj->projection = glm::perspective(glm::radians(obj->_fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, obj->_near, obj->_far);
			}
		}
	}

	m_task.RegisterJob(new Job(bind_function(&Render::LoadComponents, this), "Render_Component_Loader", &m_scene.GetComponents(SceneManager::RENDER), Job::JOB_RENDER_LOAD));
	return true;
}

void Render::Close(void* content) {}

void Render::HandleEvent(const EventType & e, void * data)
{
	switch (e)
	{
	case EventType::RENDER_NEW_OBJECT:
		m_task.RegisterJob(new Job(bind_function(&Render::LoadSingleComponent, this), "Load_Single_Render", data, Job::JOB_RENDER_LOAD_SINGLE), false);
		break;
	default:
		break;
	}
}

void Render::InitUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	for (auto camera : m_scene.GetComponents(SceneManager::CAMERA))
	{
		auto obj = static_cast<CameraComponent*>(camera);
		{
			glm::mat4 look_matrix;
			auto transform = static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, camera->_id))->_transform;
			{
				auto position = transform * obj->_eye;
				auto direction = position + obj->_aim;

				look_matrix = glm::lookAtRH(
					glm::vec3(position.x(), position.y(), position.z()),
					glm::vec3(direction.x(), direction.y(), direction.z()),
					glm::vec3(obj->_up.x(), obj->_up.y(), obj->_up.z()));
			}
			projection_look_matrix = obj->projection * look_matrix;
		}
	}
}

void Render::ComponentUpdate(RenderComponent * rc, const Transform & trans)
{
	Model * model_ptr = rc->GetModel();
	Shader * shader_ptr = rc->GetShader();
	Texture * texture_ptr = rc->GetTexture();
	if (model_ptr != nullptr && shader_ptr != nullptr && texture_ptr != nullptr)
	{
		glBindVertexArray(rc->GetVertexArray());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_ptr->elem_buff_obj);

		glUseProgram(shader_ptr->shade_prog);

		if (rc->GetTexture() != nullptr && rc->GetTexture()->TextureID != 0)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, rc->GetTexture()->TextureID);
			glUniform4f(shader_ptr->r_color_vec4_loc, rc->GetColor().x, rc->GetColor().y, rc->GetColor().z, rc->GetColor().w);
		}
		else
		{
			glm::vec4 _color = glm::vec4(1.0f, 0.411f, 0.705f, 1.0f);
			glUniform4f(shader_ptr->r_color_vec4_loc, _color.x, _color.y, _color.z, _color.w);
		}

		glUniform1i(shader_ptr->r_text_unit, 0);
		glUniform4f(shader_ptr->r_text_color, 1.0f, 1.0f, 1.0f, 1.0f);

		btScalar matrix[16];
		trans._transform.getOpenGLMatrix(matrix);

		glUniformMatrix4fv(shader_ptr->r_project_mat4_loc, 1, GL_FALSE, glm::value_ptr(projection_look_matrix));
		glUniformMatrix4fv(shader_ptr->r_model_mat4_loc, 1, GL_FALSE, glm::value_ptr(getGLMMatrix4(matrix)));
		glDrawElements(GL_TRIANGLES, model_ptr->ISize, GL_UNSIGNED_INT, NULL);

		glBindTexture(GL_TEXTURE_2D, 0);

		glBindVertexArray(0);
		glUseProgram(0);
	}
}

JOB_RETURN Render::Update(void * ptr)
{
	this->InitUpdate();
	for (auto & it : *static_cast<std::vector<BaseComponent*>*>(ptr))
	{
		assert(dynamic_cast<RenderComponent*>(it));
		{
			this->ComponentUpdate(static_cast<RenderComponent*>(it), *static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, it->_id)));
		}
	}
	this->SwapBuffers();
	return JOB_COMPLETED;
}

JOB_RETURN Render::LoadSingleComponent(void * ptr)
{
	//assert(dynamic_cast<RenderComponent*>(ptr));
	auto obj = static_cast<RenderComponent*>(ptr);
	{
		if (!_models->HasItem(obj->GetModelPath(), obj->GetModelAdd()))
			m_task.RegisterJob(new Job(bind_function(&Render::ModelFileImport, this), "Model_Import", obj, Job::JOB_LOAD_MODEL));
		else
			this->GenerateVAO(obj);

		if (!_shaders->HasItem(obj->GetShaderPath(), obj->GetShaderAdd()))
			m_task.RegisterJob(new Job(bind_function(&Render::ShaderFileImport, this), "Shader_Import", obj, Job::JOB_LOAD_SHADER));

		if (!_textures->HasItem(obj->GetTexturePath(), obj->GetTextureAdd()))
			m_task.RegisterJob(new Job(bind_function(&Render::TextureFileImport, this), "Texture_Import", obj, Job::JOB_LOAD_TEXTURE));
	}
	return JOB_COMPLETED;
}

JOB_RETURN Render::LoadComponents(void * ptr)
{
	for (auto & render : *static_cast<std::vector<BaseComponent*>*>(ptr))
	{
		assert(dynamic_cast<RenderComponent*>(render));
		auto obj = static_cast<RenderComponent*>(render);
		{
			if (!_models->HasItem(obj->GetModelPath(), obj->GetModelAdd()))
				m_task.RegisterJob(new Job(bind_function(&Render::ModelFileImport, this), "Model_Import", render, Job::JOB_LOAD_MODEL));

			if (!_shaders->HasItem(obj->GetShaderPath(), obj->GetShaderAdd()))
				m_task.RegisterJob(new Job(bind_function(&Render::ShaderFileImport, this), "Shader_Import", render, Job::JOB_LOAD_SHADER));

			if (!_textures->HasItem(obj->GetTexturePath(), obj->GetTextureAdd()))
				m_task.RegisterJob(new Job(bind_function(&Render::TextureFileImport, this), "Texture_Import", render, Job::JOB_LOAD_TEXTURE));
		}
	}
	return JOB_COMPLETED;
}

bool Render::InitSDL()
{
	// Just for safety sake
	Uint32 subsystem_init = SDL_WasInit(SDL_INIT_VIDEO);
	if (!(subsystem_init & SDL_INIT_VIDEO))
	{
		printf("SDL INIT VIDEO failed! SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}

	m_task.RegisterJob(new Job(bind_function(&Render::GiveThreadedContext, this), "Threaded_Context", nullptr, Job::JOB_RENDER_DEFAULT));
	return true;
}

bool Render::InitGL()
{
	glViewport(0, 0, screen_width, screen_height);
	return true;
}

JOB_RETURN Render::GiveThreadedContext(void * ptr)
{
	Uint32 subsystem_init = SDL_WasInit(SDL_INIT_VIDEO);
	if (!(subsystem_init & SDL_INIT_VIDEO))
	{
		printf("SDL INIT VIDEO failed! SDL_ERROR: %s\n", SDL_GetError());
		return JOB_ISSUE;
	}

	sdl_gl_context = SDL_GL_CreateContext(sdl_window);
	if (sdl_gl_context == NULL)
	{
		printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		return JOB_ISSUE;
	}
	else
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		glewExperimental = GL_TRUE;
		GLenum glewError = glewInit();
		if (glewError != GLEW_OK)
		{
			printf("Error initializing GLEW! %s", glewGetErrorString(glewError));
			return JOB_ISSUE;
		}
	}
	return JOB_COMPLETED;
}

//std::mutex temp;

JOB_RETURN Render::ModelFileImport(void * ptr)
{
	//std::lock_guard<std::mutex> lock(temp);
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	if (LoadOBJModelFile(rc->GetModelPath(), rc->GetModelAdd()))
	{
		m_task.RegisterJob(new Job(bind_function(&Render::BindModel, this), "Bind_Model", rc, Job::JOB_BIND_MODEL));
		return JOB_COMPLETED;
	}
	return JOB_ISSUE;
}

JOB_RETURN Render::ShaderFileImport(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	if (LoadShaderFile(rc->GetVShaderPath(), rc->GetFShaderPath(), rc->GetShaderAdd()))
	{
		m_task.RegisterJob(new Job(bind_function(&Render::BindShader, this), "Bind_Shader", rc, Job::JOB_BIND_SHADER));
		return JOB_COMPLETED;
	}
	return JOB_ISSUE;
}

JOB_RETURN Render::TextureFileImport(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	if (LoadTextureFile(rc->GetTexturePath(), rc->GetTextureAdd()))
	{
		m_task.RegisterJob(new Job(bind_function(&Render::BindTexture, this), "Bind_Texture", rc, Job::JOB_BIND_TEXTURE));
		return JOB_COMPLETED;
	}
	return JOB_ISSUE;
}

JOB_RETURN Render::BindModel(void * ptr)
{
	RenderComponent * rc_cp = static_cast<RenderComponent*>(ptr);
	if (rc_cp->GetModel() != nullptr)
	{
		Model * model_ptr = rc_cp->GetModel();

		glGenBuffers(1, &model_ptr->elem_buff_obj);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_ptr->elem_buff_obj);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			(sizeof(GLuint) * model_ptr->ISize),
			model_ptr->_indices,
			GL_STATIC_DRAW);

		glGenBuffers(1, &model_ptr->vert_buff_obj);
		glBindBuffer(GL_ARRAY_BUFFER, model_ptr->vert_buff_obj);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * model_ptr->VSize),
			model_ptr->_vertices,
			GL_STATIC_DRAW);
		for (auto & rc_cp : m_scene.GetComponents(SceneManager::RENDER))
		{
			assert(dynamic_cast<RenderComponent*>(rc_cp));
			auto render = static_cast<RenderComponent*>(rc_cp);
			if (render->GetModel() == model_ptr)
			{
				this->GenerateVAO(render);
			}
		}
	}
	else
	{
		printf("Object skipped no available model\n");
		return JOB_ISSUE;
	}

	return JOB_COMPLETED;
}

void Render::GenerateVAO(RenderComponent * const render)
{
	glGenVertexArrays(1, &render->GetVertexArray());
	glBindVertexArray(render->GetVertexArray());

	glBindBuffer(GL_ARRAY_BUFFER, render->GetModel()->vert_buff_obj);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(5 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
}

JOB_RETURN Render::BindTexture(void * ptr)
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
		return JOB_ISSUE;
	}
	return JOB_COMPLETED;
}

JOB_RETURN Render::BindShader(void * ptr)
{
	RenderComponent * rc_cp = static_cast<RenderComponent*>(ptr);
	Shader & shader = *rc_cp->GetShaderAdd();
	if (rc_cp->GetShader() != nullptr)
	{
		const GLuint program = (shader.shade_prog = glCreateProgram());
		glAttachShader(program, shader._shaderID_Vert);
		glAttachShader(program, shader._shaderID_Frag);
		glLinkProgram(program);
		GLint programSuccess = GL_FALSE;

		glGetProgramiv(program, GL_LINK_STATUS, &programSuccess);
		if (programSuccess != GL_TRUE)
		{
			printf("Error linking program %d!\n", glGetError());
			return JOB_ISSUE;
		}

		glUseProgram(program);

		shader.r_model_mat4_loc = glGetUniformLocation(program, "model_matrix");
		shader.r_project_mat4_loc = glGetUniformLocation(program, "projection_matrix");
		shader.r_color_vec4_loc = glGetUniformLocation(program, "color_vec");

		shader.r_text_color = glGetUniformLocation(program, "tex_color");
		shader.r_text_unit = glGetUniformLocation(program, "tex_unit");
	}
	else
	{
		printf("Object skipped no available shaders\n");
		return JOB_ISSUE;
	}
	
	return JOB_COMPLETED;
}

