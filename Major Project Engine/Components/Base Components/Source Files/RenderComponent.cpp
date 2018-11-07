#include "RenderComponent.h"

RenderComponent::RenderComponent() {}

RenderComponent::~RenderComponent() {
	if (current_model != nullptr) 
	{ 
		current_model.reset();
		current_model = nullptr; 
	} 
}