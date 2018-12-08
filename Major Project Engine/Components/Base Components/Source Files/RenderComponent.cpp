#include "RenderComponent.h"
#include "ThreadManager.h"

RenderComponent::RenderComponent(const std::string model, const std::string v_shader, const std::string f_shader, const std::string texture) 
{
	//ThreadManager::register_job(new Job(bind_function(&FileLoader::add_model, FileLoader::Instance()), nullptr, IO_TYPE);
	/*FileLoader::Instance().load_obj_file(model, current_model);
	FileLoader::Instance().load_shader(v_shader, GL_VERTEX_SHADER, current_v_shader);
	FileLoader::Instance().load_shader(f_shader, GL_FRAGMENT_SHADER, current_f_shader);
	FileLoader::Instance().load_texture(texture, current_texture);*/
}

RenderComponent::RenderComponent()
{
	current_model = nullptr;
	current_shader = nullptr;
	current_texture = nullptr;
}

RenderComponent::~RenderComponent() {
	if (current_model != nullptr) delete current_model;
	if (current_shader != nullptr) delete current_shader;
	if (current_texture != nullptr) delete current_texture;
}