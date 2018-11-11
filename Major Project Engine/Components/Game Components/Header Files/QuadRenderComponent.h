#pragma once

#ifndef _QUADRENDERCOMPONENT_H
#define _QUADRENDERCOMPONENT_H

#include "RenderComponent.h"

#include <memory>
#include <string>

static const std::string model_to_load = "Assets/Models/quad.obj";
static const std::string shader_v_load = "Assets/Shaders/default_V_shader.glvs";
static const std::string shader_f_load = "Assets/Shaders/default_F_shader.glfs";

class QuadRenderComponent : public RenderComponent
{
public:
	QuadRenderComponent();
	~QuadRenderComponent();

private:

};

#endif // !_QUADRENDERCOMPONENT_H
