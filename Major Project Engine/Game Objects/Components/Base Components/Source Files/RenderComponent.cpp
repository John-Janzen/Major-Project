#include "RenderComponent.h"
#include "ThreadManager.h"

RenderComponent::RenderComponent(const std::uint16_t & id, const std::string model, const std::string v_shader, const std::string f_shader, const std::string texture)
	: BaseComponent(id) {}

RenderComponent::RenderComponent(const std::uint16_t & id)
	: BaseComponent(id)
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