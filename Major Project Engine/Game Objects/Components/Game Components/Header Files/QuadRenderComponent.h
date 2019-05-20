#pragma once

#ifndef _QUADRENDERCOMPONENT_H
#define _QUADRENDERCOMPONENT_H

#include "RenderComponent.h"

class QuadRenderComponent : public RenderComponent
{
public:
	QuadRenderComponent(const std::uint16_t & id)
		: RenderComponent(id)
	{
		model_path = std::string("Assets/Models/quad.obj");
		vertex_shader_path = std::string("Assets/Shaders/default_V_shader.glvs");
		fragment_shader_path = std::string("Assets/Shaders/default_F_shader.glfs");
		texture_path = std::string("Assets/Textures/test_texture.png");
	}

	~QuadRenderComponent() {}

private:

};

#endif // !_QUADRENDERCOMPONENT_H
