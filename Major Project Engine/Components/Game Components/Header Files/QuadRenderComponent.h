#pragma once
#include "RenderComponent.h"

#include <memory>
#include <string>

static const std::string model_to_load = "Assets/quad.obj";

class QuadRenderComponent : public RenderComponent
{
public:
	QuadRenderComponent();
	~QuadRenderComponent();

private:
	
};

