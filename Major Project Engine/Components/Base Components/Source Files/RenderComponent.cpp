#include "RenderComponent.h"

RenderComponent::RenderComponent(const std::string model, const std::string v_shader, const std::string f_shader, const std::string texture) 
{
	FileLoader::Instance().load_obj_file(model, current_model);
	FileLoader::Instance().load_shader(v_shader, GL_VERTEX_SHADER, current_v_shader);
	FileLoader::Instance().load_shader(f_shader, GL_FRAGMENT_SHADER, current_f_shader);
	FileLoader::Instance().load_texture(texture, current_texture);
}

RenderComponent::RenderComponent()
{
	current_model = nullptr;
	current_v_shader = nullptr;
	current_f_shader = nullptr;
	current_texture = nullptr;
}

RenderComponent::~RenderComponent() {
	if (current_model != nullptr) 
	{ 
		current_model.reset();
		current_model = nullptr; 
	} 
	if (current_v_shader != nullptr)
	{
		current_v_shader.reset();
		current_v_shader = nullptr;
	}
	if (current_f_shader != nullptr)
	{
		current_f_shader.reset();
		current_f_shader = nullptr;
	}
	if (current_texture != nullptr)
	{
		current_texture.reset();
		current_texture = nullptr;
	}
}