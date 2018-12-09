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
	if (current_model != nullptr) current_model = nullptr;
	if (current_shader != nullptr) current_shader = nullptr;
	if (current_texture != nullptr) current_texture = nullptr;
}