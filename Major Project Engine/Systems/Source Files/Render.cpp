#include "Render.h"
#include "FileLoader.h"

/*
Swaps the btTransform to the glm::mat4 for rendering
*/
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
	EventHandler::Instance().SubscribeEvent(EventType::RENDER_NEW_OBJECT, this);
	EventHandler::Instance().SubscribeEvent(EventType::STATE_CHANGE, this);
	EventHandler::Instance().SubscribeEvent(EventType::NEW_FRAME, this);
	EventHandler::Instance().SubscribeEvent(EventType::BOX_COLLISION_PHYSICS_TO_RENDER, this);
	EventHandler::Instance().SubscribeEvent(EventType::BULLET_COLLISION_PHYSICS_TO_RENDER, this);

	// Jobs that need to wait on other jobs go here
	{
		m_task.dictionary.emplace(job::JOB_RENDER_THREADING_CONTEXT, std::vector<job::JOB_ID>());
		m_task.dictionary[job::JOB_RENDER_THREADING_CONTEXT].emplace_back(job::JOB_RENDER_LOAD);
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
	SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);		// Get the window size from the window

	if (!InitSDL())													// initialize SDL
	{
		printf("SDL Initialization failed, see function Load()");
		return false;
	}
	if (!InitGL())													// Initialize OpenGL
	{
		printf("GL Initialization failed, see function Load()");
		return false;
	}
	return true;
}

Render::~Render() 
{
	//std::cout << "Render Destructor called" << std::endl;
	SDL_DestroyWindow(sdl_window);
	sdl_window = nullptr;
}

bool Render::Load()
{
	// Initialize Camera
	{
		for (auto camera : m_scene.GetComponents(SceneManager::CAMERA))
		{
			auto obj = static_cast<CameraComponent*>(camera);
			{
				obj->_projection = glm::perspective(glm::radians(obj->_fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, obj->_near, obj->_far);
			}
		}
	}

	m_task.RegisterJob(new Job(bind_function(&Render::LoadComponents, this), "Render_Component_Loader", &m_scene.GetComponents(SceneManager::RENDER), job::JOB_RENDER_LOAD), true);
	m_task.RegisterJob(new Job(bind_function(&Render::GiveThreadedContext, this), "Threaded_Context", nullptr, job::JOB_RENDER_THREADING_CONTEXT), false);
	return true;
}

void Render::Close(void* content) {}

void Render::HandleEvent(const EventType & e, void * data)
{
	switch (e)
	{
	case EventType::RENDER_NEW_OBJECT:
		m_task.RegisterJob(new Job(bind_function(&Render::LoadSingleComponent, this), "Load_Single_Render", data, job::JOB_RENDER_LOAD_SINGLE), false);
		break;
	case EventType::STATE_CHANGE:
	{
		this->gs = *static_cast<GAME_STATE*>(data);
		break;
	}
	case EventType::NEW_FRAME:
		if (gs == GAME_STATE::PLAYING || this->gs == GAME_STATE::DEBUG_LOAD || this->gs == GAME_STATE::PAUSED || this->gs == GAME_STATE::FULL_PAUSE)
			m_task.RegisterJob(new Job(bind_function(&Render::Update, this), "Render_Update", &m_scene.GetComponents(SceneManager::RENDER), job::JOB_RENDER_UPDATE), true);
		break;
	case EventType::BOX_COLLISION_PHYSICS_TO_RENDER:
		num_of_box_coll = *static_cast<int*>(data);
		break;
	case EventType::BULLET_COLLISION_PHYSICS_TO_RENDER:
		num_of_bull_coll = *static_cast<int*>(data);
		break;
	default:
		break;
	}
}

void Render::InitUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	for (auto camera : m_scene.GetComponents(SceneManager::CAMERA))		// Load the camera component and set the project look matrix
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
			projection_look_matrix = obj->_projection * look_matrix;
		}
	}
}

JOB_RETURN Render::Update(void * ptr)
{
	this->InitUpdate();

	Model * model_ptr = nullptr;
	Texture * texture_ptr = nullptr;
	Shader * shader_ptr = nullptr;

	for (auto obj : *static_cast<std::vector<BaseComponent*>*>(ptr))
	{
		assert(dynamic_cast<RenderComponent*>(obj));
		auto render = static_cast<RenderComponent*>(obj);
		{
			if (model_ptr == nullptr || !(render->model_id == model_ptr->_id))		// if the model is already selected don't swap
			{
				if (model_ptr != nullptr) glBindVertexArray(0);						

				model_ptr = _models.GetItem(render->model_path);					// This saved me a lot of computing time
			}

			if (shader_ptr == nullptr || !(render->shader_id == shader_ptr->_id))	// if the shader is already select don't swap
			{
				if (shader_ptr != nullptr) glUseProgram(0);

				shader_ptr = _shaders.GetItem(render->vertex_shader_path + render->fragment_shader_path);
				glUseProgram(shader_ptr->shade_prog);

				glUniform1i(shader_ptr->r_text_unit, 0);
				glUniform4f(shader_ptr->r_text_color, 1.0f, 1.0f, 1.0f, 1.0f);

				glUniformMatrix4fv(shader_ptr->r_project_mat4_loc, 1, GL_FALSE, glm::value_ptr(projection_look_matrix));
			}

			if (texture_ptr == nullptr || !(render->texture_id == texture_ptr->_id))	// if the texture is already select don't swap
			{
				if (texture_ptr != nullptr) glBindTexture(GL_TEXTURE_2D, 0);

				texture_ptr = _textures.GetItem(render->texture_path);
				if (texture_ptr != nullptr && texture_ptr->TextureID != 0)
				{
					glActiveTexture(GL_TEXTURE0);
					glUniform4f(shader_ptr->r_color_vec4_loc, render->_color.x, render->_color.y, render->_color.z, render->_color.w);
				}
				else
				{
					glm::vec4 _color = glm::vec4(1.0f, 0.411f, 0.705f, 1.0f);
					glUniform4f(shader_ptr->r_color_vec4_loc, _color.x, _color.y, _color.z, _color.w);
				}
			}

			glBindVertexArray(render->vert_arr_obj);							// set the vertex array object
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_ptr->elem_buff_obj);	// set the indices buffer object
			glBindTexture(GL_TEXTURE_2D, texture_ptr->TextureID);				// set the texture

			btScalar matrix[16];
			static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, obj->_id))->_transform.getOpenGLMatrix(matrix);		// get the transform of the object

			glUniformMatrix4fv(shader_ptr->r_model_mat4_loc, 1, GL_FALSE, glm::value_ptr(getGLMMatrix4(matrix)));
			glDrawElements(GL_TRIANGLES, model_ptr->ISize, GL_UNSIGNED_INT, NULL);			// draw the object
		}
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glBindVertexArray(0);
	SDL_GL_SwapWindow(sdl_window);			// swap the buffer

	return JOB_COMPLETED;
}

JOB_RETURN Render::LoadSingleComponent(void * ptr)
{
	//assert(dynamic_cast<RenderComponent*>(ptr));
	auto render = static_cast<RenderComponent*>(ptr);
	{
		if (!_models.HasItem(render->model_path, render->model_id))		// if storage can't find the model then file loader it is
			m_task.RegisterJob(new Job(bind_function(&Render::ModelFileImport, this), "Model_Import", render, job::JOB_LOAD_MODEL));
		else
			this->GenerateVAO(render);

		if (!_shaders.HasItem(render->vertex_shader_path + render->fragment_shader_path, render->shader_id))	// if storage can't find the shader then file loader it is
			m_task.RegisterJob(new Job(bind_function(&Render::ShaderFileImport, this), "Shader_Import", render, job::JOB_LOAD_SHADER));

		if (!_textures.HasItem(render->texture_path, render->texture_id))	// if storage can't find the texture then file loader it is
			m_task.RegisterJob(new Job(bind_function(&Render::TextureFileImport, this), "Texture_Import", render, job::JOB_LOAD_TEXTURE));
	}
	return JOB_COMPLETED;
}

JOB_RETURN Render::LoadComponents(void * ptr)
{
	for (auto & obj : *static_cast<std::vector<BaseComponent*>*>(ptr))
	{
		assert(dynamic_cast<RenderComponent*>(obj));
		auto render = static_cast<RenderComponent*>(obj);
		{
			if (!_models.HasItem(render->model_path, render->model_id))	// if storage can't find the model then file loader it is
			{
				model_listeners[render->model_id] = std::vector<RenderComponent*>();
				m_task.RegisterJob(new Job(bind_function(&Render::ModelFileImport, this), "Model_Import", render, job::JOB_LOAD_MODEL));
			}
			else
			{
				model_listeners[render->model_id].emplace_back(render);
			}

			if (!_shaders.HasItem(render->vertex_shader_path + render->fragment_shader_path, render->shader_id))	// if storage can't find the shader then file loader it is
				m_task.RegisterJob(new Job(bind_function(&Render::ShaderFileImport, this), "Shader_Import", render, job::JOB_LOAD_SHADER));

			if (!_textures.HasItem(render->texture_path, render->texture_id))	// if storage can't find the texture then file loader it is
				m_task.RegisterJob(new Job(bind_function(&Render::TextureFileImport, this), "Texture_Import", render, job::JOB_LOAD_TEXTURE));
		}
	}
	return JOB_COMPLETED;
}

bool Render::InitSDL()
{
	// Just for safety sake
	Uint32 subsystem_init = SDL_WasInit(SDL_INIT_VIDEO);
	if (!(subsystem_init & SDL_INIT_VIDEO))								// initialize SDL video if it hasn't
	{
		printf("SDL INIT VIDEO failed! SDL_ERROR: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

bool Render::InitGL()
{
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	return true;
}

JOB_RETURN Render::GiveThreadedContext(void * ptr)
{
	Uint32 subsystem_init = SDL_WasInit(SDL_INIT_VIDEO);				// initialize SDL video if it hasn't, done twice just incase of threading issue
	if (!(subsystem_init & SDL_INIT_VIDEO))
	{
		printf("SDL INIT VIDEO failed! SDL_ERROR: %s\n", SDL_GetError());
		return JOB_ISSUE;
	}

	sdl_gl_context = SDL_GL_CreateContext(sdl_window);					// Create the OpenGL context
	if (sdl_gl_context == NULL)
	{
		printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		return JOB_ISSUE;
	}
	else
	{
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);				// enable cull face, depth test, texture 2d
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);

		glewExperimental = GL_TRUE;									// glew init
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
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	if (LoadOBJModelFile(rc->model_path, _models.GetItem(rc->model_id)))	// load the obj model
	{
		m_task.RegisterJob(new Job(bind_function(&Render::BindModel, this), "Bind_Model", rc, job::JOB_BIND_MODEL));
		return JOB_COMPLETED;
	}
	return JOB_ISSUE;
}

JOB_RETURN Render::ShaderFileImport(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	if (LoadShaderFile(rc->vertex_shader_path, rc->fragment_shader_path, _shaders.GetItem(rc->shader_id)))	// load the shader
	{
		m_task.RegisterJob(new Job(bind_function(&Render::BindShader, this), "Bind_Shader", rc, job::JOB_BIND_SHADER));
		return JOB_COMPLETED;
	}
	return JOB_ISSUE;
}

JOB_RETURN Render::TextureFileImport(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	if (LoadTextureFile(rc->texture_path, _textures.GetItem(rc->texture_id)))		// load the texture
	{
		m_task.RegisterJob(new Job(bind_function(&Render::BindTexture, this), "Bind_Texture", rc, job::JOB_BIND_TEXTURE));
		return JOB_COMPLETED;
	}
	return JOB_ISSUE;
}

JOB_RETURN Render::BindModel(void * ptr)
{
	RenderComponent * rc_cp = static_cast<RenderComponent*>(ptr);
	if (rc_cp->model_id != 0)
	{
		Model * model_ptr = _models.GetItem(rc_cp->model_id);

		glGenBuffers(1, &model_ptr->elem_buff_obj);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_ptr->elem_buff_obj);	// bind the indices
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			(sizeof(GLuint) * model_ptr->ISize),
			model_ptr->_indices,
			GL_STATIC_DRAW);

		glGenBuffers(1, &model_ptr->vert_buff_obj);
		glBindBuffer(GL_ARRAY_BUFFER, model_ptr->vert_buff_obj);			// bind the vertices
		glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * model_ptr->VSize),
			model_ptr->_vertices,
			GL_STATIC_DRAW);

		this->GenerateVAO(rc_cp);

		for (auto listen : model_listeners[model_ptr->_id])		// for all listeners generate the vertex array object
		{
			assert(dynamic_cast<RenderComponent*>(listen));
			auto render = static_cast<RenderComponent*>(listen);
			if (render->model_id == model_ptr->_id)
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
	glGenVertexArrays(1, &render->vert_arr_obj);
	glBindVertexArray(render->vert_arr_obj);

	glBindBuffer(GL_ARRAY_BUFFER, _models.GetItem(render->model_id)->vert_buff_obj);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0);									// vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));		// uv (textures)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(5 * sizeof(GL_FLOAT)));		// normals
	glEnableVertexAttribArray(2);
}

JOB_RETURN Render::BindTexture(void * ptr)
{
	RenderComponent * rc_cp = static_cast<RenderComponent*>(ptr);
	if (rc_cp->texture_id != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		Texture * text = _textures.GetItem(rc_cp->texture_id);
		glGenTextures(1, &text->TextureID);							// generate texture id
		glBindTexture(GL_TEXTURE_2D, text->TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,						// get the 2d texture
			text->texWidth,
			text->texWidth,
			0, GL_RGBA, GL_UNSIGNED_BYTE,
			text->_texture);

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
	if (rc_cp->shader_id != 0)
	{
		Shader * shader = _shaders.GetItem(rc_cp->shader_id);
		const GLuint program = (shader->shade_prog = glCreateProgram());
		glAttachShader(program, shader->_shaderID_Vert);		// attach the shader to the program
		glAttachShader(program, shader->_shaderID_Frag);		// attach the shader to the program
		glLinkProgram(program);
		GLint programSuccess = GL_FALSE;

		glGetProgramiv(program, GL_LINK_STATUS, &programSuccess);
		if (programSuccess != GL_TRUE)
		{
			printf("Error linking program %d!\n", glGetError());
			return JOB_ISSUE;
		}

		glUseProgram(program);

		shader->r_model_mat4_loc = glGetUniformLocation(program, "model_matrix");				// find the locations for the shader program
		shader->r_project_mat4_loc = glGetUniformLocation(program, "projection_matrix");
		shader->r_color_vec4_loc = glGetUniformLocation(program, "color_vec");

		shader->r_text_color = glGetUniformLocation(program, "tex_color");
		shader->r_text_unit = glGetUniformLocation(program, "tex_unit");
	}
	else
	{
		printf("Object skipped no available shaders\n");
		return JOB_ISSUE;
	}
	
	return JOB_COMPLETED;
}

