#include "RenderComponent.h"
#include "ThreadManager.h"

RenderComponent::RenderComponent(const std::string model, const std::string v_shader, const std::string f_shader, const std::string texture) 
{
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