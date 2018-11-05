#pragma once
#include "RenderComponent.h"

#include <memory>

class QuadRenderComponent : public RenderComponent
{
public:
	QuadRenderComponent();
	~QuadRenderComponent();

private:
	std::shared_ptr<Model> current_model;

};

