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

	// Jobs that need to wait on other jobs go here
	/*{
		m_task.dictionary.emplace(Job::JOB_RENDER_UPDATE, std::vector<Job::JOB_ID>());
		m_task.dictionary[Job::JOB_RENDER_UPDATE].emplace_back(Job::JOB_PHYSICS_UPDATE);
		m_task.dictionary[Job::JOB_RENDER_UPDATE].emplace_back(Job::JOB_PHYSICS_COMPONENT);
	}*/
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

	projection_matrix = glm::perspective(glm::radians(_fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, _near, _far);
	look_matrix = glm::lookAtRH(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	btScalar transform[16];
	btTransform camera_tranform = btTransform().getIdentity();
	camera_tranform.setRotation(btQuaternion(0.f, 1.f, 0.f));
	camera_tranform.setOrigin(btVector3(btScalar(0.f), btScalar(-5.f), btScalar(-75.f)));
	camera_tranform.getOpenGLMatrix(transform);

	projection_look_matrix = projection_matrix * (look_matrix *	getGLMMatrix4(transform));

	m_task.RegisterJob(new Job(bind_function(&Render::InitRenderComp, this), "Init_Render_Objects", &m_scene.GetComponents(SceneManager::RENDER), Job::JOB_RENDER_LOAD));
	return true;
}


void Render::Close(void* content) {}

void Render::InitUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
}

void Render::ComponentUpdate(RenderComponent * rc, Transform * trans)
{
	Model * model_ptr = rc->GetModel();
	glBindVertexArray(rc->GetVertexArray());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_ptr->elem_buff_obj);

	glUseProgram(rc->GetShader()->shade_prog);

	if (rc->GetTexture() != nullptr && rc->GetTexture()->TextureID != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, rc->GetTexture()->TextureID);
		glUniform4f(rc->GetColorShaderLoc(), rc->GetColor().x, rc->GetColor().y, rc->GetColor().z, rc->GetColor().w);
	}
	else
	{
		glm::vec4 _color = glm::vec4(1.0f, 0.411f, 0.705f, 1.0f);
		glUniform4f(rc->GetColorShaderLoc(), _color.x, _color.y, _color.z, _color.w);
	}

	glUniform1i(rc->r_text_unit, 0);
	glUniform4f(rc->r_text_color, 1.0f, 1.0f, 1.0f, 1.0f);

	btScalar matrix[16];
	trans->_transform.getOpenGLMatrix(matrix);

	glUniformMatrix4fv(rc->GetProjectionMatrixLoc(), 1, GL_FALSE, glm::value_ptr(projection_look_matrix));
	glUniformMatrix4fv(rc->GetModelMatrixLoc(), 1, GL_FALSE, glm::value_ptr(getGLMMatrix4(matrix)));
	glDrawElements(GL_TRIANGLES, model_ptr->ISize, GL_UNSIGNED_INT, NULL);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);
	glUseProgram(0);
}

JOB_RETURN Render::Update(void * ptr)
{
	this->InitUpdate();

	//Timer::Instance().Start();
	for (auto render_it : *static_cast<std::vector<BaseComponent*>*>(ptr))
	{
		assert(dynamic_cast<RenderComponent*>(render_it));
		{
			ComponentUpdate(static_cast<RenderComponent*>(render_it), static_cast<Transform*>(m_scene.FindComponent(SceneManager::TRANSFORM, render_it->_id)));
		}
	}
	//Timer::Instance().Stop();

	this->SwapBuffers();
	return JOB_COMPLETED;
}

JOB_RETURN Render::InitRenderComp(void * ptr)
{
	//ComponentManager * m_components = static_cast<ComponentManager*>(ptr);

	for (auto & render : *static_cast<std::vector<BaseComponent*>*>(ptr))
	{
		assert(dynamic_cast<RenderComponent*>(render));
		auto obj = static_cast<RenderComponent*>(render);
		{
			// Loading model job
			m_task.RegisterJob(new Job(bind_function(&Render::LoadModel, this), "Load_Model", render, Job::JOB_LOAD_MODEL));

			// Loading texture job
			m_task.RegisterJob(new Job(bind_function(&Render::LoadTexture, this), "Load_Texture", render, Job::JOB_LOAD_TEXTURE));

			// Loading shader job
			m_task.RegisterJob(new Job(bind_function(&Render::LoadShader, this), "Load_Shader", render, Job::JOB_LOAD_SHADER));
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

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);

	sdl_window = SDL_CreateWindow("Major Project", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	if (sdl_window == NULL)
	{
		printf("Error creating window");
		return false;
	}

	m_task.RegisterJob(new Job(bind_function(&Render::GiveThreadedContext, this), "Threaded_Context"));
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
		//SDL_GL_SwapWindow(sdl_window);
	}
	return JOB_COMPLETED;
}

JOB_RETURN Render::LoadModel(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	Job * bind_model_job, * check_job;

	switch(_models->HasItem(rc->GetModelPath(), rc->GetModelAdd()))
	{
	case LOAD::CURRENT_LOAD:
		if (LoadOBJModelFile(rc->GetModelPath(), rc->GetModelAdd()))
		{
			m_task.RegisterJob(new Job(bind_function(&Render::BindModel, this), "Bind_Model", rc, Job::JOB_BIND_MODEL));
			return JOB_COMPLETED;
		}
		break;
	case LOAD::WAIT_LOAD:
		bind_model_job = new Job(bind_function(&Render::BindModel, this), "Bind_Model", rc, Job::JOB_BIND_MODEL);
		check_job = new Job(bind_function(&Model::CheckDoneLoad, rc->GetModel()), "Model_Checker", nullptr, Job::JOB_MODEL_CHECKER);
		m_task.RegisterJob(bind_model_job, true);
		m_task.RegisterJob(check_job, false, bind_model_job);
		return JOB_COMPLETED;
		break;
	case LOAD::DONE_LOAD:
		m_task.RegisterJob(new Job(bind_function(&Render::BindModel, this), "Bind_Model", rc, Job::JOB_BIND_MODEL));
		return JOB_COMPLETED;
		break;
	default:
		break;
	}
	return JOB_ISSUE;
}

JOB_RETURN Render::LoadShader(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	Job * bind_shader_job, * check_job;

	switch (_shaders->HasItem(rc->GetShaderPath(), rc->GetShaderAdd()))
	{
	case CURRENT_LOAD:
		if (LoadShaderFile(rc->GetVShaderPath(), rc->GetFShaderPath(), rc->GetShaderAdd()))
		{
			m_task.RegisterJob(new Job(bind_function(&Render::BindShader, this), "Bind_Shader", rc, Job::JOB_BIND_SHADER));
			return JOB_COMPLETED;
		}
		break;
	case WAIT_LOAD:
		bind_shader_job = new Job(bind_function(&Render::BindShader, this), "Bind_Shader", rc, Job::JOB_BIND_SHADER);
		check_job = new Job(bind_function(&Shader::CheckDoneLoad, rc->GetShader()), "Shader_Checker", nullptr, Job::JOB_SHADER_CHECKER);
		m_task.RegisterJob(bind_shader_job, true);
		m_task.RegisterJob(check_job, false, bind_shader_job);
		return JOB_COMPLETED;
		break;
	case DONE_LOAD:
		m_task.RegisterJob(new Job(bind_function(&Render::BindShader, this), "Bind_Shader", rc, Job::JOB_BIND_SHADER));
		return JOB_COMPLETED;
		break;
	}
	return JOB_ISSUE;
}

JOB_RETURN Render::LoadTexture(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	Job * bind_texture_job, * check_job;

	switch (_textures->HasItem(rc->GetTexturePath(), rc->GetTextureAdd()))
	{
	case CURRENT_LOAD:
		if (LoadTextureFile(rc->GetTexturePath(), rc->GetTextureAdd()))
		{
			m_task.RegisterJob(new Job(bind_function(&Render::BindTexture, this), "Bind_Texture", rc, Job::JOB_BIND_TEXTURE));
			return JOB_COMPLETED;
		}
		break;
	case WAIT_LOAD:
		bind_texture_job = new Job(bind_function(&Render::BindTexture, this), "Bind_Texture", rc, Job::JOB_BIND_TEXTURE);
		check_job = new Job(bind_function(&Texture::CheckDoneLoad, rc->GetTexture()), "Texture_Checker", nullptr, Job::JOB_TEXTURE_CHECKER);
		m_task.RegisterJob(bind_texture_job, true);
		m_task.RegisterJob(check_job, false, bind_texture_job);
		return JOB_COMPLETED;
		break;
	case DONE_LOAD:
		m_task.RegisterJob(new Job(bind_function(&Render::BindTexture, this), "Bind_Texture", rc, Job::JOB_BIND_TEXTURE));
		return JOB_COMPLETED;
		break;
	}
	return JOB_ISSUE;
}

JOB_RETURN Render::BindModel(void * ptr)
{
	RenderComponent * rc_cp = static_cast<RenderComponent*>(ptr);
	if (rc_cp->GetModel() != nullptr)
	{
		Model * model_ptr = rc_cp->GetModel();

		glGenVertexArrays(1, &rc_cp->GetVertexArray());
		glBindVertexArray(rc_cp->GetVertexArray());

		if (rc_cp->GetModel()->elem_buff_obj == 0)
		{
			glGenBuffers(1, &model_ptr->elem_buff_obj);
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_ptr->elem_buff_obj);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			(sizeof(GLuint) * model_ptr->ISize),
			model_ptr->_indices,
			GL_STATIC_DRAW);

		if (model_ptr->vert_buff_obj == 0)
		{
			glGenBuffers(1, &model_ptr->vert_buff_obj);
		}
		glBindBuffer(GL_ARRAY_BUFFER, model_ptr->vert_buff_obj);
		glBufferData(GL_ARRAY_BUFFER, (sizeof(GLfloat) * model_ptr->VSize),
			model_ptr->_vertices,
			GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), 0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (GLvoid*)(5 * sizeof(GL_FLOAT)));
		glEnableVertexAttribArray(2);
	}
	else
	{
		printf("Object skipped no available model\n");
		return JOB_ISSUE;
	}

	
	return JOB_COMPLETED;
}

JOB_RETURN Render::BindTexture(void * ptr)
{
	RenderComponent * rc_cp = static_cast<RenderComponent*>(ptr);
	if (rc_cp->GetTexture() != nullptr)
	{
		glActiveTexture(GL_TEXTURE0);
		if (rc_cp->GetTexture()->TextureID == 0)
		{
			glGenTextures(1, &rc_cp->GetTexture()->TextureID);
		}
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
	if (rc_cp->GetShader() != nullptr)
	{
		const GLuint * program;
		if (rc_cp->GetShader()->shade_prog == 0)
		{
			program = &(rc_cp->GetShader()->shade_prog = glCreateProgram());
			glAttachShader(*program, rc_cp->GetShader()->_shaderID_Vert);
			glAttachShader(*program, rc_cp->GetShader()->_shaderID_Frag);
			glLinkProgram(*program);
			GLint programSuccess = GL_FALSE;
			glGetProgramiv(*program, GL_LINK_STATUS, &programSuccess);
			if (programSuccess != GL_TRUE)
			{
				printf("Error linking program %d!\n", glGetError());
				return JOB_ISSUE;
			}
		}
		program = &rc_cp->GetShader()->shade_prog;
		glUseProgram(*program);

		rc_cp->SetModelMatrixLoc(glGetUniformLocation(*program, "model_matrix"));
		rc_cp->SetProjectionMatrixLoc(glGetUniformLocation(*program, "projection_matrix"));
		rc_cp->SetColorShaderLoc(glGetUniformLocation(*program, "color_vec"));

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
	else
	{
		printf("Object skipped no available shaders\n");
		return JOB_ISSUE;
	}

	
	return JOB_COMPLETED;
}

