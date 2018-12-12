#include "Render.h"
#include "FileLoader.h"

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

JOB_RETURN Render::Load(void* content)
{
	if (!InitSDL(static_cast<SDL_GLContext>(content)))
	{
		printf("SDL Initialization failed, see function Load()");
		return JOB_ISSUE;
	}
	if (!InitGL())
	{
		printf("GL Initialization failed, see function Load()");
		return JOB_ISSUE;
	}
	return JOB_COMPLETED;
}

void Render::InitUpdate(CameraComponent * c_cp, const btTransform tran)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	btScalar matrix[16];

	tran.getOpenGLMatrix(matrix);

	project_value_ptr = c_cp->SetProjectionLook(getGLMMatrix4(matrix));
}

JOB_RETURN Render::UpdateLoop
(
	void * ptr
)
{
	ComponentManager * c_manager = static_cast<ComponentManager*>(ptr);
	EntityID camera_id;
	this->InitUpdate(c_manager->GetComponent<CameraComponent*>(camera_id),
		c_manager->GetComponent<Transform*>(camera_id)->_transform);

	for (auto render_it : c_manager->FindAllTypes<RenderComponent*>())
	{
		this->ComponentUpdate(project_value_ptr, 
			render_it.second,
			c_manager->GetComponent<Transform*>(render_it.first)->_transform);
	}
	/*for (auto entity : current_scene->GetEntityManager()->retreive_list())
	{
		if ((rc = current_scene->GetCompManager()->GetComponent<RenderComponent*>(entity.first)) != nullptr)
		{
			this->ComponentUpdate( project_value_ptr, rc, c_manager->GetComponent<Transform*>(entity.first)->_transform);
		}
	}*/

	this->FinalUpdate();
	return JOB_COMPLETED;
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
		glBindVertexArray(rc->GetVertexArray());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc->GetElementBuffer());

		glUseProgram(*rc->GetShaderProgram());

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
		transform.getOpenGLMatrix(matrix);

		glUniformMatrix4fv(rc->GetProjectionMatrixLoc(), 1, GL_FALSE, project_value);
		glUniformMatrix4fv(rc->GetModelMatrixLoc(), 1, GL_FALSE, glm::value_ptr(getGLMMatrix4(matrix)));
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

JOB_RETURN Render::InitRenderComp(void * ptr)
{
	ComponentManager * c_manager = static_cast<ComponentManager*>(ptr);

	for (auto render_it : c_manager->FindAllTypes<RenderComponent*>())
	{
		// Loading model job
		TaskManager::Instance().RegisterJob(new Job(bind_function(&Render::LoadModel, this), "Load_Model", render_it.second));

		// Loading shader job
		TaskManager::Instance().RegisterJob(new Job(bind_function(&Render::LoadShader, this), "Load_Shader", render_it.second, Job::RENDER_TYPE));

		// Loading texture job
 		TaskManager::Instance().RegisterJob(new Job(bind_function(&Render::LoadTexture, this), "Load_Texture", render_it.second));
	}

	return JOB_COMPLETED;
}

JOB_RETURN Render::LoadModel(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	Job * bind_model_job;

	auto load_type = _models->HasItem(rc->GetModelPath(), rc->GetModelAdd());
	switch(load_type)
	{
	case LOAD::CURRENT_LOAD:
		if (LoadOBJModelFile(rc->GetModelPath(), rc->GetModelAdd()))
		{
			TaskManager::Instance().RegisterJob(new Job(bind_function(&Render::BindModel, this), "Bind_Model", rc, Job::RENDER_TYPE));
			return JOB_COMPLETED;
		}
		break;
	case LOAD::WAIT_LOAD:
		bind_model_job = new Job(bind_function(&Render::BindModel, this), "Bind_Model", rc, Job::RENDER_TYPE);
		TaskManager::Instance().RegisterJob(new Job(bind_function(&Model::CheckDoneLoad, rc->GetModel()), "Model_Checker"), bind_model_job);
		TaskManager::Instance().RegisterJob(bind_model_job, true);
		return JOB_COMPLETED;
		break;
	case LOAD::DONE_LOAD:
		TaskManager::Instance().RegisterJob(new Job(bind_function(&Render::BindModel, this), "Bind_Model", rc, Job::RENDER_TYPE));
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
	Job * bind_shader_job;

	auto load_type = _shaders->HasItem(rc->GetShaderPath(), rc->GetShaderAdd());
	switch (load_type)
	{
	case CURRENT_LOAD:
		if (LoadShaderFile(rc->GetVShaderPath(), rc->GetFShaderPath(), rc->GetShaderAdd()))
		{
			TaskManager::Instance().RegisterJob(new Job(bind_function(&Render::BindShader, this), "Bind_Shader", rc, Job::RENDER_TYPE));
			return JOB_COMPLETED;
		}
		break;
	case WAIT_LOAD:
		bind_shader_job = new Job(bind_function(&Render::BindShader, this), "Bind_Shader", rc, Job::RENDER_TYPE);
		TaskManager::Instance().RegisterJob(new Job(bind_function(&Shader::CheckDoneLoad, rc->GetShader()), "Model_Checker"), bind_shader_job);
		TaskManager::Instance().RegisterJob(bind_shader_job, true);
		return JOB_COMPLETED;
		break;
	case DONE_LOAD:
		TaskManager::Instance().RegisterJob(new Job(bind_function(&Render::BindShader, this), "Bind_Shader", rc, Job::RENDER_TYPE));
		return JOB_COMPLETED;
		break;
	}
	return JOB_ISSUE;
}

JOB_RETURN Render::LoadTexture(void * ptr)
{
	RenderComponent * rc = static_cast<RenderComponent*>(ptr);
	Job * bind_texture_job;

	auto load_type = _textures->HasItem(rc->GetTexturePath(), rc->GetTextureAdd());
	switch (load_type)
	{
	case CURRENT_LOAD:
		if (LoadTextureFile(rc->GetTexturePath(), rc->GetTextureAdd()))
		{
			TaskManager::Instance().RegisterJob(new Job(bind_function(&Render::BindTexture, this), "Bind_Texture", rc, Job::RENDER_TYPE));
			return JOB_COMPLETED;
		}
		break;
	case WAIT_LOAD:
		bind_texture_job = new Job(bind_function(&Render::BindTexture, this), "Bind_Texture", rc, Job::RENDER_TYPE);
		TaskManager::Instance().RegisterJob(new Job(bind_function(&Texture::CheckDoneLoad, rc->GetTexture()), "Texture_Checker"), bind_texture_job);
		TaskManager::Instance().RegisterJob(bind_texture_job, true);
		return JOB_COMPLETED;
		break;
	case DONE_LOAD:
		TaskManager::Instance().RegisterJob(new Job(bind_function(&Render::BindTexture, this), "Bind_Texture", rc, Job::RENDER_TYPE));
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
		glGenBuffers(1, &rc_cp->GetElementBuffer());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rc_cp->GetElementBuffer());
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			(sizeof(GLuint) * rc_cp->GetModel()->ISize),
			rc_cp->GetModel()->_indices,
			GL_STATIC_DRAW);

		glGenVertexArrays(1, &rc_cp->GetVertexArray());
		glBindVertexArray(rc_cp->GetVertexArray());

		glGenBuffers(1, &rc_cp->GetVertexBuffer());
		glBindBuffer(GL_ARRAY_BUFFER, rc_cp->GetVertexBuffer());
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
	if (rc_cp->GetShader() != nullptr)
	{
		const GLuint *program = rc_cp->SetShaderProgram(glCreateProgram());

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
		else
		{
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
	}
	else
	{
		printf("Object skipped no available shaders\n");
		return JOB_ISSUE;
	}
	return JOB_COMPLETED;
}

bool Render::InitSDL(SDL_GLContext context)
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

bool Render::InitGL()
{
	glViewport(0, 0, screen_width, screen_height);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	return true;
}
