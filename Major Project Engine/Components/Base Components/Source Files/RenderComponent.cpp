#include "RenderComponent.h"

RenderComponent::RenderComponent() { current_model = std::make_shared<Model>(); }

RenderComponent::~RenderComponent() { current_model.reset(); current_model = nullptr; }